#pragma once

#include "../models/UserModels.h"
#include <vector>

namespace services {

class WorkoutPlanningService {
public:
    // Main algorithm entry point - PLACEHOLDER FOR TEAMMATE IMPLEMENTATION
    static models::WorkoutPlan GenerateWorkoutPlan(const models::UserProfile& user_profile);
    
    // Sub-algorithms - PLACEHOLDERS FOR TEAMMATE IMPLEMENTATION
    static std::vector<models::DayPlan> GenerateWeeklySchedule(
        const models::UserProfile& user_profile,
        const std::vector<models::Exercise>& available_exercises
    );
    
    static models::DayPlan GenerateDayPlan(
        const std::string& day,
        const std::vector<std::string>& target_muscle_groups,
        const std::vector<models::Exercise>& available_exercises,
        int intensity,
        const std::string& workout_type
    );
    
    static std::vector<models::ExerciseSet> SelectExercisesForMuscleGroups(
        const std::vector<std::string>& muscle_groups,
        const std::vector<models::Exercise>& available_exercises,
        int intensity
    );
    
    // Edge case handlers - PLACEHOLDERS FOR TEAMMATE IMPLEMENTATION
    static std::vector<models::Exercise> HandleSingleDayWorkout(
        const std::vector<models::Exercise>& available_exercises
    );
    
    static std::vector<models::Exercise> HandleNoMachinesBarbell(
        const std::vector<models::Exercise>& all_exercises
    );
    
    static std::vector<models::DayPlan> HandleConsecutiveDays(
        const std::vector<std::string>& consecutive_days,
        const std::vector<models::Exercise>& available_exercises
    );
    
    static std::vector<models::ExerciseSet> HandleHighPriorityAllMuscles(
        const std::vector<models::Exercise>& available_exercises,
        int intensity
    );
    
    static std::vector<models::Exercise> FilterByAvailableEquipment(
        const std::vector<models::Exercise>& exercises,
        const std::vector<std::string>& available_equipment
    );
    
    static bool ValidateWorkoutSessionDuration(const models::DayPlan& day_plan, int max_minutes = 90);
    
    static std::vector<models::DayPlan> AvoidConsecutiveCardio(
        const std::vector<models::DayPlan>& week_plan
    );
    
    static std::string GenerateErrorMessage(const std::string& error_type);

private:
    // Algorithm helpers - PLACEHOLDERS FOR TEAMMATE IMPLEMENTATION
    static std::vector<std::string> DetermineMuscleGroupPriorities(
        const std::map<std::string, std::string>& muscle_priorities
    );
    
    static std::string DetermineWorkoutType(
        const std::vector<std::string>& muscle_groups,
        int day_index
    );
    
    static int CalculateSessionDuration(const std::vector<models::ExerciseSet>& exercise_sets);
    
    static std::vector<models::Exercise> ShuffleExercises(
        const std::vector<models::Exercise>& exercises
    );
    
    static bool HasSufficientExercises(
        const std::vector<models::Exercise>& exercises,
        int min_required = 5
    );
};

// Analysis Service - BMI and Calorie calculations
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