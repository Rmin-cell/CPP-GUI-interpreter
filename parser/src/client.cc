#include <grpcpp/grpcpp.h>

#include <string>

#include "proto/mathexpr.grpc.pb.h"

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using mathexpression::ExprReply;
using mathexpression::ExprRequest;
using mathexpression::MathExpression;

class StringReverseClient {
    public:
    StringReverseClient(shared_ptr<Channel> channel)
      : stub_(MathExpression::NewStub(channel)) {}

    uint32_t sendRequest(string request_expr) {
        ClientContext context;
        ExprRequest request;
        ExprReply reply;
        
        request.set_expression(request_expr);

        Status status = stub_->sendRequest(&context, request, &reply);

        if (status.ok()) {
            return reply.time_to_compile();
        }
        else {
            cout << status.error_code() << ": " << status.error_message() << endl;
            return 0xffffff;
        }
    }

    private:
    unique_ptr<MathExpression::Stub> stub_;
};

void RunClient() {
    string target_address("0.0.0.0:5005");
    
    StringReverseClient client(
        grpc::CreateChannel(
                target_address,
                grpc::InsecureChannelCredentials()
            )
        );

    string request = "sqrt(5^2+7^2+11^2+(8-2)^2)";
    uint32_t response;

    response = client.sendRequest(request);

    if (response == 0xffffff) {
        cout << "*** Error ***";
        return;
    }

    cout << "The expression: \n\t" << request << endl;
    cout << "Time to Compile: " << response << endl;
}

int main(int argc, char* argv[]) {
    RunClient();

    return 0;
}
