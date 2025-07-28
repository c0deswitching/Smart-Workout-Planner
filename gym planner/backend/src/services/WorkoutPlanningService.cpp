#include "WorkoutPlanningService.h"
#include <algorithm>
#include <random>
#include <cmath>
#include <chrono>
#include <sstream>

namespace services {

// ============================================================================
// WORKOUT PLANNING SERVICE IMPLEMENTATION
// ============================================================================

models::WorkoutPlan WorkoutPlanningService::GenerateWorkoutPlan(const models::UserProfile& user_profile) {
    // TODO: ALGORITHM IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    // 
    // This is the main entry point for workout plan generation.
    // Your teammate should implement the following algorithm:
    //
    // 1. Validate user input (available days, muscle priorities, equipment)
    // 2. Filter exercises by available equipment
    // 3. Handle edge cases (single day, no equipment, etc.)
    // 4. Generate weekly schedule with proper muscle group distribution
    // 5. Balance intensity and recovery time
    // 6. Generate daily workout structures
    // 7. Return complete workout plan
    
    models::WorkoutPlan plan;
    plan.id = "plan_" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());
    plan.name = "Custom Workout Plan";
    plan.description = "Generated based on your preferences and goals";
    plan.user_id = "user_placeholder";
    
    // Get available exercises based on user's equipment
    auto all_exercises = models::ExerciseDatabase::GetAllExercises();
    auto available_exercises = FilterByAvailableEquipment(all_exercises, user_profile.available_equipment);
    
    // PLACEHOLDER: Call algorithm implementation here
    plan.weekly_schedule = std::map<std::string, models::DayPlan>();
    
    // Generate analysis results
    plan.analysis = AnalysisService::AnalyzeUserProfile(user_profile);
    
    // Set creation timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    plan.created_at = std::to_string(time_t);
    
    return plan;
}

std::vector<models::DayPlan> WorkoutPlanningService::GenerateWeeklySchedule(
    const models::UserProfile& user_profile,
    const std::vector<models::Exercise>& available_exercises) {
    
    // TODO: ALGORITHM IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Algorithm should:
    // 1. Parse user's available days
    // 2. Determine optimal muscle group distribution
    // 3. Avoid consecutive training of same muscle groups
    // 4. Balance strength vs cardio based on priorities
    // 5. Handle edge cases (consecutive days, single day, etc.)
    
    std::vector<models::DayPlan> weekly_schedule;
    
    // PLACEHOLDER: Return empty schedule for now
    return weekly_schedule;
}

models::DayPlan WorkoutPlanningService::GenerateDayPlan(
    const std::string& day,
    const std::vector<std::string>& target_muscle_groups,
    const std::vector<models::Exercise>& available_exercises,
    int intensity,
    const std::string& workout_type) {
    
    // TODO: ALGORITHM IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Algorithm should:
    // 1. Select appropriate exercises for target muscle groups
    // 2. Determine sets/reps based on intensity level
    // 3. Calculate rest times between exercises
    // 4. Ensure workout duration is reasonable (< 1.5 hours)
    // 5. Add warm-up and cool-down if needed
    
    models::DayPlan day_plan;
    day_plan.day = day;
    day_plan.workout_type = workout_type;
    day_plan.estimated_duration_minutes = 0;
    
    // PLACEHOLDER: Return empty day plan for now
    return day_plan;
}

std::vector<models::ExerciseSet> WorkoutPlanningService::SelectExercisesForMuscleGroups(
    const std::vector<std::string>& muscle_groups,
    const std::vector<models::Exercise>& available_exercises,
    int intensity) {
    
    // TODO: ALGORITHM IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Algorithm should:
    // 1. Filter exercises by target muscle groups
    // 2. Prioritize compound exercises when appropriate
    // 3. Select variety of exercises (avoid repetition)
    // 4. Determine sets/reps based on workout goal (intensity mapping)
    // 5. Calculate appropriate rest times
    
    std::vector<models::ExerciseSet> exercise_sets;
    
    // PLACEHOLDER: Return empty exercise sets for now
    return exercise_sets;
}

// ============================================================================
// EDGE CASE HANDLERS - PLACEHOLDERS FOR TEAMMATE IMPLEMENTATION
// ============================================================================

std::vector<models::Exercise> WorkoutPlanningService::HandleSingleDayWorkout(
    const std::vector<models::Exercise>& available_exercises) {
    
    // TODO: EDGE CASE IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // When user selects only 1 training day:
    // 1. Use compound, full body exercises
    // 2. Prioritize efficiency - 5 exercises covering all major muscle groups
    // 3. Focus on compound movements that work multiple muscle groups
    
    std::vector<models::Exercise> single_day_exercises;
    
    // PLACEHOLDER: Filter for compound exercises
    return single_day_exercises;
}

std::vector<models::Exercise> WorkoutPlanningService::HandleNoMachinesBarbell(
    const std::vector<models::Exercise>& all_exercises) {
    
    // TODO: EDGE CASE IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // When user selects no machines/barbell:
    // 1. Filter database to avoid empty plans
    // 2. Use bodyweight and dumbbell variants
    // 3. Show warning if fewer than 5 exercises available
    
    std::vector<models::Exercise> no_machine_exercises;
    
    // PLACEHOLDER: Filter for bodyweight/dumbbell exercises
    return no_machine_exercises;
}

std::vector<models::DayPlan> WorkoutPlanningService::HandleConsecutiveDays(
    const std::vector<std::string>& consecutive_days,
    const std::vector<models::Exercise>& available_exercises) {
    
    // TODO: EDGE CASE IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // When user selects consecutive training days:
    // 1. Track last muscle trained
    // 2. Spread volume across days
    // 3. Alternate muscle groups to allow recovery
    // 4. Consider upper/lower or push/pull/legs split
    
    std::vector<models::DayPlan> consecutive_schedule;
    
    // PLACEHOLDER: Generate alternating muscle group schedule
    return consecutive_schedule;
}

std::vector<models::ExerciseSet> WorkoutPlanningService::HandleHighPriorityAllMuscles(
    const std::vector<models::Exercise>& available_exercises,
    int intensity) {
    
    // TODO: EDGE CASE IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // When user selects high priority for all muscle groups:
    // 1. Use compound exercises per session
    // 2. Train each group at least once per week
    // 3. Prioritize multi-muscle movements
    
    std::vector<models::ExerciseSet> high_priority_sets;
    
    // PLACEHOLDER: Prioritize compound movements
    return high_priority_sets;
}

std::vector<models::Exercise> WorkoutPlanningService::FilterByAvailableEquipment(
    const std::vector<models::Exercise>& exercises,
    const std::vector<std::string>& available_equipment) {
    
    // TODO: IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Filter exercises by available equipment:
    // 1. Parse exercise equipment requirements
    // 2. Check against user's available equipment
    // 3. Handle equipment combinations (e.g., "Barbell + Bench")
    // 4. Return only exercises user can perform
    
    std::vector<models::Exercise> filtered_exercises;
    
    // PLACEHOLDER: Simple equipment filtering
    for (const auto& exercise : exercises) {
        // Simple check - needs enhancement
        bool has_equipment = false;
        for (const auto& equipment : available_equipment) {
            if (exercise.equipment.find(equipment) != std::string::npos) {
                has_equipment = true;
                break;
            }
        }
        if (has_equipment) {
            filtered_exercises.push_back(exercise);
        }
    }
    
    return filtered_exercises;
}

bool WorkoutPlanningService::ValidateWorkoutSessionDuration(const models::DayPlan& day_plan, int max_minutes) {
    // TODO: IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Validate workout session duration:
    // 1. Calculate total time including rest periods
    // 2. Ensure session doesn't exceed max_minutes (default 90)
    // 3. Suggest modifications if too long
    
    return day_plan.estimated_duration_minutes <= max_minutes;
}

std::vector<models::DayPlan> WorkoutPlanningService::AvoidConsecutiveCardio(
    const std::vector<models::DayPlan>& week_plan) {
    
    // TODO: EDGE CASE IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Avoid two cardio sessions back to back:
    // 1. Tag sessions as cardio, strength, core
    // 2. Rearrange schedule to avoid consecutive cardio
    // 3. Maintain user's available days constraints
    
    std::vector<models::DayPlan> optimized_plan = week_plan;
    
    // PLACEHOLDER: Return unmodified plan for now
    return optimized_plan;
}

std::string WorkoutPlanningService::GenerateErrorMessage(const std::string& error_type) {
    // TODO: IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Generate appropriate error messages:
    // 1. "Couldn't generate a full plan with your selected filters..."
    // 2. Equipment insufficient warnings
    // 3. Schedule constraint conflicts
    
    if (error_type == "insufficient_exercises") {
        return "Couldn't generate a full plan with your selected filters. Please add more equipment options or adjust muscle group priorities.";
    } else if (error_type == "no_available_days") {
        return "Please select at least one available day for workout.";
    } else if (error_type == "conflicting_schedule") {
        return "Unable to generate optimal schedule with current constraints. Consider adding more available days.";
    }
    
    return "Unknown error occurred during workout plan generation.";
}

// ============================================================================
// PRIVATE HELPER METHODS - PLACEHOLDERS FOR TEAMMATE IMPLEMENTATION
// ============================================================================

std::vector<std::string> WorkoutPlanningService::DetermineMuscleGroupPriorities(
    const std::map<std::string, std::string>& muscle_priorities) {
    
    // TODO: HELPER IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Determine muscle group training order:
    // 1. Sort by priority level (High -> Medium -> Low)
    // 2. Skip unselected muscle groups
    // 3. Return ordered list for weekly distribution
    
    std::vector<std::string> ordered_priorities;
    
    // PLACEHOLDER: Simple ordering
    for (const auto& pair : muscle_priorities) {
        if (!pair.second.empty()) {
            ordered_priorities.push_back(pair.first);
        }
    }
    
    return ordered_priorities;
}

std::string WorkoutPlanningService::DetermineWorkoutType(
    const std::vector<std::string>& muscle_groups,
    int day_index) {
    
    // TODO: HELPER IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Determine workout type for the day:
    // 1. "strength" for resistance training
    // 2. "cardio" for cardiovascular focus
    // 3. "rest" for recovery days
    
    // PLACEHOLDER: Simple alternating pattern
    if (std::find(muscle_groups.begin(), muscle_groups.end(), "Cardio") != muscle_groups.end()) {
        return "cardio";
    }
    return "strength";
}

int WorkoutPlanningService::CalculateSessionDuration(const std::vector<models::ExerciseSet>& exercise_sets) {
    // TODO: HELPER IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Calculate total session duration:
    // 1. Exercise time (sets × reps × estimated time per rep)
    // 2. Rest time between sets
    // 3. Transition time between exercises
    // 4. Warm-up and cool-down time
    
    // PLACEHOLDER: Simple estimation
    return exercise_sets.size() * 15; // 15 minutes per exercise set
}

std::vector<models::Exercise> WorkoutPlanningService::ShuffleExercises(
    const std::vector<models::Exercise>& exercises) {
    
    // TODO: HELPER IMPLEMENTATION - PLACEHOLDER FOR TEAMMATE
    //
    // Shuffle exercises to avoid repetition:
    // 1. Randomize exercise selection
    // 2. Maintain muscle group balance
    // 3. Ensure variety across weekly schedule
    
    std::vector<models::Exercise> shuffled = exercises;
    
    // PLACEHOLDER: Simple random shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), g);
    
    return shuffled;
}

bool WorkoutPlanningService::HasSufficientExercises(
    const std::vector<models::Exercise>& exercises,
    int min_required) {
    
    // Check if we have enough exercises for a complete workout
    return exercises.size() >= min_required;
}

// ============================================================================
// ANALYSIS SERVICE IMPLEMENTATION
// ============================================================================

models::AnalysisResults AnalysisService::AnalyzeUserProfile(const models::UserProfile& profile) {
    models::AnalysisResults results;
    
    // Calculate BMI
    results.bmi = CalculateBMI(profile.height_cm, profile.weight_kg);
    results.bmi_status = GetBMIStatus(results.bmi);
    
    // Calculate BMR
    results.bmr = CalculateBMR(profile.weight_kg, profile.height_cm, profile.age, profile.gender);
    
    // Calculate daily calories and weekly burn
    int activity_level = profile.training_intensity; // Use intensity as activity multiplier
    results.daily_calories = CalculateDailyCalories(results.bmr, activity_level);
    results.weekly_calorie_burn = CalculateWeeklyCalorieBurn(
        results.daily_calories, 
        profile.available_days.size(), 
        profile.training_intensity
    );
    
    return results;
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