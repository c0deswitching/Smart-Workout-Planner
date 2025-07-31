#ifndef WORKOUTSESSION_H
#define WORKOUTSESSION_H

#include "Exercise.h"
#include <vector>
#include <string>

using namespace std;

enum class SessionType {
    STRENGTH,
    CARDIO,
    MIXED,
    FULL_BODY
};

class WorkoutSession {
private:
    string day;
    string name;
    vector<Exercise> exercises;
    SessionType type;
    int duration;
    int calories;
    double weight;

    void calcStats();

public:
    WorkoutSession(const string& workoutDay,
                const vector<Exercise>& exs,
                   SessionType sessionType,
                   double userWeight=70.0);

    //Get info OF SESSION
    string getDay() const;
    string getSessionName() const;
    vector<Exercise> getExercises() const;
    SessionType getSessionType() const;
    int getDuration() const;
    int getCaloriesBurned() const;
    string getTypeString() const;
    vector<string> getMuscles() const;
    void setSessionName(const string& sessionName);

    // Checks workout
    bool tooLong(int maxTime) const;
    void show() const;
    void showDetailed() const;
};

#endif