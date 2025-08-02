#include "Exercise.h"
#include "WorkoutPlanner.h"
#include "User.h"
#include "WorkoutSession.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <unordered_set>

using json = nlohmann::json;

//Equipment categories which is broken down into main categories like free weights, support and benches, bodyweight tools, Cables and extensions, machines, cardio eqipment and others.
//The main categories in the UI could be a select all button with a drop down to select specific workouts.
unordered_map<string, vector<string>> equipmentMap = {
    {"Free Weights", {"Dumbbells", "Dumbbell", "Barbell", "Kettlebell", "EZ Bar", "EZ Curl Bar", "Trap Bar"}},
    {"Support & Benches", {"Bench", "Incline Bench", "Decline Bench", "Flat Bench"}},
    {"Bodyweight Tools", {"Pull-Up Bar", "Dip Station","Dip Bars", "Parallettes", "Suspension Trainer", "Bodyweight"}},
    {"Cables & Resistance", {"Cable Machine", "Cable","Resistance Bands", "Resistance Band"}},
    {"Machines", {"Machine", "Smith Machine", "Leg Press Machine", "Lat Pulldown Machine",
                  "Shoulder Press Machine", "Triceps Machine","Chest Press Machine",
                  "Pec Deck Machine", "Leg Extension Machine","Lying Leg Curl Machine",
                  "Seated Hamstring Curl Machine","Seated Calf Raise Machine",
                  "Hack Squat Machine", "T-Bar Machine", "Preacher Machine",
                  "Donkey Calf Raise Machine", "Assisted Pull-Up Machine"}},
    {"Cardio Equipment", {"Treadmill","Stationary Bike", "StairMaster", "Rower", "Jump Rope"}},
    {"Specialty Equipment", {"Exercise Ball", "Box", "Landmine", "Rack"}}
};

int WorkoutPlanner::getTime(const Exercise& ex, Goal goal) const {
    int time;

    //The set time it takes for each workout considering number of sets and rest time
    //Adds in a 1-2 min buffer time between each workout to account for setting up equipment and other factors
    switch (goal) {
        case Goal::ENDURANCE:
            time = 9;  // 45 min, 5 exercises
            break;
        case Goal::LIGHT_BUILD:
            time = 10; // 50 min, 5 exercises
            break;
        case Goal::MUSCLE_BUILD:
            time = 11; // 55 min,5 exercises
            break;
        case Goal::STRENGTH_BUILD:
            time = 13; // 65 min, 5 exercises
            break;
        case Goal::STRENGTH:
            time = 12; // 60 min,5 exercises
            break;
        default:
            time = 10;
            break;
    }

    return time;
}

WorkoutPlanner::WorkoutPlanner() : rng(random_device{}()) {}
bool WorkoutPlanner::loadData(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "File failed to open"<<filename<<endl;
        return false;
    }

    json j;
    file >> j;

    exercises.clear();
    for (const auto& item : j) {
        exercises.push_back(Exercise::from_json(item));
    }

    cout <<"Loaded" << exercises.size() << "exercises from file.\n";
    return true;
}

void WorkoutPlanner::setUser(const User& u) {
    user = u;
    lastTrained.clear();
    exerciseCount.clear();
}

//Lets the user expand equipment option so they could select which equipments they have avaliable.
unordered_set<string> WorkoutPlanner::expandEquipment(const unordered_set<string>& equipment)const {
    unordered_set<string> expanded;

    for (const string& item:equipment) {
        expanded.insert(item);

        if (equipmentMap.find(item)!=equipmentMap.end()){
            for (const string& specific : equipmentMap.at(item)) {
                expanded.insert(specific);
            }}
    }
    return expanded;
}

//determines if the workouts requires machines, dumbells, just bodyweight, etc..
vector<Exercise> WorkoutPlanner::filterEquipment(const vector<Exercise>&list)const {
    vector<Exercise> filtered;
    unordered_set<string> expanded = expandEquipment(user.equipment);

    for (const Exercise& ex : list) {
        bool hasEquip = false;

        if (ex.equipment == "Bodyweight") {
            hasEquip = true;
        } else {
            for (const string& available:expanded) {
                if (ex.equipment.find(available) !=string::npos ||
                    available.find(ex.equipment) !=string::npos) {
                    hasEquip =true;
                    break;}
            }
        }
        if (hasEquip) {
            filtered.push_back(ex);
        }
    }
    return filtered;
}
//Arms has sub categories of biceps and triceps which is stated specifically in the JSON file.
//Legs has sub categories of quads and hamstrings, while other muscle groups sticks to their name
vector<string> WorkoutPlanner::expandMuscles(const vector<string>& muscles) const {
    vector<string> expanded;

    for (const string& muscle : muscles) {
        if (muscle == "Arms") {
            expanded.push_back("Biceps");
            expanded.push_back("Triceps");
        } else if (muscle == "Legs") {
            expanded.push_back("Quads");
            expanded.push_back("Hamstrings");
        } else {
            expanded.push_back(muscle);
        }
    }
    return expanded;
}

vector<Exercise> WorkoutPlanner::filterMuscles(const vector<Exercise>& list, const vector<string>& targets) const {
    vector<Exercise> filtered;
    vector<string> expanded = expandMuscles(targets);
    for (const Exercise& ex : list) {
        if (ex.targetsAnyMuscle(expanded)) {
            filtered.push_back(ex);
        }
    }
    return filtered;
}

//some workouts like pushups target multiple muscle groups like chest, core and triceps. So commpound
//added just a user selects high priority for all muscle groups and only avalaible for 1 workout session in the week.
vector<Exercise> WorkoutPlanner::getCompounds() const {
    vector<Exercise> compounds;

    for (const Exercise& ex : exercises) {
        if (ex.isCompound) {
            compounds.push_back(ex);
        }
    }
    return compounds;
}

//Training legs for ex two times in a row is not ideal for muscle growth. Muscles need rest so this fuction will check the last muscle group trained to avoid having to train the group twice in a row.
vector<Exercise> WorkoutPlanner::avoidRecent(const vector<Exercise>& list, const string& day) const {

    vector<string> prevDays = getPrevDay(day);
    unordered_set<string> recent;
    for (const string& prevDay : prevDays) {
        if (lastTrained.find(prevDay)!=lastTrained.end()) {
            for (const string& muscle: lastTrained.at(prevDay)) {
                recent.insert(muscle);
            }
        }
    }

    vector<Exercise> filtered;
    for (const Exercise& ex : list) {
        bool conflicts = false;
        for (const string& muscle : ex.muscleGroups) {
            if (recent.count(muscle)) {
                conflicts = true;
                break;
            }
        }
        if (!conflicts) {
            filtered.push_back(ex);
        }
    }
    return filtered.empty() ? list : filtered;
}

vector<Exercise> WorkoutPlanner::limitRepeats(const vector<Exercise>& list) const {
    vector<Exercise> filtered;

    for (const Exercise& ex : list) {
        int count = 0;
        if (exerciseCount.find(ex.name) != exerciseCount.end()) {
            count = exerciseCount.at(ex.name);
        }

        if (count < 2) {
            filtered.push_back(ex);
        }
    }
    return filtered.empty() ? list : filtered;
}

//func. will list out session type based on which muscle group is being traned the most.
SessionType WorkoutPlanner::getType(const vector<Exercise>& list) const {
    int cardio = 0;
    int strength = 0;
    unordered_map<string, int> muscleCount;

    for (const Exercise& ex : list) {
        bool isCardio= false;
        for (const string& muscle :ex.muscleGroups) {
            if (muscle =="Cardio") {
                cardio++;
                isCardio=true;
            } else {
                muscleCount[muscle]++;
            }
        } if (!isCardio) strength++;
    }

    if (cardio>0 && strength>0) return SessionType::MIXED;
    if (cardio >strength) return SessionType::CARDIO;

    // Checks for full body vs strength training
    bool hasUpper=false;
    bool hasLower=false;

    //upper body muscle groups
    for (const auto& [muscle,count]:muscleCount) {
        if (muscle == "Chest" || muscle == "Back"|| muscle == "Shoulders" ||
            muscle == "Arms"||muscle=="Biceps" || muscle=="Triceps") {
            hasUpper = true;
        }

        //lower body muscle groups
        if (muscle=="Legs" || muscle == "Quads" ||muscle == "Hamstrings" ||
            muscle=="Glutes" ||muscle=="Calves") {
            hasLower = true;
        }
    }
    //if the workout has both upper and lower body workouts then it would be considered full body
    if (hasUpper && hasLower && muscleCount.size() >= 4) {
        return SessionType::FULL_BODY;
    }
    return SessionType::STRENGTH;
}

string WorkoutPlanner::getName(const vector<Exercise>& list)const{
    unordered_map<string, int>muscleCount;
    bool hasCardio=false;

    for (const Exercise& ex:list) {
        for (const string& muscle : ex.muscleGroups) {
            if (muscle == "Cardio") {
                hasCardio = true;
            }
            else {
                muscleCount[muscle]++;
            }}
    }

    if (hasCardio && muscleCount.size()>2) {
        return "Full Body + Cardio";
    }
    if (hasCardio) return "Cardio Day";

    //will map back to UI muscle groups
    unordered_map<string, int> uiCount;
    for (const auto& [muscle, count]:muscleCount) {
        if (muscle == "Biceps"||muscle=="Triceps") {
            uiCount["Arms"] += count;
        } else if (muscle =="Quads" || muscle =="Hamstrings") {
            uiCount["Legs"]+=count;
        }
        else {
            uiCount[muscle]+=count;
        }
    }

    // Finds dominant muscle group
    //A session might train more than one muscle group like arms and back but this function will determine main group like "BACK"
    string primary = "";
    int maxCount = 0;
    for (const auto&[muscle,count]:uiCount) {
        if (count>maxCount) {
            maxCount=count;
            primary=muscle;
        }}
    if (primary == "Chest")
        return "Chest Day";

    if (primary == "Back")
        return "Back Day";

    if (primary == "Shoulders")
        return "Shoulder Day";

    if (primary == "Arms")
        return "Arm Day";

    if (primary == "Legs")
        return "Leg Day";

    if (primary == "Glutes")
        return "Glute Day";

    if (primary == "Core")
        return "Core Day";

    return "Strength Training";
}


//Makes sure we have enough exercises for the workout
vector<Exercise> WorkoutPlanner::ensureMin(vector<Exercise> list, int min) const {
    if(list.size()>=min) return list;  //already have enough

    //Gets all exercises we can use
    vector<Exercise> available=filterEquipment(exercises);
    available=limitRepeats(available);
    //Tracks what we already picked
    unordered_set<string> selected;
    for(const Exercise& ex : list) {
        selected.insert(ex.name);
    }

    vector<Exercise> additional;
    for (const Exercise& ex : available) {
        if (selected.find(ex.name)==selected.end()) {
            additional.push_back(ex);
        }
    }
    //Randomizes the order
    shuffle(additional.begin(), additional.end(),rng);

    int needed=min-list.size();
    for(int i=0; i<needed && i<additional.size(); i++) {
        list.push_back(additional[i]);
    }

    return list;
}
//Keep workout within time limits which cap limit of 1hr 30min
vector<Exercise> WorkoutPlanner::limitTime(const vector<Exercise>& list, int minTime, int maxTime) const {
    vector<Exercise> result=list;

    int total=0;
    for(const Exercise& ex : result) {
        total += ex.estimatedDurationMinutes+2;  //adds rest time
    }

    if(total<minTime) {
        // Need to add more exercises
        vector<Exercise> available=filterEquipment(exercises);
        available=limitRepeats(available);

        unordered_set<string> selected;
        for (const Exercise& ex:result) {
            selected.insert(ex.name);
        }
        vector<Exercise> additional;
        for (const Exercise& ex:available) {
            if(selected.find(ex.name)==selected.end()) {
                additional.push_back(ex);
            }
        }
        shuffle(additional.begin(),additional.end(), rng);

        for(const Exercise& ex:additional) {
            if (total>=minTime) break;
            result.push_back(ex);
            total+=ex.estimatedDurationMinutes+2;
        }
    }

    //Remove exercises if too long
    while(total>maxTime && result.size()>5) {
        total-=(result.back().estimatedDurationMinutes+2);
        result.pop_back();
    }
    return result;
}

bool WorkoutPlanner::hasCardioBack(const vector<WorkoutSession>& plan, const string& day) const {
    vector<string> prevDays=getPrevDay(day);

    for(const string& prevDay : prevDays) {
        for (const WorkoutSession& session : plan) {
            if(session.getDay()==prevDay &&
                (session.getSessionType()==SessionType::CARDIO ||
                 session.getSessionType()==SessionType::MIXED)) {
                return true;
                 }
        }}
    return false;
}

//Checks if previous day was a lower body workout as muscles need atleast 48-72hrs of rest before traning it agaib
bool WorkoutPlanner::hasLowerBack(const vector<WorkoutSession>& plan, const string& day) const {
    vector<string> prevDays=getPrevDay(day);

    //checks if yesterday was leg day, glute, hanstring etc.. and if yes, avoids lower body for today
    //if yesterday wasn't leg day then leg day is okay to put on that day of the week
    vector<string> lowerSessions={
        "Glutes & Hamstrings", "Quads & Glutes", "Quads & Calves",
        "Glute Focus", "Quad Focus", "Hamstring Focus", "Leg Day"
    };
    for (const string& prevDay : prevDays) {
        for(const WorkoutSession& session : plan) {
            if(session.getDay()==prevDay) {
                string sessionName=session.getSessionName();
                for(const string& lowerName : lowerSessions) {
                    if (sessionName.find(lowerName)!=string::npos) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

vector<string> WorkoutPlanner::getPrevDay(const string& day) const {
    vector<string> days={"Monday","Tuesday", "Wednesday", "Thursday", "Friday", "Saturday","Sunday"};
    vector<string> prev;
    auto it=find(days.begin(), days.end(), day);
    if(it!=days.end() && it!=days.begin()) {
        prev.push_back(*(it-1));
    }

    return prev;
}

int WorkoutPlanner::getDayNum(const string& day) const {
    vector<string> days={"Monday", "Tuesday", "Wednesday","Thursday", "Friday", "Saturday", "Sunday"};
    auto it=find(days.begin(), days.end(),day);
    return it!=days.end() ? distance(days.begin(),it) :-1;
}

// Main algorithm to create weekly workout plan
vector<WorkoutSession> WorkoutPlanner::makePlan() {
    vector<WorkoutSession> plan;
    exerciseCount.clear();

    if(exercises.empty()) {
        cout << "Error: No exercises loaded.\n";
        return plan;
    }
    
    // Handle single day case
    if(user.hasOneDay()) {
        vector<Exercise> fullBody=makeDay();
        if(!fullBody.empty()) {
            string sessionName=getName(fullBody);
            WorkoutSession session(user.workoutDays[0], fullBody,SessionType::FULL_BODY);
            session.setSessionName(sessionName);
            plan.push_back(session);
        }
        return plan;
    }

    // Edge case to address if there are little exercises available based on equipment
    vector<Exercise> available=filterEquipment(exercises);

    if (available.size()<5) {
        cout << "Warning: Few exercises available with current equipment.\n";
    }
    
    // Get muscle priorities
    vector<string> high=user.getHighMuscles();
    vector<string> medium=user.getMediumMuscles();
    vector<string> low=user.getLowMuscles();
    vector<string> allMuscles;
    allMuscles.insert(allMuscles.end(),high.begin(),high.end());
    allMuscles.insert(allMuscles.end(),medium.begin(),medium.end());
    allMuscles.insert(allMuscles.end(),low.begin(), low.end());

    // FIX: If no muscle priorities are set, use default muscle groups
    if (allMuscles.empty()) {
        allMuscles = {"Chest", "Back", "Shoulders", "Arms", "Legs", "Core"};
        cout << "No muscle priorities set, using default rotation.\n";
    }
    
    // FIX: Ensure high priority list is not empty to avoid division by zero
    if (high.empty()) {
        high = allMuscles; // Use all muscles as high priority if none specified
    }

    for(int dayIdx=0;dayIdx<user.workoutDays.size(); dayIdx++) {
        string day=user.workoutDays[dayIdx];
        vector<Exercise> dayExercises;

        string primaryMuscle="";
        if (dayIdx<allMuscles.size()) {
            primaryMuscle=allMuscles[dayIdx];
        } else {
            // FIX: Safe modulo operation - high is guaranteed to be non-empty now
            primaryMuscle=high[dayIdx%high.size()];
        }

        // Get exercises for main muscle group
        vector<Exercise> primaryExs=filterMuscles(available, {primaryMuscle});
        primaryExs=avoidRecent(primaryExs, day);
        primaryExs=limitRepeats(primaryExs);

        if(!primaryExs.empty()) {
            shuffle(primaryExs.begin(),primaryExs.end(), rng);
            int primaryCount=min(4, (int)primaryExs.size());
            for(int i=0; i<primaryCount; i++) {
                dayExercises.push_back(primaryExs[i]);
            }
        }

        // Fill in remaining spots with other exercises
        if(dayExercises.size()<5) {
            vector<string> secondary;
            for (const string& muscle : allMuscles) {
                if(muscle!=primaryMuscle) {
                    secondary.push_back(muscle);
                }
            }
            vector<Exercise> secondaryExs=filterMuscles(available, secondary);
            secondaryExs=limitRepeats(secondaryExs);
            if (!secondaryExs.empty()) {
                shuffle(secondaryExs.begin(), secondaryExs.end(), rng);
                int needed=5-dayExercises.size();
                for(int i=0; i<needed && i<secondaryExs.size(); i++) {
                    dayExercises.push_back(secondaryExs[i]);
                }
            }
        }

        dayExercises=ensureMin(dayExercises, 5);

        // Set exercise times based on training goal
        for(Exercise& ex : dayExercises) {
            ex.estimatedDurationMinutes=getTime(ex, user.goal);
        }
        dayExercises=limitTime(dayExercises, 45, 90);

        if(!dayExercises.empty()) {
            string sessionName=primaryMuscle+" Day";
            SessionType sessionType=SessionType::STRENGTH;

            WorkoutSession session(day, dayExercises, sessionType, user.weight);
            session.setSessionName(sessionName);
            plan.push_back(session);

            lastTrained[day]=session.getMuscles();
            for(const Exercise& ex : dayExercises) {
                exerciseCount[ex.name]++;
            }
        }
    }
    return plan;
}
//Creates a  workout for people with only one day available, by chossing compound workouts
vector<Exercise> WorkoutPlanner::makeDay() {
    vector<Exercise> compounds=getCompounds();
    vector<Exercise> available=filterEquipment(compounds);
    if(available.empty()) {
        // Fallback - use any exercises
        vector<Exercise> all=filterEquipment(exercises);
        shuffle(all.begin(), all.end(), rng);
        vector<Exercise> selected;

        unordered_set<string> covered;
        for (const Exercise& ex : all) {
            if(selected.size()>=5) break;
            bool addedNew=false;
            for(const string& muscle : ex.muscleGroups) {
                if (covered.find(muscle)==covered.end()){
                    addedNew=true;
                    covered.insert(muscle);
                }
            }
            if(addedNew || selected.size()<3) {
                selected.push_back(ex);
            }
        }

        return selected;
    }

    shuffle(available.begin(), available.end(), rng);
    vector<Exercise> selected;

    for(int i=0; i<min(5, (int)available.size()); i++) {
        selected.push_back(available[i]);
    }

    for(Exercise& ex : selected) {
        ex.estimatedDurationMinutes=getTime(ex, user.goal);
    }
    return selected;
}

//Plan Output display text

void WorkoutPlanner::showPlan(const vector<WorkoutSession>& plan) const {
    cout << "\n" << string(50, '=') << "\n";
    cout << "           WEEKLY WORKOUT PLAN\n";
    cout << string(50, '=') << "\n";

    for(const WorkoutSession& session : plan) {
        session.show();
    }

    cout << "Weekly Summary:\n";
    cout << "Total Workouts: " << plan.size() << "\n";
    cout << "Total Calories Burned: " << getCalories(plan) << "\n";
    cout << string(50, '=') << "\n\n";
}

string priorityText(Priority p) {
    switch(p) {
        case Priority::HIGH: return "High";
        case Priority::MEDIUM: return "Medium";
        case Priority::LOW: return "Low";
        default: return "Unknown";
    }
}

void WorkoutPlanner::showAnalysis() const {
    cout << "\n" << string(40, '=') << "\n";
    cout << "         ANALYSIS RESULTS\n";
    cout << string(40, '=') << "\n";

    cout << "BMI: " << fixed << setprecision(1) << user.getBMI() << "\n";
    cout << "Daily Calories: " << user.getDailyCalories() << " calories\n\n";

    cout << "Goal: ";
    switch(user.goal) {
        case Goal::ENDURANCE:
            cout << "Endurance (High reps, 3-4 sets, 30-60sec rest)\n";
            break;
        case Goal::LIGHT_BUILD:
            cout << "Light Build (12-15 reps, 3-4 sets, 60-90sec rest)\n";
            break;
        case Goal::MUSCLE_BUILD:
            cout << "Muscle Build (8-12 reps, 3-4 sets, 90-120sec rest)\n";
            break;
        case Goal::STRENGTH_BUILD:
            cout << "Strength Build (6-10 reps, 2-3 sets, 120-180 sec rest)\n";
            break;
        case Goal::STRENGTH:
            cout << "Strength (4-6 reps, 2 sets, 3-4 min rest)\n";
            break;
        default:
            cout << "Unknown goal\n";
            break;
    }

    cout << "\nEquipment: ";
    for(const string& equip : user.equipment) {
        cout << equip << " ";
    }
    cout << "\nMuscle Priorities:\n";
    for(const auto& [muscle, level] : user.priorities) {
        cout << " - " << muscle << ": " << priorityText(level) << "\n";
    }

    cout << string(40, '=') <<"\n\n";
}

//gets total number of calories burned (estimate based on muscle group)
int WorkoutPlanner::getCalories(const vector<WorkoutSession>& plan) const {
    int total=0;
    for(const auto& session : plan) {
        total+=session.getCaloriesBurned();
    }
    return total;

}
