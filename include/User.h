#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <map>
#include <iostream>
#include <iomanip>

using namespace std;

enum class Priority {
    LOW,
    MEDIUM,
    HIGH
};

enum class Goal {
    ENDURANCE,
    LIGHT_BUILD,
    MUSCLE_BUILD,
    STRENGTH_BUILD,
    STRENGTH
};

class User {
public:
    string name;

    int height; // in cm as its more universal measuremnt
    int weight; // in kg
    int age;
    string gender;
    vector<string> workoutDays;
    unordered_set<string> equipment;
    map<string, Priority> priorities;
    Goal goal;

    User();     //Constructors

    User(string n,int h, int w,int a,string g,vector<string> days,
         unordered_set<string> equip, map<string,Priority> priority,Goal userGoal);

    //Health stats
    double getBMI() const;
    int getDailyCalories() const;

    //Gets the muscles by priority
    vector<string>getHighMuscles() const;
    vector<string>getMediumMuscles() const;
    vector<string>getLowMuscles()const;
    bool hasOneDay()const;
    bool hasBackToDays() const;
    bool wantsAllHigh() const;

    void show() const;
};

#endif