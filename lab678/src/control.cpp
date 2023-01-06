#include <unistd.h>
#include <sstream>
#include <set>
#include <thread>
#include "zmq_functions.h"
#include "topology.h"

void Heartbeat(int time, std::vector<zmq::socket_t>& branches, topology& network){
    while(1)
    {
        std::set<int> availableNodes;

        for (size_t i = 0; i < branches.size(); ++i) {
            int firstNodeId = network.get_first_id(i);
            send_message(branches[i], std::to_string(firstNodeId) + " heartbeat");
            
            std::string receivedMessage = receive_message(branches[i]);
            std::istringstream reply(receivedMessage);
            int node;
            while(reply >> node) {
                availableNodes.insert(node);
            }
        }
        std::cout << "OK: ";
        if (availableNodes.empty()) {
            std::cout << "No available nodes" << std::endl;
        }
        else {
            for (auto v : availableNodes) {
                std::cout << v << " ";
            }
            std::cout << std::endl;
        }
        sleep(time);
    }
}

int main() {
    topology network;
    std::vector<zmq::socket_t> branches;
    zmq::context_t context; 

    std::string cmd;
    while (std::cin >> cmd) {

        if (cmd == "create") {
            int nodeId, parentId;
            std::cin >> nodeId >> parentId;

            if (network.find(nodeId) != -1) {
                std::cout << "Error: already exists" << std::endl;
            }
            else if (parentId == -1)
            {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("Can't create new process");
                    return -1;
                }
                if (pid == 0) {
                    if (execl("./counting", "./counting", std::to_string(nodeId).c_str(), NULL) < 0) {
                        perror("Can't execute new process");
                        return -2;
                    } 
                }
                
                branches.emplace_back(context, ZMQ_REQ); 
                branches[branches.size() - 1].setsockopt(ZMQ_SNDTIMEO, 5000);
                bind(branches[branches.size() - 1], nodeId);
                send_message(branches[branches.size() - 1], std::to_string(nodeId) + "pid");
                
                std::string reply = receive_message(branches[branches.size() - 1]);
                std::cout << reply << std::endl;
                network.insert(nodeId, parentId);
            }
            else if (network.find(parentId) == -1) {
                std::cout << "Error: parent not found" << std::endl;
            }
            else {
                int branch = network.find(parentId);
                send_message(branches[branch], std::to_string(parentId) + "create " + std::to_string(nodeId));

                std::string reply = receive_message(branches[branch]);
                std::cout << reply << std::endl;
                network.insert(nodeId, parentId);
            }
        }
        else if (cmd == "exec") {
            int destId;
            std::string numbers;
            std::cin >> destId;
            std::getline(std::cin, numbers);
            int branch = network.find(destId);
            if (branch == -1) {
                std::cout << "ERROR: incorrect node id" << std::endl;
            }
            else {
                send_message(branches[branch], std::to_string(destId) + "exec " + numbers);
                std::string reply = receive_message(branches[branch]);
                std::cout << reply << std::endl;
            }
        }
        else if (cmd == "kill") {
            int id;
            std::cin >> id;
            int branch = network.find(id);
            if (branch == -1) {
                std::cout << "ERROR: incorrect node id" << std::endl;
            }
            else {
                bool isFirst = (network.get_first_id(branch) == id);
                send_message(branches[branch], std::to_string(id) + " kill");

                std::string reply = receive_message(branches[branch]);
                std::cout << reply << std::endl;
                network.erase(id);
                if (isFirst) {
                    unbind(branches[branch], id);
                    branches.erase(branches.begin() + branch);
                }
            }
        }
        else if (cmd == "heartbeat") {
            int time;
            std::cin >> time;

            std::thread thr(Heartbeat, time, std::ref(branches), std::ref(network));
            thr.detach();
        }
        else if (cmd == "exit") {
            for (size_t i = 0; i < branches.size(); ++i) {
                int firstNodeId = network.get_first_id(i);
                send_message(branches[i], std::to_string(firstNodeId) + " kill");
                
                std::string reply = receive_message(branches[i]);
                if (reply != "OK") {
                    std::cout << reply << std::endl;
                }
                else {
                    unbind(branches[i], firstNodeId);
                }
            }
            exit(0);
        }
        else {
            std::cout << "Incorrect cmd" << std::endl;
        }
    }
}