#include <grpcpp/grpcpp.h>

#include <string>
#include <vector>

#include "proto/mathexpr.grpc.pb.h"
#include <google/protobuf/repeated_field.h>

using namespace std;

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using mathexpression::VariablesListReply;
using mathexpression::ExprRequest;
using mathexpression::MathExpression;

class StringReverseClient {
    public:
    StringReverseClient(shared_ptr<Channel> channel)
      : stub_(MathExpression::NewStub(channel)) {}

    vector<string> getVariablesList(string request_expr) {
        ClientContext context;
        ExprRequest request;
        VariablesListReply reply;
        
        request.set_expression(request_expr);

        Status status = stub_->getVariablesList(&context, request, &reply);

        if (status.ok()) {
            const google::protobuf::RepeatedPtrField<string>& repeatedField = reply.tokens();
            
            vector<string> vec(
                repeatedField.begin(), 
                repeatedField.end()
            );
            
            return vec;
        }
        else {
            cout << status.error_code() << ": " << status.error_message() << endl;
            return vector<string> {};
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

    string request = "(x^2) + sqrt(10)";
    vector<string> response;

    response = client.getVariablesList(request);

    if (response.size() == 0) {
        cout << "*** Error ***";
        return;
    }

    int i = 0;
    for (auto &token : response) {
        cout << "TOKEN " << i << ":" << token << endl;
        i++;
    }
}

int main(int argc, char* argv[]) {
    RunClient();

    return 0;
}
