#include <unordered_map>
#include <unistd.h>
#include <sstream>
 
#include "zmq_functions.h"
 
int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        throw std::runtime_error("Wrong args for counting node");
    }
    int curId = std::atoi(argv[1]);
    int childId = -1;
    if (argc == 3) {
        childId = std::atoi(argv[2]);
    }

    zmq::context_t context;
    zmq::socket_t parentSocket(context, ZMQ_REP); 
    connect(parentSocket, curId);

    zmq::socket_t childSocket(context, ZMQ_REQ);
    childSocket.setsockopt(ZMQ_SNDTIMEO, 5000); 
    if (childId != -1) {
        bind(childSocket, childId);
    }
 
    std::string message;
    while (true) {
        message = receive_message(parentSocket);
        std::istringstream request(message);
        int destId;
        request >> destId;

        std::string cmd;
        request >> cmd;

        if (destId == curId) {

            if (cmd == "pid") {
                send_message(parentSocket, "OK: " + std::to_string(getpid()));
            }

            else if (cmd == "create") {
                int newChildId;
                request >> newChildId;
                if (childId != -1) {
                    unbind(childSocket, childId);
                }
                bind(childSocket, newChildId);
                pid_t pid = fork();
                if (pid < 0) {
                    perror("Can't create new process");
                    return -1;
                }
                if (pid == 0) {
                    execl("./counting", "./counting", std::to_string(newChildId).c_str(), std::to_string(childId).c_str(), NULL);
                    perror("Can't execute new process");
                    return -2;
                }
                send_message(childSocket, std::to_string(newChildId) + "pid");
                childId = newChildId;
                send_message(parentSocket, receive_message(childSocket));
            }
            else if (cmd == "exec") {
                long unsigned int sum = 0;
                std::string number;
                int count;
                request >> count;
                if ( count < 1){
                    send_message(parentSocket, "Error: wrong count of numbers");
                }
                else
                {
                    while (request >> number) {
                        sum += std::stoi(number);
                    }
                    send_message(parentSocket, "OK: " + std::to_string(curId) + ": " + std::to_string(sum));
                }
            }

            else if (cmd == "heartbeat") {
                std::string reply;
                if (childId != -1) {
                    send_message(childSocket, std::to_string(childId) + " heartbeat");
                    std::string msg = receive_message(childSocket);
                    reply += " " + msg;
                }
                send_message(parentSocket, std::to_string(curId) + reply);
            }
            else if (cmd == "kill") {
                if (childId != -1) {
                    send_message(childSocket, std::to_string(childId) + " kill");
                    std::string msg = receive_message(childSocket);
                    if (msg == "OK") {
                        send_message(parentSocket, "OK");
                    }
                    unbind(childSocket, childId);
                    disconnect(parentSocket, curId);
                    break;
                }
                send_message(parentSocket, "OK");
                disconnect(parentSocket, curId);
                break;
            }
        }
        else if (childId != -1) {
            send_message(childSocket, message);
            send_message(parentSocket, receive_message(childSocket));
            if (childId == destId && cmd == "kill") {
                childId = -1;
            }
        }
        else {
            send_message(parentSocket, "Error: node is unavailable");
        }
    }
}
