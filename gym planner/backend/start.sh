#!/bin/bash

# Smart Workout Planner - Server Start Script

echo "🏋️ Starting Smart Workout Planner Server..."

# Check if the executable exists
EXECUTABLE="./bin/smart_workout_planner"
if [ ! -f "$EXECUTABLE" ]; then
    echo "❌ Server executable not found! Please build first with: ./build.sh"
    echo "💡 Make sure you're in the build/ directory"
    exit 1
fi

echo "🚀 Launching server on http://localhost:8080"
echo "📡 API Endpoints available:"
echo "   GET  /api/health - Server health check"
echo "   GET  /api/users/status - Server status info"
echo "   POST /api/users/analyze - Fitness analysis"
echo ""
echo "💡 Press Ctrl+C to stop the server"
echo ""

# Start the server
$EXECUTABLE