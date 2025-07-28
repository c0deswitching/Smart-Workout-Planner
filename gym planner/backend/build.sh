#!/bin/bash

# Smart Workout Planner - Backend Build Script

echo "🏋️ Building Smart Workout Planner Backend..."

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "📁 Creating build directory..."
    mkdir build
fi

cd build

echo "🔧 Running CMake configuration..."
cmake ..

echo "🛠️ Building the project..."
make

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo "🚀 To start the server, run: cd build/bin && ./smart_workout_planner"
    echo "📡 Server will be available at: http://localhost:8080"
    echo ""
    echo "💡 Or use the start script: cd build && ./start.sh"
else
    echo "❌ Build failed!"
    exit 1
fi