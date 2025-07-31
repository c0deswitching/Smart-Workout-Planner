#include "AlgorithmAdapter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace services {

User AlgorithmAdapter::ConvertToAlgorithmUser(const models::UserProfile& webProfile) {
    // Convert equipment from vector to unordered_set
    std::unordered_set<std::string> equipmentSet;
    for (const auto& equipment : webProfile.available_equipment) {
        equipmentSet.insert(equipment);
    }
    
    // Convert muscle priorities from map<string,string> to map<string,Priority>
    std::map<std::string, Priority> algorithmPriorities;
    for (const auto& pair : webProfile.muscle_priorities) {
        if (!pair.second.empty()) { // Skip empty priorities like cardio
            algorithmPriorities[pair.first] = StringToPriority(pair.second);
        }
    }
    
    // Convert intensity to goal
    Goal userGoal = IntensityToGoal(webProfile.training_intensity);
    
    // Create algorithm User object
    User algorithmUser(
        "WebUser", // Default name
        static_cast<int>(webProfile.height_cm),
        static_cast<int>(webProfile.weight_kg),
        webProfile.age,
        webProfile.gender,
        webProfile.available_days,
        equipmentSet,
        algorithmPriorities,
        userGoal
    );
    
    std::cout << "🔄 Converted web profile to algorithm user" << std::endl;
    std::cout << "   Height: " << algorithmUser.height << "cm" << std::endl;
    std::cout << "   Weight: " << algorithmUser.weight << "kg" << std::endl;
    std::cout << "   Available days: " << webProfile.available_days.size() << std::endl;
    std::cout << "   Equipment count: " << equipmentSet.size() << std::endl;
    std::cout << "   Muscle priorities: " << algorithmPriorities.size() << std::endl;
    
    return algorithmUser;
}

json::SimpleJson AlgorithmAdapter::ConvertWorkoutSessionsToJson(const std::vector<WorkoutSession>& sessions) {
    json::SimpleJson weeklySchedule;
    
    for (const auto& session : sessions) {
        json::SimpleJson dayJson;
        dayJson.setString("type", session.getTypeString());
        dayJson.setString("name", session.getSessionName());
        dayJson.setInt("duration", session.getDuration());
        dayJson.setInt("calories", session.getCaloriesBurned());
        
        // Add exercises as simple comma-separated string for easier parsing
        std::string exercisesStr = "";
        auto exercises = session.getExercises();
        for (size_t i = 0; i < exercises.size(); ++i) {
            if (i > 0) exercisesStr += ", ";
            exercisesStr += exercises[i].name;
        }
        dayJson.setString("exercises", exercisesStr);
        
        // Add muscle groups as simple comma-separated string  
        std::string musclesStr = "";
        auto muscles = session.getMuscles();
        for (size_t i = 0; i < muscles.size(); ++i) {
            if (i > 0) musclesStr += ", ";
            musclesStr += muscles[i];
        }
        dayJson.setString("muscles", musclesStr);
        
        weeklySchedule.setObject(session.getDay(), dayJson);
    }
    
    return weeklySchedule;
}

json::SimpleJson AlgorithmAdapter::ConvertWorkoutSessionToJson(const WorkoutSession& session) {
    json::SimpleJson sessionJson;
    sessionJson.setString("day", session.getDay());
    sessionJson.setString("name", session.getSessionName());
    sessionJson.setString("type", session.getTypeString());
    sessionJson.setInt("duration", session.getDuration());
    sessionJson.setInt("calories", session.getCaloriesBurned());
    
    return sessionJson;
}

Priority AlgorithmAdapter::StringToPriority(const std::string& priorityStr) {
    if (priorityStr == "High") return Priority::HIGH;
    if (priorityStr == "Medium") return Priority::MEDIUM;
    if (priorityStr == "Low") return Priority::LOW;
    return Priority::MEDIUM; // Default
}

Goal AlgorithmAdapter::IntensityToGoal(int intensity) {
    // Map intensity 1-10 to Goal enum
    if (intensity <= 2) return Goal::ENDURANCE;
    if (intensity <= 4) return Goal::LIGHT_BUILD;
    if (intensity <= 6) return Goal::MUSCLE_BUILD;
    if (intensity <= 8) return Goal::STRENGTH_BUILD;
    return Goal::STRENGTH;
}

models::WorkoutPlan AlgorithmAdapter::GenerateRealWorkoutPlan(const models::UserProfile& webProfile) {
    try {
        std::cout << "🚀 Starting real workout plan generation..." << std::endl;
        
        // Initialize WorkoutPlanner
        WorkoutPlanner planner;
        
        // Load exercise database (try multiple paths from different working directories)
        bool loaded = false;
        std::vector<std::string> paths = {
            "../../data/exercise_database.json",      // From backend/build/bin/
            "../../../backend/data/exercise_database.json",  // Alternative from bin/
            "backend/data/exercise_database.json",    // From project root
            "data/exercise_database.json",            // From backend/
            "./backend/data/exercise_database.json",  // From project root with ./
            "../data/exercise_database.json",         // From build/
            "exercise_database.json"                  // Same directory
        };
        
        for (const auto& path : paths) {
            if (planner.loadData(path)) {
                std::cout << "✅ Loaded exercise database from: " << path << std::endl;
                loaded = true;
                break;
            }
        }
        
        if (!loaded) {
            std::cout << "⚠️  Failed to load exercise database from all paths, generating basic plan" << std::endl;
            // Return basic plan with some workout sessions instead of empty
            models::WorkoutPlan basicPlan;
            basicPlan.id = "basic-001";
            basicPlan.name = "Basic Workout Plan";
            basicPlan.description = "Simple workout plan (exercise database not loaded)";
            
            auto now = std::time(nullptr);
            auto tm = *std::localtime(&now);
            std::stringstream ss;
            ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
            basicPlan.created_at = ss.str();
            
            // Create a basic weekly schedule manually
            std::stringstream scheduleStream;
            scheduleStream << "{";
            bool firstDay = true;
            
            for (const auto& day : webProfile.available_days) {
                if (!firstDay) scheduleStream << ",";
                firstDay = false;
                
                scheduleStream << "\"" << day << "\":{";
                scheduleStream << "\"type\":\"strength\",";
                scheduleStream << "\"name\":\"Basic " << day << " Workout\",";
                scheduleStream << "\"duration\":45,";
                scheduleStream << "\"calories\":300,";
                scheduleStream << "\"exercises\":\"Push-ups, Squats, Planks\",";
                scheduleStream << "\"muscles\":\"Full Body\"";
                scheduleStream << "}";
            }
            scheduleStream << "}";
            
            basicPlan.weekly_schedule_json = scheduleStream.str();
            basicPlan.weekly_calories = webProfile.available_days.size() * 300;
            
            std::cout << "✅ Generated basic plan with " << webProfile.available_days.size() << " workout days" << std::endl;
            return basicPlan;
        }
        
        // Convert web profile to algorithm user
        User algorithmUser = ConvertToAlgorithmUser(webProfile);
        
        // Set user in planner
        planner.setUser(algorithmUser);
        
        // Generate workout plan using real algorithms
        std::vector<WorkoutSession> workoutSessions = planner.makePlan();
        
        std::cout << "✅ Algorithm executed, generated " << workoutSessions.size() << " workout sessions" << std::endl;
        
        // Check if algorithm generated empty sessions and provide fallback
        if (workoutSessions.empty()) {
            std::cout << "⚠️ Algorithm returned empty sessions, generating fallback plan" << std::endl;
            
            // Create fallback workout sessions
            models::WorkoutPlan fallbackPlan;
            fallbackPlan.id = "fallback-" + std::to_string(std::time(nullptr));
            fallbackPlan.name = "Fallback Workout Plan";
            fallbackPlan.description = "Algorithm-generated fallback plan";
            
            auto now = std::time(nullptr);
            auto tm = *std::localtime(&now);
            std::stringstream ss;
            ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
            fallbackPlan.created_at = ss.str();
            
            // Create fallback weekly schedule
            std::stringstream scheduleStream;
            scheduleStream << "{";
            bool firstDay = true;
            int totalCalories = 0;
            
            for (const auto& day : webProfile.available_days) {
                if (!firstDay) scheduleStream << ",";
                firstDay = false;
                
                int dayCalories = 250 + (webProfile.training_intensity * 30);
                totalCalories += dayCalories;
                
                scheduleStream << "\"" << day << "\":{";
                scheduleStream << "\"type\":\"strength\",";
                scheduleStream << "\"name\":\"" << day << " Training\",";
                scheduleStream << "\"duration\":" << (30 + webProfile.training_intensity * 5) << ",";
                scheduleStream << "\"calories\":" << dayCalories << ",";
                
                // Add exercises based on available equipment
                std::string exercises = "Bodyweight exercises";
                if (std::find(webProfile.available_equipment.begin(), webProfile.available_equipment.end(), "Dumbbells") != webProfile.available_equipment.end()) {
                    exercises = "Dumbbell exercises, Bodyweight movements";
                }
                if (std::find(webProfile.available_equipment.begin(), webProfile.available_equipment.end(), "Barbell") != webProfile.available_equipment.end()) {
                    exercises = "Barbell training, Dumbbell work, Compound movements";
                }
                
                scheduleStream << "\"exercises\":\"" << exercises << "\",";
                scheduleStream << "\"muscles\":\"Full Body Training\"";
                scheduleStream << "}";
            }
            scheduleStream << "}";
            
            fallbackPlan.weekly_schedule_json = scheduleStream.str();
            fallbackPlan.weekly_calories = totalCalories;
            
            std::cout << "✅ Generated fallback plan with " << webProfile.available_days.size() << " days, " << totalCalories << " total calories" << std::endl;
            return fallbackPlan;
        }
        
        // Convert back to web format
        models::WorkoutPlan webPlan;
        webPlan.id = "plan-" + std::to_string(std::time(nullptr));
        webPlan.name = "Smart Workout Plan";
        webPlan.description = "AI-generated personalized workout plan";
        
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        webPlan.created_at = ss.str();
        
        // Calculate total weekly calories
        int totalCalories = planner.getCalories(workoutSessions);
        webPlan.weekly_calories = totalCalories;
        
        // Convert the sessions to a proper JSON object manually to avoid SimpleJson issues
        std::stringstream scheduleStream;
        scheduleStream << "{";
        bool firstDay = true;
        
        for (const auto& session : workoutSessions) {
            if (!firstDay) scheduleStream << ",";
            firstDay = false;
            
            scheduleStream << "\"" << session.getDay() << "\":{";
            scheduleStream << "\"type\":\"" << session.getTypeString() << "\",";
            scheduleStream << "\"name\":\"" << session.getSessionName() << "\",";
            scheduleStream << "\"duration\":" << session.getDuration() << ",";
            scheduleStream << "\"calories\":" << session.getCaloriesBurned() << ",";
            
            // Add exercises as comma-separated string
            std::string exercisesStr = "";
            auto exercises = session.getExercises();
            for (size_t i = 0; i < exercises.size(); ++i) {
                if (i > 0) exercisesStr += ", ";
                exercisesStr += exercises[i].name;
            }
            scheduleStream << "\"exercises\":\"" << exercisesStr << "\",";
            
            // Add muscle groups as comma-separated string
            std::string musclesStr = "";
            auto muscles = session.getMuscles();
            for (size_t i = 0; i < muscles.size(); ++i) {
                if (i > 0) musclesStr += ", ";
                musclesStr += muscles[i];
            }
            scheduleStream << "\"muscles\":\"" << musclesStr << "\"";
            scheduleStream << "}";
        }
        scheduleStream << "}";
        
        webPlan.weekly_schedule_json = scheduleStream.str();
        
        std::cout << "💪 Real workout plan generated successfully!" << std::endl;
        std::cout << "   Total sessions: " << workoutSessions.size() << std::endl;
        std::cout << "   Weekly calories: " << totalCalories << std::endl;
        
        return webPlan;
        
    } catch (const std::exception& e) {
        std::cout << "💥 Error in real workout generation: " << e.what() << std::endl;
        
        // Return error plan
        models::WorkoutPlan errorPlan;
        errorPlan.id = "error-001";
        errorPlan.name = "Error - Using Mock Plan";
        errorPlan.description = std::string("Algorithm error: ") + e.what();
        
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        errorPlan.created_at = ss.str();
        
        return errorPlan;
    }
}

models::AnalysisResults AlgorithmAdapter::GenerateRealAnalysis(const models::UserProfile& webProfile) {
    try {
        // Convert to algorithm user to use real BMI/calorie calculations
        User algorithmUser = ConvertToAlgorithmUser(webProfile);
        
        models::AnalysisResults results;
        
        // Use algorithm's real calculations
        results.bmi = algorithmUser.getBMI();
        results.daily_calories = algorithmUser.getDailyCalories();
        
        // Calculate BMR using the algorithm's internal calculation
        // Since we can't access the BMR directly, estimate from daily calories
        results.bmr = results.daily_calories * 0.7; // Rough estimate
        
        // BMI status
        if (results.bmi < 18.5) results.bmi_status = "Underweight";
        else if (results.bmi < 25.0) results.bmi_status = "Normal weight";
        else if (results.bmi < 30.0) results.bmi_status = "Overweight";
        else results.bmi_status = "Obese";
        
        // Estimate weekly calorie burn based on intensity and available days
        int workoutDays = static_cast<int>(webProfile.available_days.size());
        int baseCaloriesPerWorkout = 200 + (webProfile.training_intensity * 50);
        results.weekly_calorie_burn = workoutDays * baseCaloriesPerWorkout;
        
        std::cout << "📊 Real analysis generated using algorithm calculations" << std::endl;
        std::cout << "   BMI: " << results.bmi << " (" << results.bmi_status << ")" << std::endl;
        std::cout << "   Daily calories: " << results.daily_calories << std::endl;
        std::cout << "   Weekly burn: " << results.weekly_calorie_burn << " (Days: " << workoutDays << ", Intensity: " << webProfile.training_intensity << ")" << std::endl;
        
        return results;
        
    } catch (const std::exception& e) {
        std::cout << "💥 Error in real analysis: " << e.what() << std::endl;
        
        // Fallback to simple calculations
        models::AnalysisResults results;
        double height_m = webProfile.height_cm / 100.0;
        results.bmi = webProfile.weight_kg / (height_m * height_m);
        results.daily_calories = 2000; // Default
        results.bmr = 1500; // Default
        results.weekly_calorie_burn = 1500; // Default
        results.bmi_status = (results.bmi < 25) ? "Normal weight" : "Overweight";
        
        return results;
    }
}

}