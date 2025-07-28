#include "WorkoutPlanner.h"
#include "User.h"
#include "helpers.h"
#include <iostream>
#include <unordered_set>
#include <map>

int main() {
    WorkoutPlanner planner;

    // Load exercise data
    if (!planner.loadData("exercise_database.json")) {
        std::cerr << "Failed to load data.\n";
        return 1;
    }

    // Create user
    string name = "Gator";
    int height = 160;
    int weight = 47;
    int age = 19;
    string gender = "Female";

    vector<string> days = {"Monday", "Wednesday", "Thursday", "Friday"};

    // Equipment categories
    unordered_set<string> equipment = {
        "Free Weights",
        "Machines"
    };

    // Muscle priorities
    map<string, Priority> priorities = {
        {"Chest", Priority::LOW},
        {"Back", Priority::HIGH},
        {"Shoulders", Priority::LOW},
        {"Arms", Priority::HIGH},
        {"Legs", Priority::HIGH},
        {"Glutes", Priority::MEDIUM},
        {"Core", Priority::HIGH},
        {"Cardio", Priority::HIGH}
    };

    Goal goal = Goal::STRENGTH_BUILD;

    User user(name, height, weight, age, gender, days, equipment, priorities, goal);

    planner.setUser(user);

    // Generate plan
    auto plan = planner.makePlan();
    planner.showPlan(plan);
    planner.showAnalysis();

    // Summary
    cout << "\n=== PLAN SUMMARY ===\n";
    for (const auto& session : plan) {
        cout << session.getDay() << ": " << session.getSessionName()
             << " (" << session.getDuration()<<" min, "
             << session.getExercises().size()<<" exercises)\n";
    }

    return 0;
}
