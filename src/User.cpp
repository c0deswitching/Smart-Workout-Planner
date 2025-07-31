#include "User.h"
#include <algorithm>
#include <set>
#include <iostream>

//Default constructor of user information in case user doesnt type in anythign
User::User() : name(""), height(170), weight(70), age(25), gender("Male"),
               workoutDays({"Monday","Wednesday", "Friday"}),
               equipment({"Bodyweight"}),
               goal(Goal::LIGHT_BUILD) {

    //Set default priorities which are all medium (balanced)
    priorities = {
        {"Chest", Priority::MEDIUM},
        {"Back", Priority::MEDIUM},
        {"Shoulders", Priority::MEDIUM},
        {"Arms", Priority::MEDIUM},
        {"Legs", Priority::MEDIUM},
        {"Glutes", Priority::MEDIUM},
        {"Core", Priority::MEDIUM},
        {"Cardio", Priority::MEDIUM}
    };
}

//Main constructor
User::User(string n, int h, int w, int a, string g, vector<string> days,
           unordered_set<string> equip, map<string, Priority> priority, Goal userGoal)
    : name(move(n)), height(h), weight(w), age(a), gender(move(g)),
      workoutDays(move(days)), equipment(move(equip)),
      priorities(move(priority)), goal(userGoal) {}

double User::getBMI() const {
    double heightM = height / 100.0;
    return weight / (heightM * heightM);
}

int User::getDailyCalories() const {
    // Basic BMR calculation
    double bmr;
    if (gender == "Male") {
        bmr = 10 * weight + 6.25 * height - 5 * age + 5;
    } else {
        bmr = 10 * weight + 6.25 * height - 5 * age - 161;
    }

    return static_cast<int>(bmr * 1.55); // Moderate activity
}

vector<string> User::getHighMuscles() const {
    vector<string> result;
    for (const auto& [muscle, level] : priorities) {
        if (level == Priority::HIGH) result.push_back(muscle);
    }
    return result;
}

vector<string> User::getMediumMuscles() const {
    vector<string> result;
    for (const auto& [muscle, level] : priorities) {
        if (level == Priority::MEDIUM) result.push_back(muscle);
    }
    return result;
}

vector<string> User::getLowMuscles() const {
    vector<string> result;
    for (const auto& [muscle, level] : priorities) {
        if (level == Priority::LOW) result.push_back(muscle);
    }
    return result;
}

bool User::hasOneDay() const {
    return workoutDays.size() == 1;
}

bool User::hasBackToDays() const {
    if (workoutDays.size() < 2) return false;

    vector<string> dayOrder = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    set<int> indices;

    for (const string& day : workoutDays) {
        auto it = find(dayOrder.begin(),dayOrder.end(), day);
        if (it != dayOrder.end()) {
            indices.insert(distance(dayOrder.begin(), it));
        }
    }

    vector<int> ordered(indices.begin(), indices.end());
    for (size_t i = 1; i<ordered.size(); ++i) {
        if (ordered[i]- ordered[i - 1] == 1) return true;
    }

    return false;
}

bool User::wantsAllHigh() const {
    int total = 0, high = 0;
    for (const auto& [_, level] : priorities) {
        ++total;
        if (level == Priority::HIGH) ++high;
    }
    return high >= max(1, total * 80 / 100);
}

void User::show() const {
    cout << "\n=== User Info ===\n";
    cout << "Name: " << name << "\n";
    cout << "Age: " << age <<" | Gender: " << gender << "\n";
    cout << "Height: " << height<<" cm | Weight: " << weight << " kg\n";
    cout << "BMI: " << fixed<<setprecision(1) << getBMI() << "\n";
    cout << "Daily Calories: "<<getDailyCalories() << "\n\n";

    cout << "Workout Days: ";
    for (const string& day : workoutDays) cout << day << " ";
    cout << "\n\nGoal: ";
    switch (goal) {
        case Goal::ENDURANCE:cout<<"Endurance"; break;
        case Goal::LIGHT_BUILD: cout<<"Light Build"; break;
        case Goal::MUSCLE_BUILD: cout<< "Muscle Build"; break;
        case Goal::STRENGTH_BUILD:cout<<"Strength Build"; break;
        case Goal::STRENGTH: cout<<"Strength"; break;
    }
    cout << "\n\nMuscle Priorities:\n";
    for (const auto& [muscle, level] : priorities) {
        cout << " - " << muscle << ": ";
        switch (level) {
            case Priority::LOW: cout<<"Low"; break;
            case Priority::MEDIUM: cout<< "Medium"; break;
            case Priority::HIGH:cout<< "High"; break;
        }
        cout << "\n";
    }
    cout << "================\n\n";
}