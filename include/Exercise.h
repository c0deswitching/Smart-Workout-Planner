#ifndef EXERCISE_H
#define EXERCISE_H
#include <string>
#include <vector>
#include <iostream>
#include "json.hpp"

using namespace std;
using json=nlohmann::json;

class Exercise {
public:
    string name;
    vector<string> muscleGroups;
    string equipment;
    string equipmentCategory;

    //edge case in case a user selects only one avaliability day and high priority for all muscle groups.
    //system would select the most commpound exercises like squats and assign it to the schedule.
    bool isCompound;

    //estimated time the workout will take to complete, factors like set up time and so are included.
    int estimatedDurationMinutes;
    Exercise(string n, vector<string> muscles, string equip, bool compound = false, int duration = 5);
    void display() const;

    bool targetsAnyMuscle(const vector<string>& targetMuscles) const;

    //checks if the workout needs any equipment. Like pushups dont need any so it would return false.
    bool requiresEquipment(const string& equip) const;

    static Exercise from_json(const json& j);
    json to_json() const;
};

#endif
