# Smart Workout Planner :house:

A C++ application that generates personalized workout plans based on user preferences, available equipment, and muscle group priorities.

## Features
- Equipment category selection system (Free Weights, Machines, Bodyweight Tools, etc...)
- Customizable muscle group priorities (High/Medium/Low for each muscle group)
- Intelligent session timing (45-90 minutes based on training goals)
- Recovery logic that prevents overtraining of muscle groups
- BMI calculation and calorie burn estimation
- Smart workout split generation (Chest Day, Back Day, etc.)

## Prerequisites
- C++ compiler with C++20 support
- nlohmann/json library (json.hpp file)
- Exercise database in JSON format

## Installation & Setup

### Step 1: Download Dependencies
Download `json.hpp` from https://github.com/nlohmann/json/releases/latest and place it in the `include/` directory.

### Step 2: Exercise Database
The exercise database (`exercise_database.json`) is already included in the repository with 100+ exercises covering all major muscle groups and equipment types.

