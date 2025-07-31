#ifndef ALGORITHM_ADAPTER_H
#define ALGORITHM_ADAPTER_H

#include "../models/UserModels.h"
#include "../../include/User.h"
#include "../../include/WorkoutPlanner.h"
#include "../../include/WorkoutSession.h"
#include "../utils/SimpleJson.h"
#include <vector>
#include <string>

namespace services {

class AlgorithmAdapter {
public:
    // Convert from web API format to algorithm format
    static User ConvertToAlgorithmUser(const models::UserProfile& webProfile);
    
    // Convert from algorithm format back to web API format
    static json::SimpleJson ConvertWorkoutSessionsToJson(const std::vector<WorkoutSession>& sessions);
    
    // Convert individual workout session to JSON
    static json::SimpleJson ConvertWorkoutSessionToJson(const WorkoutSession& session);
    
    // Convert priority string to enum
    static Priority StringToPriority(const std::string& priorityStr);
    
    // Convert intensity to Goal enum
    static Goal IntensityToGoal(int intensity);
    
    // Execute complete workout planning pipeline
    static models::WorkoutPlan GenerateRealWorkoutPlan(const models::UserProfile& webProfile);
    
    // Generate analysis using real algorithm calculations
    static models::AnalysisResults GenerateRealAnalysis(const models::UserProfile& webProfile);
};

}

#endif