#include "UserController.h"
#include "../utils/SimpleJson.h"
#include "../services/AlgorithmAdapter.h"
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
        
        // Parse availableDays array using new JSON array parsing
        std::cout << "🔍 Debug - availableDays raw value: '" << requestData.getString("availableDays", "NOT_FOUND") << "'" << std::endl;
        userProfile.available_days = requestData.getStringArray("availableDays", {"Monday", "Tuesday", "Thursday", "Friday"});
        std::cout << "📅 Parsed " << userProfile.available_days.size() << " available days: ";
        for (const auto& day : userProfile.available_days) {
            std::cout << day << " ";
        }
        std::cout << std::endl;
        
        // Parse muscleGroupPriorities object using new JSON object parsing
        auto priorities = requestData.getObject("muscleGroupPriorities", {});
        if (!priorities.empty()) {
            // Convert unordered_map to map
            for (const auto& pair : priorities) {
                userProfile.muscle_priorities[pair.first] = pair.second;
            }
        } else {
            // Default priorities
            userProfile.muscle_priorities = {
                {"Chest", "Medium"},
                {"Back", "Medium"},
                {"Shoulders", "Medium"},
                {"Arms", "Medium"},
                {"Legs", "Medium"},
                {"Core", "Medium"}
            };
        }
        
        // Parse availableEquipment array using new JSON array parsing
        userProfile.available_equipment = requestData.getStringArray("availableEquipment", {"Bodyweight", "Dumbbells", "Bench", "Barbell"});
        std::cout << "🏋️ Parsed " << userProfile.available_equipment.size() << " equipment items: ";
        for (const auto& eq : userProfile.available_equipment) {
            std::cout << eq << " ";
        }
        std::cout << std::endl;
        
        // Analyze user profile using REAL algorithms
        models::AnalysisResults analysis = services::AlgorithmAdapter::GenerateRealAnalysis(userProfile);
        
        std::cout << "📊 Analysis complete: BMI:" << analysis.bmi 
                  << " Cal:" << analysis.daily_calories 
                  << " Burn:" << analysis.weekly_calorie_burn << std::endl;
        
        // Generate workout plan using REAL algorithms
        models::WorkoutPlan workoutPlan = services::AlgorithmAdapter::GenerateRealWorkoutPlan(userProfile);
        
        std::cout << "💪 Workout plan generated: " << workoutPlan.name << std::endl;
        
        // Build response with REAL DATA from algorithms using direct JSON construction
        // to avoid SimpleJson string escaping issues
        std::stringstream responseStream;
        responseStream << "{";
        responseStream << "\"success\":true,";
        
        // Analysis results (REAL DATA)
        responseStream << "\"analysis\":{";
        responseStream << "\"bmi\":" << std::round(analysis.bmi * 10) / 10.0 << ",";
        responseStream << "\"bmiStatus\":\"" << analysis.bmi_status << "\",";
        responseStream << "\"dailyCalories\":" << analysis.daily_calories << ",";
        responseStream << "\"weeklyBurn\":" << analysis.weekly_calorie_burn << ",";
        responseStream << "\"bmr\":" << static_cast<int>(std::round(analysis.bmr));
        responseStream << "},";
        
        // Workout plan info (REAL DATA from algorithms)
        responseStream << "\"workoutPlan\":{";
        responseStream << "\"id\":\"" << workoutPlan.id << "\",";
        responseStream << "\"name\":\"" << workoutPlan.name << "\",";
        responseStream << "\"description\":\"" << workoutPlan.description << "\",";
        responseStream << "\"created_at\":\"" << workoutPlan.created_at << "\",";
        responseStream << "\"weeklyCalories\":" << workoutPlan.weekly_calories << ",";
        
        // Add real workout schedule directly as JSON object (not string)
        if (!workoutPlan.weekly_schedule_json.empty()) {
            responseStream << "\"weeklySchedule\":" << workoutPlan.weekly_schedule_json;
        } else {
            responseStream << "\"status\":\"Real algorithms executed - detailed schedule in development\"";
        }
        
        responseStream << "},";
        responseStream << "\"userData\":" << req.body;
        responseStream << "}";
        
        std::string finalResponse = responseStream.str();
        std::cout << "🚀 Sending direct JSON response..." << std::endl;
        
        response.setJson(finalResponse);
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
        
        // Parse arrays using new JSON parsing capabilities
        userProfile.available_days = requestData.getStringArray("availableDays", {"Monday", "Tuesday", "Thursday", "Friday"});
        
        auto priorities = requestData.getObject("muscleGroupPriorities", {});
        if (!priorities.empty()) {
            // Convert unordered_map to map
            for (const auto& pair : priorities) {
                userProfile.muscle_priorities[pair.first] = pair.second;
            }
        } else {
            userProfile.muscle_priorities = {
                {"Chest", "Medium"},
                {"Back", "Medium"},
                {"Shoulders", "Medium"},
                {"Arms", "Medium"},
                {"Legs", "Medium"},
                {"Core", "Medium"}
            };
        }
        
        userProfile.available_equipment = requestData.getStringArray("availableEquipment", {"Bodyweight", "Dumbbells", "Bench", "Barbell"});
        
        // Generate workout plan using REAL algorithms
        models::WorkoutPlan workoutPlan = services::AlgorithmAdapter::GenerateRealWorkoutPlan(userProfile);
        
        // Build detailed response with REAL DATA
        SimpleJson responseJson;
        responseJson.setBool("success", true);
        
        SimpleJson planJson;
        planJson.setString("id", workoutPlan.id);
        planJson.setString("name", workoutPlan.name);
        planJson.setString("description", workoutPlan.description);
        planJson.setString("created_at", workoutPlan.created_at);
        planJson.setInt("weeklyCalories", workoutPlan.weekly_calories);
        
        // Add real workout schedule if available
        if (!workoutPlan.weekly_schedule_json.empty()) {
            planJson.setString("weeklySchedule", workoutPlan.weekly_schedule_json);
        } else {
            planJson.setString("status", "Real algorithms executed successfully");
        }
        
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
        
        // Build response with exercises array
        std::stringstream responseStream;
        responseStream << "{";
        responseStream << "\"success\":true,";
        responseStream << "\"count\":" << exercises.size() << ",";
        responseStream << "\"exercises\":[";
        
        bool first = true;
        for (const auto& exercise : exercises) {
            if (!first) responseStream << ",";
            first = false;
            
            responseStream << "{";
            responseStream << "\"id\":\"" << exercise.id << "\",";
            responseStream << "\"name\":\"" << exercise.name << "\",";
            responseStream << "\"muscleGroup\":\"" << exercise.muscle_group << "\",";
            responseStream << "\"equipment\":\"" << exercise.equipment << "\",";
            responseStream << "\"isCompound\":" << (exercise.is_compound ? "true" : "false") << ",";
            responseStream << "\"duration\":" << exercise.duration_minutes << ",";
            responseStream << "\"difficulty\":\"" << exercise.difficulty << "\"";
            if (!exercise.instructions.empty()) {
                responseStream << ",\"instructions\":\"" << exercise.instructions << "\"";
            }
            responseStream << "}";
        }
        
        responseStream << "]";
        responseStream << "}";
        
        response.setJson(responseStream.str());
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
        "\"Workout generation: IMPLEMENTED\","
        "\"Exercise filtering: IMPLEMENTED\","
        "\"JSON parsing: IMPLEMENTED\","
        "\"Edge case handling: IN PROGRESS\""
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