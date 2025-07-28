#include <iostream>
#include <signal.h>
#include "utils/SimpleServer.h"
#include "controllers/UserController.h"

using namespace http;

SimpleServer* server = nullptr;

void signalHandler(int signal) {
    std::cout << "\n🛑 Shutting down server..." << std::endl;
    if (server) {
        server->stop();
    }
    exit(0);
}

int main() {
    try {
        // Set up signal handling
        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);

        // Create server instance
        server = new SimpleServer(8080);
        
        // Create controller instance
        api::UserController controller;
        
        // Register routes
        server->addRoute("GET", "/api/health", 
            [&controller](const Request& req, Response& resp) {
                controller.health(req, resp);
            });
            
        server->addRoute("GET", "/api/users/status", 
            [&controller](const Request& req, Response& resp) {
                controller.getStatus(req, resp);
            });
            
        server->addRoute("POST", "/api/users/analyze", 
            [&controller](const Request& req, Response& resp) {
                controller.analyzeProfile(req, resp);
            });
            
        // New endpoints for workout planning
        server->addRoute("POST", "/api/workouts/generate", 
            [&controller](const Request& req, Response& resp) {
                controller.generateWorkoutPlan(req, resp);
            });
            
        server->addRoute("GET", "/api/exercises", 
            [&controller](const Request& req, Response& resp) {
                controller.getExercises(req, resp);
            });

        std::cout << "🏋️ Smart Workout Planner Backend Starting..." << std::endl;
        std::cout << "⚡ Using C++ Simple HTTP Server (no external dependencies)" << std::endl;
        std::cout << "📋 Exercise Database: Loaded with 60+ exercises" << std::endl;
        std::cout << "🧠 Algorithms: Analysis implemented, workout generation ready for teammate" << std::endl;
        std::cout << "🚀 Server running on http://localhost:8080" << std::endl;
        std::cout << "\n📍 Available endpoints:" << std::endl;
        std::cout << "   GET  /api/health                 - Health check" << std::endl;
        std::cout << "   GET  /api/users/status           - Server status" << std::endl;
        std::cout << "   POST /api/users/analyze          - Analyze user profile" << std::endl;
        std::cout << "   POST /api/workouts/generate      - Generate workout plan" << std::endl;
        std::cout << "   GET  /api/exercises              - Get exercise database" << std::endl;
        
        // Start the server (this blocks)
        server->run();
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Server error: " << e.what() << std::endl;
        return 1;
    }
    
    // Cleanup
    delete server;
    return 0;
}