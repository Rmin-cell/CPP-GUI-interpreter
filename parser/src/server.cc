#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>

#include <string>
#include <vector>
#include <cmath>

#include "proto/mathexpr.grpc.pb.h"

#include "tinyexpr.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using mathexpression::VariablesListReply;
using mathexpression::ExprRequest;
using mathexpression::MathExpression;
using mathexpression::DataPointsReply;
using mathexpression::DataPointsRequest;

class ReverseServiceImplementation final : public MathExpression::Service {
    Status getVariablesList(
        ServerContext* context,
        const ExprRequest* request,
        VariablesListReply* reply
    ) override {
        vector<string> tokens;
        
        string expression = request->expression();
        const char* cstr_expression = expression.data();
        
        char** tokens_list = parse_tokens(cstr_expression);

        if (tokens_list != NULL) {
            // Loop through the tokens until NULL is encountered
            for (size_t i = 0; tokens_list[i] != NULL; i++) {
                // tokens[i] = tokens_list[i];
                reply->add_tokens(tokens_list[i]);
            }

            // Free memory after printing
            for (size_t i = 0; tokens_list[i] != NULL; i++) {
                free(tokens_list[i]); // Free each string
            }
            free(tokens_list); // Free the array itself
        }

        // reply->set_tokens(tokens_list);
        // for (auto& temp_token : tokens) {
        //     reply->add_tokens();
        // }
        // reply->mutable_tokens() = {tokens.begin(), tokens.end()};

        return Status::OK;
    }

    //

    Status getDataPoints(
        ServerContext* context,
        const DataPointsRequest* request,
        DataPointsReply* reply
    ) override {
        string expression = request->expression();
        const char* cstr_expression = expression.data();
        
        const google::protobuf::RepeatedPtrField<mathexpression::VariableWithRange>& ranges = request->variable_ranges();
        int number_of_variables = ranges.size();

        // ui_variable* ui_v = (ui_variable *) (malloc(sizeof(ui_variable) * number_of_variables));
        ui_variable* ui_v = new ui_variable[number_of_variables];
        
        for (int i = 0; i < number_of_variables; i++) {
            ui_v[i].name = strdup(ranges[i].variable_name().data());
            ui_v[i].min = ranges[i].min();
            ui_v[i].max = ranges[i].max();
        }
        
        for (int i = 0; i < number_of_variables; i++) {
            printf("%s - %f - %f\n", ui_v[i].name, ui_v[i].min, ui_v[i].max);
        }

        double *points = get_data_points(cstr_expression, number_of_variables, ui_v);

        if (points != NULL) {
            // Loop through the tokens until NULL is encountered
            size_t total_combinations = (size_t) (ceil(pow(NUMBER_OF_DATA_POINTS, number_of_variables)));
            for (size_t i = 0; i < total_combinations; i++) {
                reply->add_points(points[i]);
            }

            free(points); // Free the array itself
        }

        delete[] ui_v;

        return Status::OK;
    }
};

void CustomLogFunction(gpr_log_func_args* args) {
    std::cout << "gRPC Log: " << args->message << std::endl;
}

void RunServer() {
    string server_address("0.0.0.0:5005");
    ReverseServiceImplementation service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on port: " << server_address << endl;

    // gpr_set_log_verbosity(GPR_LOG_SEVERITY_DEBUG);
    gpr_set_log_function(CustomLogFunction);

    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
