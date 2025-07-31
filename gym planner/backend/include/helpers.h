#ifndef HELPERS_H
#define HELPERS_H

#include "Exercise.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;
using json = nlohmann::json;

vector<Exercise> loadDatabase(const string& filename);

string toLowerCase(const string& text);
string formatText(const unordered_map<string, string>& data);

//Equipment matching
bool hasEquipment(const string& needed, const unordered_set<string>& owned);
unordered_map<string, vector<string>> getCategories();
vector<string> getCategoryNames();
bool isCategory(const string& name);
vector<string> expandCategory(const string& category);

bool checkDuration(const vector<Exercise>& exercises, int minTime = 45, int maxTime = 90);
unordered_map<string, int> countMuscles(const vector<Exercise>& exercises);

#endif