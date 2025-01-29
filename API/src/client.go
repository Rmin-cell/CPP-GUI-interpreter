package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"time"

	pb "API/proto"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

type MathExpressionClient struct {
	client pb.MathExpressionClient
}

func NewMathExpressionClient(conn *grpc.ClientConn) *MathExpressionClient {
	return &MathExpressionClient{
		client: pb.NewMathExpressionClient(conn),
	}
}

func (c *MathExpressionClient) GetVariablesList(ctx context.Context, expression string) ([]string, error) {
	req := &pb.ExprRequest{
		Expression: expression,
	}

	resp, err := c.client.GetVariablesList(ctx, req)
	if err != nil {
		return nil, fmt.Errorf("could not get variables: %v", err)
	}
	return resp.GetTokens(), nil
}

func (c *MathExpressionClient) GetDataPoints(ctx context.Context, expression string, variables []*pb.VariableWithRange) ([]float64, error) {
	req := &pb.DataPointsRequest{
		Expression:      expression,
		VariableRanges: variables,
	}

	resp, err := c.client.GetDataPoints(ctx, req)
	if err != nil {
		return nil, fmt.Errorf("gRPC error: %v", err)
	}

	return resp.GetPoints(), nil
}

// CORS middleware
func corsMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type")

		if r.Method == http.MethodOptions {
			w.WriteHeader(http.StatusNoContent)
			return
		}

		next.ServeHTTP(w, r)
	})
}

func runRESTAPI(client *MathExpressionClient) {
	mux := http.NewServeMux()

	// Serve static files from the "static" directory
	fs := http.FileServer(http.Dir("static"))
	mux.Handle("/static/", http.StripPrefix("/static/", fs))

	// Serve main.html at the root endpoint
	mux.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, "static/main.html")
	})

	mux.HandleFunc("/variables", func(w http.ResponseWriter, r *http.Request) {
		handleVariablesEndpoint(client, w, r)
	})

	mux.HandleFunc("/points", func(w http.ResponseWriter, r *http.Request) {
		handlePointsEndpoint(client, w, r)
	})

	log.Println("REST API is running on :8080")
	log.Fatal(http.ListenAndServe(":8080", corsMiddleware(mux)))
}

func handleVariablesEndpoint(client *MathExpressionClient, w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Only POST requests are allowed", http.StatusMethodNotAllowed)
		return
	}

	var reqBody struct {
		Expression string `json:"expression"`
	}

	if err := json.NewDecoder(r.Body).Decode(&reqBody); err != nil {
		http.Error(w, "Invalid request body", http.StatusBadRequest)
		return
	}

	ctx, cancel := context.WithTimeout(r.Context(), 5*time.Second)
	defer cancel()

	tokens, err := client.GetVariablesList(ctx, reqBody.Expression)
	if err != nil {
		http.Error(w, fmt.Sprintf("Error calling gRPC: %v", err), http.StatusInternalServerError)
		return
	}

	sendJSONResponse(w, http.StatusOK, map[string]interface{}{"tokens": tokens})
}

func handlePointsEndpoint(client *MathExpressionClient, w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Only POST requests are allowed", http.StatusMethodNotAllowed)
		return
	}

	var reqBody struct {
		Expression string          `json:"expression"`
		Variables  [][]interface{} `json:"variables"`
	}

	if err := json.NewDecoder(r.Body).Decode(&reqBody); err != nil {
		http.Error(w, "Invalid request body", http.StatusBadRequest)
		return
	}

	// Convert variables to protobuf format
	grpcVars, err := convertVariables(reqBody.Variables)
	if err != nil {
		http.Error(w, err.Error(), http.StatusBadRequest)
		return
	}

	ctx, cancel := context.WithTimeout(r.Context(), 10*time.Second)
	defer cancel()

	points, err := client.GetDataPoints(ctx, reqBody.Expression, grpcVars)
	if err != nil {
		http.Error(w, fmt.Sprintf("Error calling gRPC: %v", err), http.StatusInternalServerError)
		return
	}

	sendJSONResponse(w, http.StatusOK, map[string]interface{}{"points": points})
}

func convertVariables(restVars [][]interface{}) ([]*pb.VariableWithRange, error) {
	grpcVars := make([]*pb.VariableWithRange, 0, len(restVars))
	
	for i, v := range restVars {
		if len(v) != 3 {
			return nil, fmt.Errorf("invalid variable format at position %d - expected [name, min, max]", i)
		}

		name, ok1 := v[0].(string)
		min, ok2 := v[1].(float64)
		max, ok3 := v[2].(float64)
		
		if !ok1 || !ok2 || !ok3 {
			return nil, fmt.Errorf("invalid variable types at position %d - expected [string, number, number]", i)
		}

		grpcVars = append(grpcVars, &pb.VariableWithRange{
			VariableName: name,
			Min:          min,
			Max:          max,
		})
	}
	
	return grpcVars, nil
}

func sendJSONResponse(w http.ResponseWriter, statusCode int, data interface{}) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(statusCode)
	if err := json.NewEncoder(w).Encode(data); err != nil {
		log.Printf("Error encoding response: %v", err)
	}
}

func main() {
	serverAddress := "localhost:5005"

	conn, err := grpc.Dial(serverAddress, 
		grpc.WithTransportCredentials(insecure.NewCredentials()),
		grpc.WithBlock(),
		grpc.WithTimeout(5*time.Second),
	)
	if err != nil {
		log.Fatalf("Failed to connect to gRPC server: %v", err)
	}
	defer conn.Close()

	client := NewMathExpressionClient(conn)
	runRESTAPI(client)
}
