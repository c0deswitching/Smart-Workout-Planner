#include "../models/UserModels.h"
#include <algorithm>
#include <unordered_set>

namespace models {

// Exercise Database Implementation
std::vector<Exercise> ExerciseDatabase::GetAllExercises() {
    return InitializeDatabase();
}

std::vector<Exercise> ExerciseDatabase::GetExercisesByMuscleGroup(const std::string& muscle_group) {
    auto all_exercises = GetAllExercises();
    std::vector<Exercise> filtered;
    
    for (const auto& exercise : all_exercises) {
        if (exercise.muscle_group == muscle_group) {
            filtered.push_back(exercise);
        }
    }
    
    return filtered;
}

std::vector<Exercise> ExerciseDatabase::GetExercisesByEquipment(const std::vector<std::string>& available_equipment) {
    auto all_exercises = GetAllExercises();
    std::vector<Exercise> filtered;
    std::unordered_set<std::string> equipment_set(available_equipment.begin(), available_equipment.end());
    
    for (const auto& exercise : all_exercises) {
        // Check if any of the required equipment is available
        bool has_equipment = false;
        
        // Simple equipment matching - could be enhanced with better parsing
        if (exercise.equipment.find("Bodyweight") != std::string::npos && 
            equipment_set.count("Bodyweight")) {
            has_equipment = true;
        } else if (exercise.equipment.find("Dumbbells") != std::string::npos && 
                   equipment_set.count("Dumbbells")) {
            has_equipment = true;
        } else if (exercise.equipment.find("Barbell") != std::string::npos && 
                   equipment_set.count("Barbell")) {
            has_equipment = true;
        } else if (exercise.equipment.find("Cable Machine") != std::string::npos && 
                   equipment_set.count("Cable Machine")) {
            has_equipment = true;
        }
        // Add more equipment checks as needed
        
        if (has_equipment) {
            filtered.push_back(exercise);
        }
    }
    
    return filtered;
}

std::vector<Exercise> ExerciseDatabase::GetCompoundExercises() {
    auto all_exercises = GetAllExercises();
    std::vector<Exercise> compound;
    
    for (const auto& exercise : all_exercises) {
        if (exercise.is_compound) {
            compound.push_back(exercise);
        }
    }
    
    return compound;
}

Exercise ExerciseDatabase::GetExerciseById(const std::string& id) {
    auto all_exercises = GetAllExercises();
    
    for (const auto& exercise : all_exercises) {
        if (exercise.id == id) {
            return exercise;
        }
    }
    
    return Exercise(); // Return empty exercise if not found
}

std::vector<Exercise> ExerciseDatabase::InitializeDatabase() {
    std::vector<Exercise> exercises;
    
    // Chest Exercises
    exercises.emplace_back("chest-1", "Bench Press", "Chest", "Barbell + Bench", true);
    exercises.emplace_back("chest-2", "Incline Dumbbell Press", "Chest", "Dumbbells + Incline Bench", true);
    exercises.emplace_back("chest-3", "Dumbbell Chest Fly", "Chest", "Dumbbells + Flat Bench", false);
    exercises.emplace_back("chest-4", "Push-Up", "Chest", "Bodyweight", true);
    exercises.emplace_back("chest-5", "Cable Chest Fly", "Chest", "Cable Machine", false);
    
    // Shoulder Exercises
    exercises.emplace_back("shoulders-1", "Dumbbell Shoulder Press", "Shoulders", "Dumbbells", true);
    exercises.emplace_back("shoulders-2", "Arnold Press", "Shoulders", "Dumbbells", false);
    exercises.emplace_back("shoulders-3", "Dumbbell Lateral Raise", "Shoulders", "Dumbbells", false);
    exercises.emplace_back("shoulders-4", "Reverse Dumbbell Fly", "Shoulders", "Dumbbells", false);
    exercises.emplace_back("shoulders-5", "Machine Shoulder Press", "Shoulders", "Shoulder Press Machine", true);
    exercises.emplace_back("shoulders-6", "Cable Lateral Raise", "Shoulders", "Cable Machine", false);
    
    // Back Exercises
    exercises.emplace_back("back-1", "Pull-Up / Chin-Up", "Back", "Pull-Up Bar", true);
    exercises.emplace_back("back-2", "Inverted Row", "Back", "Bar or Smith Machine", true);
    exercises.emplace_back("back-3", "Lat Pulldown", "Back", "Lat Pulldown Machine", true);
    exercises.emplace_back("back-4", "Dumbbell Row", "Back", "Dumbbells", true);
    exercises.emplace_back("back-5", "Barbell Row", "Back", "Barbell + Bench", true);
    exercises.emplace_back("back-6", "Seated Cable Row", "Back", "Cable Row Machine", true);
    exercises.emplace_back("back-7", "Assisted Pull-Up", "Back", "Assisted Pull-Up Machine", true);
    
    // Arms - Biceps
    exercises.emplace_back("biceps-1", "Dumbbell Curl", "Arms", "Dumbbells", false);
    exercises.emplace_back("biceps-2", "Hammer Curl", "Arms", "Dumbbells", false);
    exercises.emplace_back("biceps-3", "Barbell Curl", "Arms", "Barbell", false);
    exercises.emplace_back("biceps-4", "Cable Curl", "Arms", "Cable Machine", false);
    exercises.emplace_back("biceps-5", "Resistance Band Curl", "Arms", "Resistance Bands", false);
    
    // Arms - Triceps
    exercises.emplace_back("triceps-1", "Tricep Dips", "Arms", "Bench / Dip Bars", true);
    exercises.emplace_back("triceps-2", "Dumbbell Overhead Triceps Extension", "Arms", "Dumbbells", false);
    exercises.emplace_back("triceps-3", "Tricep Pushdown", "Arms", "Cable Machine", false);
    exercises.emplace_back("triceps-4", "Close-Grip Push-Up", "Arms", "Bodyweight", true);
    exercises.emplace_back("triceps-5", "Machine Triceps Extension", "Arms", "Triceps Machine", false);
    
    // Legs - Quads
    exercises.emplace_back("quads-1", "Bodyweight Squat", "Legs", "Bodyweight", true);
    exercises.emplace_back("quads-2", "Goblet Squat", "Legs", "Dumbbell", true);
    exercises.emplace_back("quads-3", "Bulgarian Split Squat", "Legs", "Dumbbells / Bodyweight", true);
    exercises.emplace_back("quads-4", "Barbell Back Squat", "Legs", "Barbell + Rack", true);
    exercises.emplace_back("quads-5", "Leg Extension", "Legs", "Leg Extension Machine", false);
    
    // Legs - Hamstrings
    exercises.emplace_back("hamstrings-1", "Romanian Deadlift", "Legs", "Barbell / Dumbbells", true);
    exercises.emplace_back("hamstrings-2", "Glute Bridge (Hamstring Focus)", "Legs", "Bodyweight", true);
    exercises.emplace_back("hamstrings-3", "Lying Leg Curl", "Legs", "Lying Leg Curl Machine", false);
    exercises.emplace_back("hamstrings-4", "Seated Leg Curl", "Legs", "Seated Hamstring Curl Machine", false);
    exercises.emplace_back("hamstrings-5", "Stability Ball Leg Curl", "Legs", "Exercise Ball", false);
    
    // Legs - Glutes
    exercises.emplace_back("glutes-1", "Glute Bridge", "Legs", "Bodyweight / Dumbbells", true);
    exercises.emplace_back("glutes-2", "Hip Thrust", "Legs", "Barbell / Dumbbells + Bench", true);
    exercises.emplace_back("glutes-3", "Dumbbell Romanian Deadlift", "Legs", "Dumbbells", true);
    exercises.emplace_back("glutes-4", "Cable Kickbacks", "Legs", "Cable Machine", false);
    exercises.emplace_back("glutes-5", "Step-Ups", "Legs", "Box + Dumbbells / Bodyweight", true);
    
    // Legs - Calves
    exercises.emplace_back("calves-1", "Standing Calf Raise", "Legs", "Bodyweight / Smith Machine", false);
    exercises.emplace_back("calves-2", "Seated Calf Raise", "Legs", "Seated Calf Raise Machine", false);
    exercises.emplace_back("calves-3", "Dumbbell Calf Raise", "Legs", "Dumbbells", false);
    
    // Core Exercises
    exercises.emplace_back("core-1", "Plank", "Core", "Bodyweight", true);
    exercises.emplace_back("core-2", "Crunch", "Core", "Bodyweight", false);
    exercises.emplace_back("core-3", "Hanging Leg Raise", "Core", "Pull-Up Bar", false);
    exercises.emplace_back("core-4", "Cable Crunch", "Core", "Cable Machine", false);
    exercises.emplace_back("core-5", "Russian Twists", "Core", "Bodyweight / Dumbbell", false);
    exercises.emplace_back("core-6", "Bicycle Crunch", "Core", "Bodyweight", false);
    
    // Cardio Exercises
    exercises.emplace_back("cardio-1", "Treadmill Run/Walk", "Cardio", "Treadmill", false);
    exercises.emplace_back("cardio-2", "Stationary Bike", "Cardio", "Stationary Bike", false);
    exercises.emplace_back("cardio-3", "StairMaster", "Cardio", "StairMaster", false);
    exercises.emplace_back("cardio-4", "Rowing Machine", "Cardio", "Rower", true);
    exercises.emplace_back("cardio-5", "Jump Rope", "Cardio", "Jump Rope", false);
    exercises.emplace_back("cardio-6", "HIIT Bodyweight Circuit", "Cardio", "Bodyweight", true);
    
    return exercises;
}

// Workout Goal Helper Implementation
WorkoutParameters WorkoutGoalHelper::GetParametersForIntensity(int intensity) {
    WorkoutGoal goal = IntensityToGoal(intensity);
    WorkoutParameters params;
    
    switch (goal) {
        case WorkoutGoal::ENDURANCE:
            params = {12, 20, 4, false, 30, 60, "High reps, endurance focused"};
            break;
        case WorkoutGoal::LIGHT_HYPERTROPHY:
            params = {12, 15, 4, false, 60, 90, "Light hypertrophy, muscle building"};
            break;
        case WorkoutGoal::STANDARD_HYPERTROPHY:
            params = {8, 12, 3, false, 90, 120, "Standard hypertrophy, muscle growth"};
            break;
        case WorkoutGoal::STRENGTH_HYPERTROPHY:
            params = {6, 10, 3, true, 120, 180, "Strength-hypertrophy hybrid"};
            break;
        case WorkoutGoal::STRENGTH:
            params = {4, 6, 2, true, 180, 240, "Pure strength focused"};
            break;
        default:
            params = {8, 12, 3, false, 90, 120, "Standard hypertrophy"};
    }
    
    return params;
}

WorkoutGoal WorkoutGoalHelper::IntensityToGoal(int intensity) {
    if (intensity <= 2) return WorkoutGoal::ENDURANCE;
    else if (intensity <= 4) return WorkoutGoal::LIGHT_HYPERTROPHY;
    else if (intensity <= 6) return WorkoutGoal::STANDARD_HYPERTROPHY;
    else if (intensity <= 8) return WorkoutGoal::STRENGTH_HYPERTROPHY;
    else return WorkoutGoal::STRENGTH;
}

std::string WorkoutGoalHelper::GetGoalDescription(WorkoutGoal goal) {
    switch (goal) {
        case WorkoutGoal::ENDURANCE:
            return "Endurance: High reps, 3-4 sets, not till failure, 30-60sec rest";
        case WorkoutGoal::LIGHT_HYPERTROPHY:
            return "Light Hypertrophy: 12-15 reps, 3-4 sets, no till failure, 60-90sec rest";
        case WorkoutGoal::STANDARD_HYPERTROPHY:
            return "Standard Hypertrophy: 8-12 reps, 3 sets, until failure optional, 90-120sec rest";
        case WorkoutGoal::STRENGTH_HYPERTROPHY:
            return "Strength-Hypertrophy: 6-10 reps, 2-3 sets, final set to failure, 120-180sec rest";
        case WorkoutGoal::STRENGTH:
            return "Strength: 4-6 reps, 2 sets, until failure, 3-4min rest";
        default:
            return "Standard Hypertrophy";
    }
}

}