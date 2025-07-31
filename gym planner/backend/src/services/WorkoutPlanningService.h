#pragma once

#include "../models/UserModels.h"
#include <vector>

namespace services {

// NOTE: This service has been DEPRECATED and replaced by AlgorithmAdapter
// All workout planning functionality is now handled by:
// - services::AlgorithmAdapter::GenerateRealWorkoutPlan()
// - services::AlgorithmAdapter::GenerateRealAnalysis()
//
// This class remains as a stub to avoid breaking compilation.
// All actual implementations have been moved to the algorithm layer.

class WorkoutPlanningService {
public:
    // DEPRECATED: Use AlgorithmAdapter::GenerateRealWorkoutPlan() instead
    static models::WorkoutPlan GenerateWorkoutPlan(const models::UserProfile& user_profile);
    
    // Utility method that delegates to ExerciseDatabase
    static std::vector<models::Exercise> FilterByAvailableEquipment(
        const std::vector<models::Exercise>& exercises,
        const std::vector<std::string>& available_equipment
    );
};

// DEPRECATED: Use AlgorithmAdapter::GenerateRealAnalysis() instead
class AnalysisService {
public:
    static models::AnalysisResults AnalyzeUserProfile(const models::UserProfile& profile);
    
private:
    static double CalculateBMI(double height_cm, double weight_kg);
    static std::string GetBMIStatus(double bmi);
    static double CalculateBMR(double weight_kg, double height_cm, int age, const std::string& gender);
    static int CalculateDailyCalories(double bmr, int activity_level);
    static int CalculateWeeklyCalorieBurn(int daily_calories, int workout_days, int intensity);
};

}