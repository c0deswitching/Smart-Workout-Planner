#ifndef WORKOUTPLANNER_H
#define WORKOUTPLANNER_H

#include "Exercise.h"
#include "User.h"
#include "WorkoutSession.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <random>

using namespace std;

class WorkoutPlanner {
private:
    vector<Exercise> exercises;
    User user;
    unordered_map<string, vector<string>> lastTrained;
    mutable unordered_map<string, int> exerciseCount;
    mutable mt19937 rng;

    // Filtering
    vector<Exercise> filterEquipment(const vector<Exercise>& list) const;
    vector<Exercise> filterMuscles(const vector<Exercise>& list, const vector<string>& targets) const;
    vector<Exercise> getCompounds() const;

    vector<Exercise> avoidRecent(const vector<Exercise>& list, const string& day) const;
    vector<Exercise> limitRepeats(const vector<Exercise>& list) const;

    vector<Exercise> ensureMin(vector<Exercise> list, int min) const;
    vector<Exercise> limitTime(const vector<Exercise>& list, int minTime, int maxTime) const;
    //Session logic
    SessionType getType(const vector<Exercise>& list) const;
    string getName(const vector<Exercise>& list) const;
    vector<string> expandMuscles(const vector<string>& muscles) const;  // ADD THIS LINE
    bool hasCardioBack(const vector<WorkoutSession>& plan, const string& day) const;
    bool hasLowerBack(const vector<WorkoutSession>& plan, const string& day) const;
    vector<string> getPrevDay(const string& day) const;
    int getDayNum(const string& day) const;
    int getTime(const Exercise& ex, Goal goal) const;

    //Equipment expansion toggle option
    unordered_set<string> expandEquipment(const unordered_set<string>& equipment) const;

public:
    WorkoutPlanner();

    bool loadData(const string& filename);
    void setUser(const User& u);
    vector<WorkoutSession> makePlan();
    vector<Exercise> makeDay();
    void showPlan(const vector<WorkoutSession>& plan) const;
    void showAnalysis() const;
    int getCalories(const vector<WorkoutSession>& plan) const;
};

#endif