#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <thread>
#include <mutex>

std::mutex fileMutex;

class TCPServer {
private:
    int serverPort;

public:
    TCPServer(int port) : serverPort(port) {}

    void run() {
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            std::cerr << "Error creating socket" << std::endl;
            exit(1);
        }

        struct sockaddr_in serverAddr;
        std::memset(&serverAddr, 0, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(serverPort);

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error binding socket" << std::endl;
            exit(1);
        }

        if (listen(serverSocket, 10) < 0) {
            std::cerr << "Error listening" << std::endl;
            exit(1);
        }

        while (true) {
            int clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket < 0) {
                std::cerr << "Error accepting client connection" << std::endl;
                exit(1);
            }

            std::thread clientThread([this, clientSocket]() {
                handleClient(clientSocket);
            });
            clientThread.detach();
        }

        close(serverSocket);
    }

private:
    void handleClient(int clientSocket) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
            std::cerr << "Error receiving message from client" << std::endl;
        }

        std::string message = buffer;

        fileMutex.lock();
        std::ofstream logFile("log.txt", std::ios::out | std::ios::app);
        logFile << message << std::endl;
        logFile.close();
        fileMutex.unlock();

        close(clientSocket);
    }
};
