#pragma once

#include "../utils/SimpleServer.h"

using namespace http;

namespace api {

class UserController
{
public:
    // Health check
    void health(const Request& req, Response& response);

    // Analyze user profile and return BMI, calorie recommendations, workout plan
    void analyzeProfile(const Request& req, Response& response);
    
    // Generate detailed workout plan
    void generateWorkoutPlan(const Request& req, Response& response);
    
    // Get exercise database
    void getExercises(const Request& req, Response& response);

    // Get server status
    void getStatus(const Request& req, Response& response);
};

}