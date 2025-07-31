#include "WorkoutPlanningService.h"
#include "AlgorithmAdapter.h"
#include <algorithm>
#include <cmath>
#include <chrono>

namespace services {

// ============================================================================
// DEPRECATED WORKOUT PLANNING SERVICE - REPLACED BY ALGORITHM ADAPTER
// ============================================================================

models::WorkoutPlan WorkoutPlanningService::GenerateWorkoutPlan(const models::UserProfile& user_profile) {
    // DEPRECATED: This method has been replaced by AlgorithmAdapter::GenerateRealWorkoutPlan()
    // Delegating to the real implementation to avoid breaking existing code
    return AlgorithmAdapter::GenerateRealWorkoutPlan(user_profile);
}

std::vector<models::Exercise> WorkoutPlanningService::FilterByAvailableEquipment(
    const std::vector<models::Exercise>& exercises,
    const std::vector<std::string>& available_equipment) {
    
    // Delegate to ExerciseDatabase which has the real implementation
    return models::ExerciseDatabase::GetExercisesByEquipment(available_equipment);
}

// ============================================================================
// DEPRECATED ANALYSIS SERVICE - REPLACED BY ALGORITHM ADAPTER
// ============================================================================

models::AnalysisResults AnalysisService::AnalyzeUserProfile(const models::UserProfile& profile) {
    // DEPRECATED: This method has been replaced by AlgorithmAdapter::GenerateRealAnalysis()
    // Delegating to the real implementation to avoid breaking existing code
    return AlgorithmAdapter::GenerateRealAnalysis(profile);
}

double AnalysisService::CalculateBMI(double height_cm, double weight_kg) {
    double height_m = height_cm / 100.0;
    return weight_kg / (height_m * height_m);
}

std::string AnalysisService::GetBMIStatus(double bmi) {
    if (bmi < 18.5) return "underweight";
    else if (bmi < 25.0) return "normal";
    else if (bmi < 30.0) return "overweight";
    else return "obese";
}

double AnalysisService::CalculateBMR(double weight_kg, double height_cm, int age, const std::string& gender) {
    // Mifflin-St Jeor Equation
    if (gender == "male") {
        return 10 * weight_kg + 6.25 * height_cm - 5 * age + 5;
    } else {
        return 10 * weight_kg + 6.25 * height_cm - 5 * age - 161;
    }
}

int AnalysisService::CalculateDailyCalories(double bmr, int activity_level) {
    // Activity multipliers based on training intensity
    double multiplier = 1.2; // Sedentary baseline
    
    if (activity_level >= 8) multiplier = 1.9;      // Very active
    else if (activity_level >= 6) multiplier = 1.725; // Very active
    else if (activity_level >= 4) multiplier = 1.55;  // Moderately active
    else if (activity_level >= 2) multiplier = 1.375; // Lightly active
    
    return static_cast<int>(bmr * multiplier);
}

int AnalysisService::CalculateWeeklyCalorieBurn(int daily_calories, int workout_days, int intensity) {
    // Estimate calories burned per workout based on intensity
    int calories_per_workout = 200 + (intensity * 50); // Base 200 + intensity scaling
    return workout_days * calories_per_workout;
}

}