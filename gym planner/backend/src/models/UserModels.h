#pragma once

#include <string>
#include <vector>
#include <map>

namespace models {

// Workout Goals Configuration
enum class WorkoutGoal {
    ENDURANCE = 1,        // 12-20 reps, 3-4 sets, no failure, 30-60sec rest
    LIGHT_HYPERTROPHY = 2,  // 12-15 reps, 3-4 sets, no failure, 60-90sec rest
    STANDARD_HYPERTROPHY = 3, // 8-12 reps, 3 sets, failure optional, 90-120sec rest
    STRENGTH_HYPERTROPHY = 4, // 6-10 reps, 2-3 sets, final set failure, 120-180sec rest
    STRENGTH = 5            // 4-6 reps, 2 sets, until failure, 3-4min rest
};

struct WorkoutParameters {
    int reps_min;
    int reps_max;
    int sets;
    bool train_to_failure;
    int rest_seconds_min;
    int rest_seconds_max;
    std::string description;
};

struct UserProfile {
    double height_cm;
    double weight_kg;
    int age;
    std::string gender;
    std::vector<std::string> available_days;
    std::map<std::string, std::string> muscle_priorities; // muscle group -> priority level (High/Medium/Low)
    int training_intensity; // 1-10 scale (maps to WorkoutGoal)
    std::vector<std::string> available_equipment;
    
    UserProfile() : height_cm(170.0), weight_kg(70.0), age(25), 
                   gender("male"), training_intensity(5) {}
};

struct AnalysisResults {
    double bmi;
    std::string bmi_status;
    int daily_calories;
    int weekly_calorie_burn;
    double bmr; // Basal Metabolic Rate
    
    AnalysisResults() : bmi(0.0), daily_calories(0), weekly_calorie_burn(0), bmr(0.0) {}
};

struct Exercise {
    std::string id;
    std::string name;
    std::string muscle_group;
    std::string equipment;
    bool is_compound;       // For edge case handling
    int duration_minutes;
    std::string difficulty;
    std::string instructions;
    
    Exercise() : is_compound(false), duration_minutes(0) {}
    
    Exercise(const std::string& _id, const std::string& _name, 
             const std::string& _muscle_group, const std::string& _equipment,
             bool _is_compound = false)
        : id(_id), name(_name), muscle_group(_muscle_group), 
          equipment(_equipment), is_compound(_is_compound), duration_minutes(0) {}
};

struct ExerciseSet {
    std::string exercise_id;
    int sets;
    std::string reps;       // e.g., "8-12", "15", "to failure"
    std::string rest_time;  // e.g., "90-120sec", "3-4min"
    std::string notes;      // Additional instructions
};

struct DayPlan {
    std::string day;
    std::string workout_type; // "strength", "cardio", "rest"
    std::vector<ExerciseSet> exercise_sets;
    int estimated_duration_minutes;
    std::string focus_muscle_groups; // e.g., "Chest + Triceps"
    
    DayPlan() : estimated_duration_minutes(0) {}
};

struct WorkoutPlan {
    std::string id;
    std::string name;
    std::string description;
    std::string user_id;
    std::map<std::string, DayPlan> weekly_schedule;
    AnalysisResults analysis;
    std::string created_at;
    int weekly_calories;
    std::string weekly_schedule_json; // JSON representation of the schedule
    
    WorkoutPlan() : weekly_calories(0) {}
};

// Exercise Database Structure
class ExerciseDatabase {
public:
    static std::vector<Exercise> GetAllExercises();
    static std::vector<Exercise> GetExercisesByMuscleGroup(const std::string& muscle_group);
    static std::vector<Exercise> GetExercisesByEquipment(const std::vector<std::string>& available_equipment);
    static std::vector<Exercise> GetCompoundExercises();
    static Exercise GetExerciseById(const std::string& id);
    
private:
    static std::vector<Exercise> InitializeDatabase();
};

// Workout Goal Configuration Helper
class WorkoutGoalHelper {
public:
    static WorkoutParameters GetParametersForIntensity(int intensity);
    static WorkoutGoal IntensityToGoal(int intensity);
    static std::string GetGoalDescription(WorkoutGoal goal);
};

}