#include <bits/stdc++.h>
#include <grpcpp/grpcpp.h>

#include <string>

#include "proto/mathexpr.grpc.pb.h"

using namespace std;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using mathexpression::ExprReply;
using mathexpression::ExprRequest;
using mathexpression::MathExpression;

class ReverseServiceImplementation final : public MathExpression::Service {
    Status sendRequest(
        ServerContext* context,
        const ExprRequest* request,
        ExprReply* reply
    ) override {
        string expression = request->expression();

        reply->set_time_to_compile(23);
        return Status::OK;
    }
};

void RunServer() {
    string server_address("0.0.0.0:5005");
    ReverseServiceImplementation service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on port: " << server_address << endl;

    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
