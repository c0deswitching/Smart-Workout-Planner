#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

namespace http {

struct Request {
    std::string method;
    std::string path;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    
    std::string getHeader(const std::string& name) const {
        auto it = headers.find(name);
        return (it != headers.end()) ? it->second : "";
    }
};

struct Response {
    int statusCode = 200;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
    
    void setHeader(const std::string& name, const std::string& value) {
        headers[name] = value;
    }
    
    void setJson(const std::string& jsonBody) {
        body = jsonBody;
        setHeader("Content-Type", "application/json");
    }
    
    void setCORS() {
        setHeader("Access-Control-Allow-Origin", "*");
        setHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        setHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    }
};

using RequestHandler = std::function<void(const Request&, Response&)>;

class SimpleServer {
public:
    SimpleServer(int port = 8080);
    ~SimpleServer();
    
    void addRoute(const std::string& method, const std::string& path, RequestHandler handler);
    void run();
    void stop();

private:
    int port_;
    int serverSocket_;
    bool running_;
    
    void handleClient(int clientSocket);
    Request parseRequest(const std::string& rawRequest);
    std::string buildResponse(const Response& response);
    void setupSocket();
};

}