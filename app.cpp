#include <iostream>
#include "TCPClient.h"
#include "TCPServer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        return 1;
    }

    int serverPort = std::stoi(argv[1]);

    TCPServer server(serverPort);
    std::thread serverThread([&
