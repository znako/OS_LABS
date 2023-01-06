#include <zmq.hpp>
#include <iostream>
 
const int MAIN_PORT = 4040;
 
void send_message(zmq::socket_t& socket, const std::string& msg) {
    zmq::message_t message(msg.size());
    memcpy(message.data(), msg.c_str(), msg.size());
    socket.send(message, zmq :: send_flags ::none);
}
 
std::string receive_message(zmq::socket_t& socket) {
    zmq::message_t message;
    bool charsRead;
    try {
        charsRead = socket.recv(&message);
    }
    catch (...) {
        charsRead = false;
    }
    if (charsRead == 0) {
        return "Error: node is unavailable [zmq_func]";
    }
    std::string receivedMsg(static_cast<char*>(message.data()), message.size());

    return receivedMsg;
}
 
void connect(zmq::socket_t& socket, int id) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(MAIN_PORT + id);
    socket.connect(address);
}

void disconnect(zmq::socket_t& socket, int id) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(MAIN_PORT + id);
    socket.disconnect(address);
}

void bind(zmq::socket_t& socket, int id) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(MAIN_PORT + id);
    socket.bind(address);
}

void unbind(zmq::socket_t& socket, int id) {
    std::string address = "tcp://127.0.0.1:" + std::to_string(MAIN_PORT + id);
    socket.unbind(address);
}