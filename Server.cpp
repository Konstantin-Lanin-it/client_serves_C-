#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

class TCPClient {
private:
    std::string clientName;
    std::string serverIP;
    int serverPort;
    int connectionPeriod;

public:
    TCPClient(const std::string& name, const std::string& ip, int port, int period)
        : clientName(name), serverIP(ip), serverPort(port), connectionPeriod(period) {}

    void run() {
        struct sockaddr_in serverAddr;
        std::memset(&serverAddr, 0, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
        serverAddr.sin_port = htons(serverPort);

        while (true) {
            int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (clientSocket < 0) {
                std::cerr << "Error creating socket" << std::endl;
                exit(1);
            }

            if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
                std::cerr << "Error connecting to server" << std::endl;
                exit(1);
            }

            std::string message = "[" + getCurrentTime() + "] \"" + clientName + "\"";

            if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
                std::cerr << "Error sending message to server" << std::endl;
            }

            close(clientSocket);

            sleep(connectionPeriod);
        }
    }

private:
    std::string getCurrentTime() const {
        time_t now = time(nullptr);
        struct tm* timeinfo = localtime(&now);

        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S.000", timeinfo);

        return std::string(buffer);
    }
};
