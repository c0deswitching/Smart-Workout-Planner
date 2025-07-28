#include "UserController.h"
#include "../utils/SimpleJson.h"
#include "../services/WorkoutPlanningService.h"
#include "../models/UserModels.h"
#include <cmath>
#include <ctime>
#include <iostream>

using namespace http;
using namespace json;

namespace api {

void UserController::health(const Request& req, Response& response)
{
    SimpleJson jsonResponse;
    jsonResponse.setString("status", "ok");
    jsonResponse.setString("message", "Smart Workout Planner API is running");
    jsonResponse.setInt("timestamp", static_cast<int64_t>(std::time(nullptr)));
    
    response.setJson(jsonResponse.toString());
    response.statusCode = 200;
}

void UserController::analyzeProfile(const Request& req, Response& response)
{
    try {
        std::cout << "📨 Received analyze request" << std::endl;
        std::cout << "📋 Request body: " << req.body << std::endl;
        
        if (req.body.empty()) {
            std::cout << "❌ Empty request body" << std::endl;
            SimpleJson error;
            error.setString("error", "Request body is empty");
            response.setJson(error.toString());
            response.statusCode = 400;
            return;
        }

        SimpleJson requestData = SimpleJson::parse(req.body);
        std::cout << "✅ JSON parsed successfully" << std::endl;
        
        // Create UserProfile from request data
        models::UserProfile userProfile;
        
        // Basic information
        userProfile.height_cm = requestData.getDouble("height", 170.0);
        userProfile.weight_kg = requestData.getDouble("weight", 70.0);
        userProfile.age = requestData.getInt("age", 25);
        userProfile.gender = requestData.getString("gender", "male");
        userProfile.training_intensity = requestData.getInt("intensity", 5);
        
        std::cout << "👤 User Profile: H:" << userProfile.height_cm 
                  << " W:" << userProfile.weight_kg 
                  << " A:" << userProfile.age 
                  << " G:" << userProfile.gender 
                  << " I:" << userProfile.training_intensity << std::endl;
        
        // TODO: Parse arrays from JSON properly (for now using defaults)
        userProfile.available_days = {"Monday", "Tuesday", "Thursday", "Friday"};
        userProfile.muscle_priorities = {
            {"Chest", "Medium"},
            {"Back", "Medium"},
            {"Shoulders", "Medium"},
            {"Arms", "Medium"},
            {"Legs", "Medium"},
            {"Core", "Medium"},
            {"Cardio", ""}
        };
        userProfile.available_equipment = {"Bodyweight", "Dumbbells", "Bench", "Barbell"};
        
        // Analyze user profile using new service
        models::AnalysisResults analysis = services::AnalysisService::AnalyzeUserProfile(userProfile);
        
        std::cout << "📊 Analysis complete: BMI:" << analysis.bmi 
                  << " Cal:" << analysis.daily_calories 
                  << " Burn:" << analysis.weekly_calorie_burn << std::endl;
        
        // Generate workout plan (placeholder for algorithm implementation)
        models::WorkoutPlan workoutPlan = services::WorkoutPlanningService::GenerateWorkoutPlan(userProfile);
        
        std::cout << "💪 Workout plan generated: " << workoutPlan.name << std::endl;
        
        // Build response with MOCK DATA for demonstration
        SimpleJson responseJson;
        responseJson.setBool("success", true);
        
        // Analysis results
        SimpleJson analysisJson;
        analysisJson.setDouble("bmi", std::round(analysis.bmi * 10) / 10.0);
        analysisJson.setString("bmiStatus", analysis.bmi_status);
        analysisJson.setInt("dailyCalories", analysis.daily_calories);
        analysisJson.setInt("weeklyBurn", analysis.weekly_calorie_burn);
        analysisJson.setInt("bmr", static_cast<int>(std::round(analysis.bmr)));
        
        responseJson.setObject("analysis", analysisJson);
        
        // Workout plan info with MOCK DATA
        SimpleJson planJson;
        planJson.setString("id", workoutPlan.id);
        planJson.setString("name", workoutPlan.name);
        planJson.setString("description", workoutPlan.description);
        planJson.setString("created_at", workoutPlan.created_at);
        
        // Add MOCK weekly schedule for demonstration
        std::string mockSchedule = "{"
            "\"Monday\": {\"type\": \"strength\", \"focus\": \"Chest + Triceps\", \"duration\": 60},"
            "\"Tuesday\": {\"type\": \"cardio\", \"focus\": \"HIIT Training\", \"duration\": 30},"
            "\"Wednesday\": {\"type\": \"rest\", \"focus\": \"Recovery\", \"duration\": 0},"
            "\"Thursday\": {\"type\": \"strength\", \"focus\": \"Back + Biceps\", \"duration\": 60},"
            "\"Friday\": {\"type\": \"strength\", \"focus\": \"Legs + Glutes\", \"duration\": 75},"
            "\"Saturday\": {\"type\": \"cardio\", \"focus\": \"Light Cardio\", \"duration\": 30},"
            "\"Sunday\": {\"type\": \"rest\", \"focus\": \"Recovery\", \"duration\": 0}"
            "}";
        
        planJson.setString("mockSchedule", mockSchedule);
        planJson.setString("status", "MOCK DATA - Algorithm implementation pending");
        
        responseJson.setObject("workoutPlan", planJson);
        responseJson.setObject("userData", requestData);

        std::cout << "🚀 Sending response..." << std::endl;
        response.setJson(responseJson.toString());
        response.statusCode = 200;

    } catch (const std::exception& e) {
        std::cout << "💥 Exception caught: " << e.what() << std::endl;
        SimpleJson error;
        error.setString("error", "Internal server error");
        error.setString("message", e.what());
        response.setJson(error.toString());
        response.statusCode = 500;
    }
}

void UserController::generateWorkoutPlan(const Request& req, Response& response)
{
    try {
        if (req.body.empty()) {
            SimpleJson error;
            error.setString("error", "Request body is empty");
            response.setJson(error.toString());
            response.statusCode = 400;
            return;
        }

        SimpleJson requestData = SimpleJson::parse(req.body);
        
        // Create UserProfile from request data (same parsing as above)
        models::UserProfile userProfile;
        userProfile.height_cm = requestData.getDouble("height", 170.0);
        userProfile.weight_kg = requestData.getDouble("weight", 70.0);
        userProfile.age = requestData.getInt("age", 25);
        userProfile.gender = requestData.getString("gender", "male");
        userProfile.training_intensity = requestData.getInt("intensity", 5);
        
        // TODO: Parse arrays from JSON properly
        userProfile.available_days = {"Monday", "Tuesday", "Thursday", "Friday"};
        userProfile.muscle_priorities = {
            {"Chest", "Medium"},
            {"Back", "Medium"},
            {"Shoulders", "Medium"},
            {"Arms", "Medium"},
            {"Legs", "Medium"},
            {"Core", "Medium"},
            {"Cardio", ""}
        };
        userProfile.available_equipment = {"Bodyweight", "Dumbbells", "Bench", "Barbell"};
        
        // Generate workout plan
        models::WorkoutPlan workoutPlan = services::WorkoutPlanningService::GenerateWorkoutPlan(userProfile);
        
        // Build detailed response
        SimpleJson responseJson;
        responseJson.setBool("success", true);
        
        SimpleJson planJson;
        planJson.setString("id", workoutPlan.id);
        planJson.setString("name", workoutPlan.name);
        planJson.setString("description", workoutPlan.description);
        planJson.setString("created_at", workoutPlan.created_at);
        
        // TODO: Add weekly schedule details once algorithm is implemented
        // For now, return placeholder message
        planJson.setString("status", "Plan generation algorithm pending implementation by teammate");
        
        responseJson.setObject("workoutPlan", planJson);

        response.setJson(responseJson.toString());
        response.statusCode = 200;

    } catch (const std::exception& e) {
        SimpleJson error;
        error.setString("error", "Internal server error");
        error.setString("message", e.what());
        response.setJson(error.toString());
        response.statusCode = 500;
    }
}

void UserController::getExercises(const Request& req, Response& response)
{
    try {
        // Get all exercises from database
        auto exercises = models::ExerciseDatabase::GetAllExercises();
        
        // Build response
        SimpleJson responseJson;
        responseJson.setBool("success", true);
        responseJson.setInt("count", static_cast<int>(exercises.size()));
        
        // TODO: Convert exercises to JSON array
        // For now, return count and placeholder message
        responseJson.setString("message", "Exercise database loaded successfully");
        responseJson.setString("note", "Exercise data serialization pending - algorithm implementation needed");
        
        response.setJson(responseJson.toString());
        response.statusCode = 200;

    } catch (const std::exception& e) {
        SimpleJson error;
        error.setString("error", "Internal server error");
        error.setString("message", e.what());
        response.setJson(error.toString());
        response.statusCode = 500;
    }
}

void UserController::getStatus(const Request& req, Response& response)
{
    SimpleJson jsonResponse;
    jsonResponse.setString("server", "Smart Workout Planner Backend");
    jsonResponse.setString("version", "1.0.0");
    jsonResponse.setString("status", "running");
    jsonResponse.setString("framework", "C++ Drogon-like Simple HTTP Server");
    
    // Add features array manually as a string
    std::string featuresJson = "["
        "\"BMI calculation\","
        "\"Calorie analysis\","
        "\"Workout planning\","
        "\"Exercise database\","
        "\"Muscle group prioritization\","
        "\"Equipment filtering\","
        "\"Edge case handling\""
        "]";
    
    // Add algorithm status
    std::string algorithmsJson = "["
        "\"Analysis algorithms: IMPLEMENTED\","
        "\"Workout generation: PLACEHOLDER (ready for teammate)\","
        "\"Exercise filtering: PLACEHOLDER (ready for teammate)\","
        "\"Edge case handling: PLACEHOLDER (ready for teammate)\""
        "]";
    
    // Build the full response manually to include the arrays
    std::string fullResponse = "{"
        "\"server\":\"Smart Workout Planner Backend\","
        "\"version\":\"1.0.0\","
        "\"status\":\"running\","
        "\"framework\":\"C++ Drogon-like Simple HTTP Server\","
        "\"features\":" + featuresJson + ","
        "\"algorithms\":" + algorithmsJson +
        "}";
    
    response.setJson(fullResponse);
    response.statusCode = 200;
}

}