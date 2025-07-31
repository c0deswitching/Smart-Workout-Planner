//functions that store information
//vector of all exercises
#include "Exercise.h"
#include "helpers.h"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>

//Loads the exercises from JSON file
vector<Exercise> loadDatabase(const string& filename) {
    vector<Exercise> exercises;
    ifstream file(filename);

    if(!file.is_open()) {
        cerr << "Error: Could not open file: "<<filename<<endl;
        return exercises;
    }

    try {
        json j;
        file >> j;

        if (!j.is_array()) {
            cerr << "Error: Invalid JSON format."<<endl;
            return exercises;
        }

        for(const auto& item : j) {
            if(item.contains("exercise") && item.contains("muscle_groups") && item.contains("equipment")) {
                exercises.push_back(Exercise::from_json(item));
            } else {
                cerr << "Warning: Skipping invalid exercise."<<endl;
            }
        }

        cout << "Loaded "<<exercises.size() <<" exercises from "<< filename << endl;

    } catch(const json::exception& e) {
        cerr << "Error parsing file: " << e.what() << endl;
        return {};
    }

    return exercises;
}

//converts the string to lowercase for processing
string toLowerCase(const string& text) {
    string result=text;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string formatText(const unordered_map<string, string>& data) {
    stringstream ss;
    for (const auto& [key, value] : data) {
        ss << "- " << key << ": " << value << "\n";
    }
    return ss.str();
}

//Checks if user has required equipment for a workout
bool hasEquipment(const string& needed, const unordered_set<string>& owned) {
    if(needed=="Bodyweight") return true;  // everyone can do bodyweight

    string neededLower=toLowerCase(needed);
    for(const string& item : owned) {
        string itemLower=toLowerCase(item);
        if (neededLower.find(itemLower)!=string::npos ||
            itemLower.find(neededLower)!=string::npos) {
            return true;
        }
    } return false;
}

// Equipment categories mapping
unordered_map<string, vector<string>> getCategories() {
    return {
        //workout equipments
        {"Free Weights", {"Dumbbells", "Dumbbell", "Barbell", "Kettlebell", "EZ Bar", "EZ Curl Bar", "Trap Bar"}},
        {"Support & Benches", {"Bench", "Incline Bench", "Decline Bench", "Flat Bench"}},
        {"Bodyweight Tools", {"Pull-Up Bar", "Dip Station", "Dip Bars", "Parallettes", "Suspension Trainer", "Bodyweight"}},
        {"Cables & Resistance", {"Cable Machine", "Cable", "Resistance Bands", "Resistance Band"}},
        {"Machines", {"Machine", "Smith Machine", "Leg Press Machine", "Lat Pulldown Machine",
                                      "Shoulder Press Machine","Triceps Machine", "Chest Press Machine",
                                      "Pec Deck Machine", "Leg Extension Machine", "Lying Leg Curl Machine",
                                      "Seated Hamstring Curl Machine","Seated Calf Raise Machine",
                                      "Hack Squat Machine", "T-Bar Machine", "Preacher Machine",
                                      "Donkey Calf Raise Machine", "Assisted Pull-Up Machine"}},
        {"Cardio Equipment", {"Treadmill", "Stationary Bike", "StairMaster", "Rower", "Jump Rope"}},
        {"Specialty Equipment", {"Exercise Ball", "Box", "Landmine", "Rack"}}
    };
}

vector<string> getCategoryNames() {
    auto categories=getCategories();
    vector<string> names;
    for(const auto& [category, _] : categories) {
        names.push_back(category);
    }
    return names;
}

bool isCategory(const string& name) {
    auto categories=getCategories();
    return categories.find(name)!=categories.end();
}

vector<string> expandCategory(const string& category) {
    auto categories=getCategories();
    if(categories.find(category)!=categories.end()) {
        return categories[category];
    }
    return {category};  //returns original if not a category
}
//Validate the workout duration
bool checkDuration(const vector<Exercise>& exercises, int minTime, int maxTime) {
    int total=0;
    for(const Exercise& ex : exercises) {
        total += ex.estimatedDurationMinutes+2; // add rest time
    }
    return total>=minTime && total<=maxTime;
}

// Count exercises by muscle group
unordered_map<string, int> countMuscles(const vector<Exercise>& exercises) {
    unordered_map<string, int> count;
    for(const Exercise& ex : exercises) {
        for (const string& muscle : ex.muscleGroups) {
            count[muscle]++;
        }
    }
    return count;
}