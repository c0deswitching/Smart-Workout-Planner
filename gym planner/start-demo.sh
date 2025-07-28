#!/bin/bash

# Smart Workout Planner - Full Stack Startup Script

echo "🏋️ Starting Smart Workout Planner Full Stack Demo..."
echo ""

# Check if we're in the right directory
if [ ! -d "backend" ] || [ ! -d "frontend" ]; then
    echo "❌ Please run this script from the gym planner root directory"
    echo "   (the directory containing both 'backend' and 'frontend' folders)"
    exit 1
fi

echo "🔧 Step 1: Building and starting C++ backend server..."
cd backend
if [ ! -f "build/bin/smart_workout_planner" ]; then
    echo "   Building backend..."
    ./build.sh
fi

echo "   Starting backend server on port 8080..."
cd build/bin
./smart_workout_planner &
BACKEND_PID=$!
cd ../../..

sleep 2

echo ""
echo "⚛️ Step 2: Starting React frontend server..."
cd frontend
npm run dev &
FRONTEND_PID=$!
cd ..

sleep 3

echo ""
echo "🚀 **************** DEMO READY ****************"
echo ""
echo "📱 Frontend (Web UI):  http://localhost:5173"
echo "🔗 Backend (API):      http://localhost:8080"
echo ""
echo "💡 Open your browser and visit: http://localhost:5173"
echo "   This is your Smart Workout Planner interface!"
echo ""
echo "🛑 To stop both servers, press Ctrl+C"
echo ""

# Wait for Ctrl+C
trap "echo; echo '🛑 Stopping servers...'; kill $BACKEND_PID $FRONTEND_PID 2>/dev/null; exit 0" INT

# Keep script running
wait