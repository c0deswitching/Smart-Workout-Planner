# Smart Workout Planner

A full-stack web application that helps users create scientifically optimized workout plans with automatic scheduling, muscle group balance, recovery time management, and equipment considerations.

## 🎯 Problem Statement

Many people work out for long periods without seeing optimal results due to poor workout planning, leading to overstrained muscles, imbalanced training, and wasted time.

## 💡 Solution

An intelligent workout planner that generates personalized, balanced weekly schedules based on user goals, availability, and equipment access.

## 🚀 Development Environment Setup

### Prerequisites

- **Backend**: C++ compiler with C++17 support, CMake 3.5+
- **Frontend**: Node.js 16+, npm

### 1. Clone and Setup

```bash
git clone https://github.com/c0deswitching/Smart-Workout-Planner.git
cd Smart-Workout-Planner
```

### 2. Backend Setup (C++ + Simple HTTP Server)

```bash
cd backend

# Build the project (no external dependencies required)
mkdir build && cd build
cmake ..
make

# Start the server
./bin/smart_workout_planner
```

The backend API will be available on `http://localhost:8080`

### 3. Frontend Setup (React + shadcn/ui)

```bash
cd frontend

# Install dependencies
npm install

# Start development server
npm run dev
```

The web interface will be available on `http://localhost:5173`

### 4. Quick Start (Both Servers)

```bash
# From project root
./start-demo.sh
```

## 🏗️ Architecture

### Tech Stack

- **Backend**: C++ 17 with Simple HTTP Server (zero external dependencies)
- **Frontend**: React 19 + TypeScript + Vite
- **UI Framework**: shadcn/ui + TailwindCSS
- **Data Sources**: Kaggle exercise datasets (60+ exercises)

### Project Structure

```
Smart-Workout-Planner/
├── backend/                    # C++ HTTP Server
│   ├── src/
│   │   ├── main.cpp           # Server entry point
│   │   ├── controllers/       # API endpoint handlers
│   │   ├── models/            # Data structures & exercise database
│   │   ├── services/          # Business logic & algorithms
│   │   └── utils/             # HTTP server & JSON utilities
│   └── CMakeLists.txt         # Build configuration
├── frontend/                   # React Application
│   ├── src/
│   │   ├── components/        # React components & shadcn/ui
│   │   ├── App.tsx           # Main application
│   │   └── main.tsx          # React entry point
│   └── package.json          # Dependencies
└── README.md
```

## ✨ Features

### User Input
- **Personal Information**: Height, weight, age, gender selection
- **Training Schedule**: Available days (Monday-Sunday) multi-select
- **Muscle Group Priorities**: High/Medium/Low priority settings for 7 muscle groups
- **Training Intensity**: 1-10 scale mapping to workout goals
- **Equipment Selection**: 16+ equipment types with search functionality

### Analysis Engine
- **BMI Calculation**: Standard BMI formula with status classification
- **Metabolic Analysis**: BMR calculation using Mifflin-St Jeor equation
- **Calorie Planning**: Daily intake and weekly burn estimations
- **Workout Goal Mapping**: Intensity-based rep/set/rest recommendations

### Algorithm Framework (Ready for Implementation)
- **Weekly Schedule Generation**: Muscle group distribution with recovery time
- **Daily Plan Creation**: Exercise selection based on priorities and equipment
- **Edge Case Handling**: 9 specialized handlers for optimal user experience
- **Equipment Filtering**: Smart filtering based on available equipment

### Exercise Database
- **60+ Exercises**: Categorized by muscle group and equipment requirements
- **Compound Movement Tags**: Efficient full-body exercise identification
- **Equipment Mapping**: Detailed equipment requirements for each exercise

## 🔧 API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/health` | Server health check |
| GET | `/api/users/status` | Server status and features |
| POST | `/api/users/analyze` | Analyze user profile and return metrics |
| POST | `/api/workouts/generate` | Generate detailed workout plan |
| GET | `/api/exercises` | Get exercise database |

## 🎨 UI Components

- **Modern Dark Theme**: Professional fitness app design
- **Interactive Controls**: Sliders, multi-select buttons, searchable dropdowns
- **Responsive Layout**: Desktop and mobile optimized
- **Real-time Feedback**: Instant analysis results and workout plan preview
- **Exercise Directory**: Comprehensive exercise database table

## 🧮 Algorithm Implementation Status

- ✅ **Analysis Algorithms**: BMI, BMR, calorie calculations implemented
- 🔲 **Workout Generation**: Algorithm framework ready, implementation pending
- 🔲 **Exercise Selection**: Equipment filtering and muscle group optimization
- 🔲 **Edge Case Handling**: 9 specialized algorithms for optimal user experience

### Edge Cases Covered
1. Single day workout optimization
2. No machines/barbell alternatives
3. Consecutive training day management
4. High priority all muscle groups
5. Equipment availability validation
6. Exercise variety and rotation
7. Session duration limits (90min)
8. Consecutive cardio avoidance
9. Insufficient exercise warnings

## 🚀 Development Status

The application is ready for algorithm implementation with:
- Complete frontend interface matching all requirements
- Full backend architecture with service framework
- Exercise database with 60+ categorized exercises
- Mock data responses for development testing
- Comprehensive edge case handling framework

## 📊 Data Models

### Workout Goals
- **Endurance**: 12-20 reps, 3-4 sets, 30-60sec rest
- **Light Hypertrophy**: 12-15 reps, 3-4 sets, 60-90sec rest  
- **Standard Hypertrophy**: 8-12 reps, 3 sets, 90-120sec rest
- **Strength-Hypertrophy**: 6-10 reps, 2-3 sets, 120-180sec rest
- **Strength**: 4-6 reps, 2 sets, 3-4min rest

### Exercise Categories
- **Chest**: 5 exercises (compound & isolation)
- **Shoulders**: 6 exercises (press & raise variations)
- **Back**: 7 exercises (pull & row movements)
- **Arms**: 10 exercises (biceps & triceps)
- **Legs**: 15 exercises (quads, hamstrings, glutes, calves)
- **Core**: 6 exercises (stability & strength)
- **Cardio**: 6 exercises (HIIT & steady state)

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/algorithm-implementation`)
3. Implement algorithms in the placeholder functions
4. Test with frontend interface
5. Submit a pull request

## 📄 License

This project is licensed under the MIT License.