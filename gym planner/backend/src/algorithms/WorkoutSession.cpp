//Represents a single workout day session
//Stores day of week, list of exercise, estimated duration and calories burned, type such as strength training, cardio, mix.
// Functions: Adds exercises, calculates metrics, prints session plan
#include "Exercise.h"
#include "WorkoutSession.h"
#include <iostream>
#include <iomanip>
#include <unordered_set>

using namespace std;

//Calorie burn rates for different workout types which is based on studys
double getMET(SessionType type){
    switch(type) {
        case SessionType::CARDIO:
            return 8.0;
        case SessionType::MIXED:
            return 6.5;
        case SessionType::FULL_BODY:
            return 5.5;
        case SessionType::STRENGTH:
            return 5.0;
        default: return 5.0;
    }
}

WorkoutSession::WorkoutSession(const string& workoutDay, const vector<Exercise>& exs,
                               SessionType sessionType, double userWeight)
    : day(workoutDay), exercises(exs), type(sessionType), weight(userWeight) {
    name=getTypeString();
    calcStats();
}

//CalculateS total time and calories of workout
void WorkoutSession::calcStats() {
    duration=0;
    for(const Exercise& ex : exercises) {
        duration+=ex.estimatedDurationMinutes;
    }
    double hours=duration/60.0;
    double MET=getMET(type);
    calories=static_cast<int>(MET*weight*hours);
}

bool WorkoutSession::tooLong(int maxTime) const {
    return duration>maxTime;
}

//Gets all muscle groups trained in this session to keep track of
vector<string> WorkoutSession::getMuscles() const {
    unordered_set<string> muscles;
    for (const Exercise& ex : exercises) {
        for(const string& muscle : ex.muscleGroups) {
            muscles.insert(muscle);
        }
    }
    return vector<string>(muscles.begin(), muscles.end());
}
string WorkoutSession::getDay() const {
    return day;
}
string WorkoutSession::getSessionName() const {
    return name;
}

vector<Exercise> WorkoutSession::getExercises() const {
    return exercises;
}

SessionType WorkoutSession::getSessionType() const {
    return type;
}
int WorkoutSession::getDuration() const {
    return duration;
}
int WorkoutSession::getCaloriesBurned() const {
    return calories;
}
void WorkoutSession::setSessionName(const string& sessionName) {
    name=sessionName;
}

//gets the workoutsession type name
string WorkoutSession::getTypeString() const {
    switch(type) {
        case SessionType::STRENGTH:
            return "Strength";
        case SessionType::CARDIO:
            return "Cardio";
        case SessionType::MIXED:
            return "Mixed";
        case SessionType::FULL_BODY:
            return "Full Body";
        default: return "Unknown";
    }
}

// Print workout details
void WorkoutSession::show() const {
    cout<< "\n" << string(40, '-') << "\n";
    cout<<"Day: " << day << "\n";
    cout<<"Session: " << name<< "\n";
    cout<<"Type: " << getTypeString()<< "\n";
    cout<<"Duration: " << duration <<" minutes\n";
    cout<<"Calories: " << calories <<" kcal\n";
    cout<<"Exercises (" << exercises.size() << "):\n";
    for(const Exercise& ex : exercises) {
        cout << " - " << ex.name << " (" << ex.estimatedDurationMinutes << " min)\n";
        cout << "   Equipment: " << ex.equipment << "\n";
        cout << "   Muscles: ";
        for(size_t i=0; i<ex.muscleGroups.size(); ++i) {
            cout << ex.muscleGroups[i];
            if (i<ex.muscleGroups.size()-1) cout << ", ";
        }
        cout << "\n";
    }
    cout<< string(40, '-')<<"\n";
}

void WorkoutSession::showDetailed() const {
    show();
    cout << "Muscles Trained:\n";
    for(const string& muscle : getMuscles()) {
        cout <<" * "<< muscle << "\n";
    }
}