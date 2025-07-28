#include "SimpleServer.h"
#include <iostream>
#include <thread>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cstring>
#endif

namespace http {

SimpleServer::SimpleServer(int port) : port_(port), serverSocket_(-1), running_(false) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
#endif
}

SimpleServer::~SimpleServer() {
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

void SimpleServer::setupSocket() {
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    int opt = 1;
    if (setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, 
                   reinterpret_cast<const char*>(&opt), sizeof(opt)) < 0) {
        throw std::runtime_error("Failed to set socket options");
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(serverSocket_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Failed to bind socket to port " + std::to_string(port_));
    }

    if (listen(serverSocket_, 10) < 0) {
        throw std::runtime_error("Failed to listen on socket");
    }
}

std::unordered_map<std::string, std::unordered_map<std::string, RequestHandler>> routes_;

void SimpleServer::addRoute(const std::string& method, const std::string& path, RequestHandler handler) {
    routes_[method][path] = handler;
}

void SimpleServer::run() {
    setupSocket();
    running_ = true;
    
    std::cout << "🚀 Smart Workout Planner Server started on http://localhost:" << port_ << std::endl;
    std::cout << "📡 API endpoints:" << std::endl;
    std::cout << "   GET  /api/health" << std::endl;
    std::cout << "   GET  /api/users/status" << std::endl;
    std::cout << "   POST /api/users/analyze" << std::endl;
    std::cout << "💡 Press Ctrl+C to stop the server" << std::endl;

    while (running_) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        
        int clientSocket = accept(serverSocket_, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket >= 0) {
            std::thread clientThread(&SimpleServer::handleClient, this, clientSocket);
            clientThread.detach();
        }
    }
}

void SimpleServer::stop() {
    running_ = false;
    if (serverSocket_ >= 0) {
#ifdef _WIN32
        closesocket(serverSocket_);
#else
        close(serverSocket_);
#endif
        serverSocket_ = -1;
    }
}

void SimpleServer::handleClient(int clientSocket) {
    char buffer[4096];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string rawRequest(buffer);
        
        try {
            Request request = parseRequest(rawRequest);
            Response response;
            response.setCORS();

            // Handle OPTIONS request for CORS
            if (request.method == "OPTIONS") {
                response.statusCode = 200;
                response.body = "";
            } else {
                // Find matching route
                auto methodIt = routes_.find(request.method);
                if (methodIt != routes_.end()) {
                    auto pathIt = methodIt->second.find(request.path);
                    if (pathIt != methodIt->second.end()) {
                        pathIt->second(request, response);
                    } else {
                        response.statusCode = 404;
                        response.setJson("{\"error\":\"Not found\"}");
                    }
                } else {
                    response.statusCode = 405;
                    response.setJson("{\"error\":\"Method not allowed\"}");
                }
            }

            std::string responseStr = buildResponse(response);
            send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
            
        } catch (const std::exception& e) {
            std::string errorResponse = "HTTP/1.1 500 Internal Server Error\r\n"
                                      "Content-Type: application/json\r\n"
                                      "Access-Control-Allow-Origin: *\r\n\r\n"
                                      "{\"error\":\"Internal server error\"}";
            send(clientSocket, errorResponse.c_str(), errorResponse.length(), 0);
        }
    }

#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}

Request SimpleServer::parseRequest(const std::string& rawRequest) {
    Request request;
    std::istringstream stream(rawRequest);
    std::string line;
    
    // Parse request line
    if (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        lineStream >> request.method >> request.path;
    }
    
    // Parse headers
    while (std::getline(stream, line) && !line.empty() && line != "\r") {
        if (line.back() == '\r') line.pop_back();
        
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string name = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            
            // Trim whitespace
            name.erase(0, name.find_first_not_of(' '));
            name.erase(name.find_last_not_of(' ') + 1);
            value.erase(0, value.find_first_not_of(' '));
            value.erase(value.find_last_not_of(' ') + 1);
            
            request.headers[name] = value;
        }
    }
    
    // Parse body
    std::string body;
    while (std::getline(stream, line)) {
        body += line + "\n";
    }
    if (!body.empty() && body.back() == '\n') {
        body.pop_back();
    }
    request.body = body;
    
    return request;
}

std::string SimpleServer::buildResponse(const Response& response) {
    std::ostringstream stream;
    
    // Status line
    stream << "HTTP/1.1 " << response.statusCode;
    switch (response.statusCode) {
        case 200: stream << " OK"; break;
        case 400: stream << " Bad Request"; break;
        case 404: stream << " Not Found"; break;
        case 405: stream << " Method Not Allowed"; break;
        case 500: stream << " Internal Server Error"; break;
        default: stream << " Unknown"; break;
    }
    stream << "\r\n";
    
    // Headers
    stream << "Content-Length: " << response.body.length() << "\r\n";
    for (const auto& header : response.headers) {
        stream << header.first << ": " << header.second << "\r\n";
    }
    stream << "\r\n";
    
    // Body
    stream << response.body;
    
    return stream.str();
}

}