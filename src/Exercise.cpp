//Represents a single exercise
//Stores name of Exercise, muscle group, equipment, sets, reps, calories burned (estimated), training goal, instruction
#include "Exercise.h"
#include <algorithm>

Exercise::Exercise(string n, vector<string> muscles, string equip, bool compound, int duration)
    : name(move(n)),muscleGroups(move(muscles)),equipment(move(equip)),
      isCompound(compound),estimatedDurationMinutes(duration) {}

void Exercise::display() const {
    cout << name << " | ";
    for(const auto& m:muscleGroups)cout<<m<< " ";
    cout << "| " << equipment << " | Compound: " << (isCompound ? "Yes" : "No")
         << " | Duration: "<<estimatedDurationMinutes<<"min\n";
}

//Creates exercise from JSON data which has over 100 workouts
Exercise Exercise::from_json(const json& j) {
    vector<string> muscles=j["muscle_groups"];
    string exerciseName=j["exercise"];
    string equipmentName=j["equipment"];
    bool compound=muscles.size()>=2;  // compound if targets multiple muscles
    int duration=5;

    Exercise ex(exerciseName, muscles, equipmentName, compound, duration);
    ex.equipmentCategory=j.value("equipmentCategory", "");
    return ex;
}

// Checks if exercise works with any of the target muscles
bool Exercise::targetsAnyMuscle(const vector<string>& targetMuscles) const {
    for(const string& muscle : muscleGroups) {
        if(find(targetMuscles.begin(), targetMuscles.end(), muscle)!=targetMuscles.end()) {
            return true;
        }
    }
    return false;
}

bool Exercise::requiresEquipment(const string& equip) const {
    return equipment.find(equip)!=string::npos;
}

json Exercise::to_json() const {
    return json{
                {"exercise", name},
                {"muscle_groups", muscleGroups},
                {"equipment", equipment},
                {"is_compound", isCompound},
                {"duration_minutes", estimatedDurationMinutes}
    };
}