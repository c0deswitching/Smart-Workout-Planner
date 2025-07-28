import { useState } from 'react'
import type { ChangeEvent } from 'react'
import { Button } from '@/components/ui/button'
import { Input } from '@/components/ui/input'
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card'
import { Slider } from '@/components/ui/slider'
import { Select } from '@/components/ui/select'
import axios from 'axios'

interface UserData {
  height: number
  weight: number
  age: number
  gender: string
  availableDays: string[]
  muscleGroupPriorities: { [key: string]: string }
  intensity: number
  availableEquipment: string[]
}

interface AnalysisResults {
  bmi: number
  bmiStatus: string
  dailyCalories: number
  weeklyBurn: number
  bmr: number
}

interface WorkoutPlan {
  id: string
  name: string
  description: string
  exercises: Exercise[]
}

interface Exercise {
  id: string
  name: string
  muscleGroup: string
  equipment: string
  sets?: number
  reps?: string
  restTime?: string
}

const MUSCLE_GROUPS = [
  'Chest', 'Back', 'Shoulders', 'Arms', 'Legs', 'Core', 'Cardio'
]

const DAYS_OF_WEEK = [
  'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday'
]

const EQUIPMENT_OPTIONS = [
  'Barbell', 'Dumbbells', 'Bench', 'Cable Machine', 'Pull-Up Bar', 
  'Treadmill', 'Stationary Bike', 'Resistance Bands', 'Exercise Ball',
  'Bodyweight', 'StairMaster', 'Rowing Machine', 'Jump Rope',
  'Leg Extension Machine', 'Lat Pulldown Machine', 'Shoulder Press Machine'
]

const EXERCISE_DATABASE: Exercise[] = [
  // Chest
  { id: 'chest-1', name: 'Bench Press', muscleGroup: 'Chest', equipment: 'Barbell + Bench' },
  { id: 'chest-2', name: 'Incline Dumbbell Press', muscleGroup: 'Chest', equipment: 'Dumbbells + Incline Bench' },
  { id: 'chest-3', name: 'Dumbbell Chest Fly', muscleGroup: 'Chest', equipment: 'Dumbbells + Flat Bench' },
  { id: 'chest-4', name: 'Push-Up', muscleGroup: 'Chest', equipment: 'Bodyweight' },
  { id: 'chest-5', name: 'Cable Chest Fly', muscleGroup: 'Chest', equipment: 'Cable Machine' },
  
  // Shoulders
  { id: 'shoulders-1', name: 'Dumbbell Shoulder Press', muscleGroup: 'Shoulders', equipment: 'Dumbbells' },
  { id: 'shoulders-2', name: 'Arnold Press', muscleGroup: 'Shoulders', equipment: 'Dumbbells' },
  { id: 'shoulders-3', name: 'Dumbbell Lateral Raise', muscleGroup: 'Shoulders', equipment: 'Dumbbells' },
  { id: 'shoulders-4', name: 'Reverse Dumbbell Fly', muscleGroup: 'Shoulders', equipment: 'Dumbbells' },
  { id: 'shoulders-5', name: 'Machine Shoulder Press', muscleGroup: 'Shoulders', equipment: 'Shoulder Press Machine' },
  { id: 'shoulders-6', name: 'Cable Lateral Raise', muscleGroup: 'Shoulders', equipment: 'Cable Machine' },
  
  // Back
  { id: 'back-1', name: 'Pull-Up / Chin-Up', muscleGroup: 'Back', equipment: 'Pull-Up Bar' },
  { id: 'back-2', name: 'Inverted Row', muscleGroup: 'Back', equipment: 'Bar or Smith Machine' },
  { id: 'back-3', name: 'Lat Pulldown', muscleGroup: 'Back', equipment: 'Lat Pulldown Machine' },
  { id: 'back-4', name: 'Dumbbell Row', muscleGroup: 'Back', equipment: 'Dumbbells' },
  { id: 'back-5', name: 'Barbell Row', muscleGroup: 'Back', equipment: 'Barbell + Bench' },
  { id: 'back-6', name: 'Seated Cable Row', muscleGroup: 'Back', equipment: 'Cable Row Machine' },
  { id: 'back-7', name: 'Assisted Pull-Up', muscleGroup: 'Back', equipment: 'Assisted Pull-Up Machine' },
  
  // Arms - Biceps
  { id: 'biceps-1', name: 'Dumbbell Curl', muscleGroup: 'Arms', equipment: 'Dumbbells' },
  { id: 'biceps-2', name: 'Hammer Curl', muscleGroup: 'Arms', equipment: 'Dumbbells' },
  { id: 'biceps-3', name: 'Barbell Curl', muscleGroup: 'Arms', equipment: 'Barbell' },
  { id: 'biceps-4', name: 'Cable Curl', muscleGroup: 'Arms', equipment: 'Cable Machine' },
  { id: 'biceps-5', name: 'Resistance Band Curl', muscleGroup: 'Arms', equipment: 'Resistance Bands' },
  
  // Arms - Triceps
  { id: 'triceps-1', name: 'Tricep Dips', muscleGroup: 'Arms', equipment: 'Bench / Dip Bars' },
  { id: 'triceps-2', name: 'Dumbbell Overhead Triceps Extension', muscleGroup: 'Arms', equipment: 'Dumbbells' },
  { id: 'triceps-3', name: 'Tricep Pushdown', muscleGroup: 'Arms', equipment: 'Cable Machine' },
  { id: 'triceps-4', name: 'Close-Grip Push-Up', muscleGroup: 'Arms', equipment: 'Bodyweight' },
  { id: 'triceps-5', name: 'Machine Triceps Extension', muscleGroup: 'Arms', equipment: 'Triceps Machine' },
  
  // Legs - Quads
  { id: 'quads-1', name: 'Bodyweight Squat', muscleGroup: 'Legs', equipment: 'Bodyweight' },
  { id: 'quads-2', name: 'Goblet Squat', muscleGroup: 'Legs', equipment: 'Dumbbell' },
  { id: 'quads-3', name: 'Bulgarian Split Squat', muscleGroup: 'Legs', equipment: 'Dumbbells / Bodyweight' },
  { id: 'quads-4', name: 'Barbell Back Squat', muscleGroup: 'Legs', equipment: 'Barbell + Rack' },
  { id: 'quads-5', name: 'Leg Extension', muscleGroup: 'Legs', equipment: 'Leg Extension Machine' },
  
  // Legs - Hamstrings
  { id: 'hamstrings-1', name: 'Romanian Deadlift', muscleGroup: 'Legs', equipment: 'Barbell / Dumbbells' },
  { id: 'hamstrings-2', name: 'Glute Bridge (Hamstring Focus)', muscleGroup: 'Legs', equipment: 'Bodyweight' },
  { id: 'hamstrings-3', name: 'Lying Leg Curl', muscleGroup: 'Legs', equipment: 'Lying Leg Curl Machine' },
  { id: 'hamstrings-4', name: 'Seated Leg Curl', muscleGroup: 'Legs', equipment: 'Seated Hamstring Curl Machine' },
  { id: 'hamstrings-5', name: 'Stability Ball Leg Curl', muscleGroup: 'Legs', equipment: 'Exercise Ball' },
  
  // Legs - Glutes
  { id: 'glutes-1', name: 'Glute Bridge', muscleGroup: 'Legs', equipment: 'Bodyweight / Dumbbells' },
  { id: 'glutes-2', name: 'Hip Thrust', muscleGroup: 'Legs', equipment: 'Barbell / Dumbbells + Bench' },
  { id: 'glutes-3', name: 'Dumbbell Romanian Deadlift', muscleGroup: 'Legs', equipment: 'Dumbbells' },
  { id: 'glutes-4', name: 'Cable Kickbacks', muscleGroup: 'Legs', equipment: 'Cable Machine' },
  { id: 'glutes-5', name: 'Step-Ups', muscleGroup: 'Legs', equipment: 'Box + Dumbbells / Bodyweight' },
  
  // Legs - Calves
  { id: 'calves-1', name: 'Standing Calf Raise', muscleGroup: 'Legs', equipment: 'Bodyweight / Smith Machine' },
  { id: 'calves-2', name: 'Seated Calf Raise', muscleGroup: 'Legs', equipment: 'Seated Calf Raise Machine' },
  { id: 'calves-3', name: 'Dumbbell Calf Raise', muscleGroup: 'Legs', equipment: 'Dumbbells' },
  
  // Core
  { id: 'core-1', name: 'Plank', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-2', name: 'Crunch', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-3', name: 'Hanging Leg Raise', muscleGroup: 'Core', equipment: 'Pull-Up Bar' },
  { id: 'core-4', name: 'Cable Crunch', muscleGroup: 'Core', equipment: 'Cable Machine' },
  { id: 'core-5', name: 'Russian Twists', muscleGroup: 'Core', equipment: 'Bodyweight / Dumbbell' },
  { id: 'core-6', name: 'Bicycle Crunch', muscleGroup: 'Core', equipment: 'Bodyweight' },
  
  // Cardio
  { id: 'cardio-1', name: 'Treadmill Run/Walk', muscleGroup: 'Cardio', equipment: 'Treadmill' },
  { id: 'cardio-2', name: 'Stationary Bike', muscleGroup: 'Cardio', equipment: 'Stationary Bike' },
  { id: 'cardio-3', name: 'StairMaster', muscleGroup: 'Cardio', equipment: 'StairMaster' },
  { id: 'cardio-4', name: 'Rowing Machine', muscleGroup: 'Cardio', equipment: 'Rower' },
  { id: 'cardio-5', name: 'Jump Rope', muscleGroup: 'Cardio', equipment: 'Jump Rope' },
  { id: 'cardio-6', name: 'HIIT Bodyweight Circuit', muscleGroup: 'Cardio', equipment: 'Bodyweight' },
]

export default function FitnessAnalyzer() {
  const [userData, setUserData] = useState<UserData>({
    height: 175,
    weight: 70,
    age: 20,
    gender: 'male',
    availableDays: ['Monday', 'Tuesday', 'Thursday', 'Friday'],
    muscleGroupPriorities: {
      'Chest': '',
      'Back': '', 
      'Shoulders': '',
      'Arms': '',
      'Legs': '',
      'Core': '',
      'Cardio': ''
    },
    intensity: 5,
    availableEquipment: ['Bodyweight', 'Dumbbells', 'Bench', 'Barbell']
  })

  const [results, setResults] = useState<AnalysisResults | null>(null)
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState<string>('')
  const [workoutPlan, setWorkoutPlan] = useState<WorkoutPlan | null>(null)
  const [equipmentSearchTerm, setEquipmentSearchTerm] = useState('')
  const [showEquipmentDropdown, setShowEquipmentDropdown] = useState(false)

  const analyzeData = async () => {
    setLoading(true)
    setError('')
    
    try {
      // Prepare data in the format expected by backend
      const requestData = {
        height: userData.height,
        weight: userData.weight,
        age: userData.age,
        gender: userData.gender,
        intensity: userData.intensity,
        availableDays: userData.availableDays,
        muscleGroupPriorities: userData.muscleGroupPriorities,
        availableEquipment: userData.availableEquipment
      }
      
      console.log('Sending request to backend:', requestData)
      
      const response = await axios.post('http://localhost:8080/api/users/analyze', requestData)
      console.log('Received response:', response.data)
      
      setResults(response.data.analysis)
      
      // Generate workout plan placeholder
      setWorkoutPlan({
        id: 'plan-1',
        name: 'Custom Workout Plan',
        description: 'Generated based on your preferences',
        exercises: []
      })
      
    } catch (err: any) {
      console.error('Analysis error:', err)
      if (err.response) {
        console.error('Response data:', err.response.data)
        console.error('Response status:', err.response.status)
        setError(`Server error: ${err.response.data.error || err.response.statusText}`)
      } else if (err.request) {
        console.error('Request failed:', err.request)
        setError('Cannot connect to backend server. Make sure the C++ server is running on port 8080.')
      } else {
        console.error('Error message:', err.message)
        setError(`Request error: ${err.message}`)
      }
    } finally {
      setLoading(false)
    }
  }

  const updateField = (field: keyof UserData, value: any) => {
    setUserData(prev => ({
      ...prev,
      [field]: value
    }))
  }

  const toggleDay = (day: string) => {
    const newDays = userData.availableDays.includes(day)
      ? userData.availableDays.filter(d => d !== day)
      : [...userData.availableDays, day]
    updateField('availableDays', newDays)
  }

  const setPriorityLevel = (muscleGroup: string, priority: string) => {
    updateField('muscleGroupPriorities', {
      ...userData.muscleGroupPriorities,
      [muscleGroup]: priority
    })
  }

  const setAllBalanced = () => {
    const balanced = Object.keys(userData.muscleGroupPriorities).reduce((acc, muscle) => {
      if (muscle !== 'Cardio') acc[muscle] = 'Medium'
      return acc
    }, {} as { [key: string]: string })
    updateField('muscleGroupPriorities', balanced)
  }

  const toggleEquipment = (equipment: string) => {
    const newEquipment = userData.availableEquipment.includes(equipment)
      ? userData.availableEquipment.filter(e => e !== equipment)
      : [...userData.availableEquipment, equipment]
    updateField('availableEquipment', newEquipment)
  }

  const filteredEquipment = EQUIPMENT_OPTIONS.filter(eq =>
    eq.toLowerCase().includes(equipmentSearchTerm.toLowerCase())
  )

  return (
    <div className="min-h-screen bg-slate-900 text-white">
      {/* Header */}
      <div className="border-b border-slate-700 px-6 py-4">
        <div className="flex items-center justify-between">
          <div className="flex items-center gap-3">
            <div className="w-10 h-10 bg-gradient-to-br from-blue-500 to-teal-500 rounded-lg flex items-center justify-center text-2xl">
              🏋️
            </div>
            <h1 className="text-xl font-semibold">Ctrl, Alt, Deadlift</h1>
          </div>
          <div className="flex items-center gap-4">
            <Select value="English" className="w-24">
              <option value="English">English</option>
              <option value="Chinese">中文</option>
            </Select>
            <button className="w-8 h-8 rounded-lg bg-slate-700 flex items-center justify-center hover:bg-slate-600">
              🌙
            </button>
          </div>
        </div>
      </div>

      <div className="max-w-7xl mx-auto p-6 space-y-6">
        {/* Main Content Grid */}
        <div className="grid lg:grid-cols-2 gap-6">
          {/* Personal Information Input */}
          <Card className="bg-slate-800 border-slate-700">
            <CardHeader>
              <CardTitle className="text-white">Personal Information Input</CardTitle>
            </CardHeader>
            <CardContent className="space-y-6">
              {/* Height, Weight, Age */}
              <div className="grid grid-cols-3 gap-4">
                <div>
                  <label className="text-sm font-medium text-slate-300 block mb-2">Height (cm)</label>
                  <Input
                    type="number"
                    value={userData.height}
                    onChange={(e: ChangeEvent<HTMLInputElement>) => updateField('height', parseInt(e.target.value) || 0)}
                    className="bg-slate-700 border-slate-600 text-white"
                    min="100"
                    max="250"
                  />
                </div>
                <div>
                  <label className="text-sm font-medium text-slate-300 block mb-2">Weight (kg)</label>
                  <Input
                    type="number"
                    value={userData.weight}
                    onChange={(e: ChangeEvent<HTMLInputElement>) => updateField('weight', parseInt(e.target.value) || 0)}
                    className="bg-slate-700 border-slate-600 text-white"
                    min="30"
                    max="200"
                  />
                </div>
                <div>
                  <label className="text-sm font-medium text-slate-300 block mb-2">Age</label>
                  <Input
                    type="number"
                    value={userData.age}
                    onChange={(e: ChangeEvent<HTMLInputElement>) => updateField('age', parseInt(e.target.value) || 0)}
                    className="bg-slate-700 border-slate-600 text-white"
                    min="16"
                    max="100"
                  />
                </div>
              </div>

              {/* Gender */}
              <div>
                <label className="text-sm font-medium text-slate-300 block mb-2">Gender</label>
                <Select 
                  value={userData.gender}
                  onChange={(e: ChangeEvent<HTMLSelectElement>) => updateField('gender', e.target.value)}
                >
                  <option value="male">Male</option>
                  <option value="female">Female</option>
                  <option value="prefer-not-to-say">Prefer not to say</option>
                </Select>
              </div>

              {/* Days Available */}
              <div>
                <label className="text-sm font-medium text-slate-300 block mb-3">Days available to workout</label>
                <div className="grid grid-cols-7 gap-2">
                  {DAYS_OF_WEEK.map((day) => (
                    <button
                      key={day}
                      onClick={() => toggleDay(day)}
                      className={`p-2 text-xs rounded border transition-colors ${
                        userData.availableDays.includes(day)
                          ? 'bg-blue-600 border-blue-500 text-white'
                          : 'bg-slate-700 border-slate-600 text-slate-300 hover:bg-slate-600'
                      }`}
                    >
                      {day.slice(0, 3)}
                    </button>
                  ))}
                </div>
              </div>

              {/* Training Intensity */}
              <div>
                <label className="text-sm font-medium text-slate-300 block mb-2">Training Intensity</label>
                <div className="px-2">
                  <Slider
                    value={[userData.intensity]}
                    onValueChange={(value) => updateField('intensity', value[0])}
                    max={10}
                    min={1}
                    step={1}
                    className="mb-2"
                  />
                  <div className="flex justify-between text-xs text-slate-400">
                    <span>Low</span>
                    <span className="bg-blue-600 px-2 py-1 rounded text-white">{userData.intensity}</span>
                    <span>High</span>
                  </div>
                </div>
              </div>

              {/* Available Equipment */}
              <div className="relative">
                <div className="flex items-center justify-between mb-2">
                  <label className="text-sm font-medium text-slate-300">Available Equipment</label>
                  <button
                    onClick={() => setShowEquipmentDropdown(!showEquipmentDropdown)}
                    className="text-sm text-blue-400 hover:text-blue-300"
                  >
                    Edit
                  </button>
                </div>
                
                {showEquipmentDropdown && (
                  <div className="absolute z-10 w-full bg-slate-700 border border-slate-600 rounded-md p-4 mt-2">
                    <Input
                      type="text"
                      placeholder="Search equipment..."
                      value={equipmentSearchTerm}
                      onChange={(e) => setEquipmentSearchTerm(e.target.value)}
                      className="mb-3 bg-slate-800 border-slate-600 text-white"
                    />
                    <div className="max-h-40 overflow-y-auto space-y-2">
                      {filteredEquipment.map((equipment) => (
                        <label key={equipment} className="flex items-center space-x-2 cursor-pointer">
                          <input
                            type="checkbox"
                            checked={userData.availableEquipment.includes(equipment)}
                            onChange={() => toggleEquipment(equipment)}
                            className="form-checkbox text-blue-600"
                          />
                          <span className="text-sm text-slate-200">{equipment}</span>
                        </label>
                      ))}
                    </div>
                  </div>
                )}
                
                <div className="text-xs text-slate-400">
                  Selected: {userData.availableEquipment.length} items
                </div>
              </div>

              {/* Analyze Button */}
              <Button 
                onClick={analyzeData} 
                disabled={loading} 
                className="w-full bg-gradient-to-r from-blue-600 to-teal-600 hover:from-blue-700 hover:to-teal-700 text-white font-medium py-3"
              >
                {loading ? 'Analyzing...' : 'Analyze'}
              </Button>

              {error && (
                <div className="p-3 bg-red-900/20 border border-red-500 rounded-md">
                  <p className="text-red-400 text-sm">{error}</p>
                </div>
              )}
            </CardContent>
          </Card>

          {/* Target Muscle Groups */}
          <Card className="bg-slate-800 border-slate-700">
            <CardHeader>
              <CardTitle className="text-white">Target muscle groups and their level of importance</CardTitle>
            </CardHeader>
            <CardContent className="space-y-4">
              <Button 
                onClick={setAllBalanced}
                variant="outline"
                className="w-full mb-4"
              >
                All Balanced
              </Button>
              
              {MUSCLE_GROUPS.map((muscle) => (
                <div key={muscle} className="flex items-center justify-between">
                  <span className="text-slate-200 font-medium w-20">{muscle}:</span>
                  <div className="flex gap-2">
                    {['High', 'Medium', 'Low'].map((priority) => (
                      <button
                        key={priority}
                        onClick={() => setPriorityLevel(muscle, priority)}
                        className={`px-3 py-1 text-xs rounded border transition-colors ${
                          userData.muscleGroupPriorities[muscle] === priority
                            ? 'bg-blue-600 border-blue-500 text-white'
                            : 'bg-slate-700 border-slate-600 text-slate-300 hover:bg-slate-600'
                        }`}
                      >
                        {priority}
                      </button>
                    ))}
                  </div>
                </div>
              ))}
            </CardContent>
          </Card>
        </div>

        {/* Analysis Results */}
        {results && (
          <Card className="bg-slate-800 border-slate-700">
            <CardHeader>
              <CardTitle className="text-white">Analysis Results</CardTitle>
            </CardHeader>
            <CardContent>
              <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                {/* BMI Index */}
                <div className="p-4 bg-slate-700 rounded-lg border-l-4 border-l-blue-500">
                  <div className="text-sm text-slate-400">BMI Index</div>
                  <div className="text-3xl font-bold text-white mt-1">{results.bmi}</div>
                  <div className="text-sm text-green-400 capitalize">{results.bmiStatus}</div>
                </div>

                {/* Daily Calorie Intake */}
                <div className="p-4 bg-slate-700 rounded-lg border-l-4 border-l-green-500">
                  <div className="text-sm text-slate-400">Daily Calorie Intake</div>
                  <div className="text-3xl font-bold text-white mt-1">{results.dailyCalories} kcal</div>
                  <div className="text-sm text-green-400">Based on Your Goal</div>
                </div>

                {/* Weekly Calorie Burn */}
                <div className="p-4 bg-slate-700 rounded-lg border-l-4 border-l-purple-500">
                  <div className="text-sm text-slate-400">Weekly Calorie Burn</div>
                  <div className="text-3xl font-bold text-white mt-1">{results.weeklyBurn} kcal</div>
                  <div className="text-sm text-green-400">Achievable Target</div>
                </div>
              </div>
            </CardContent>
          </Card>
        )}

        {/* Workout Planner */}
        <Card className="bg-slate-800 border-slate-700">
          <CardHeader>
            <div className="flex items-center justify-between">
              <CardTitle className="text-white">Workout Planner</CardTitle>
              <div className="flex gap-2">
                {!workoutPlan ? (
                  <Button className="bg-blue-600 hover:bg-blue-700">
                    Create Plan
                  </Button>
                ) : (
                  <>
                    <Button className="bg-blue-600 hover:bg-blue-700">
                      Create Plan
                    </Button>
                    <Button variant="outline">
                      Edit Plan
                    </Button>
                  </>
                )}
              </div>
            </div>
          </CardHeader>
          <CardContent>
            {!workoutPlan ? (
              <div className="text-center py-8 text-slate-400">
                <div className="text-4xl mb-2">💪</div>
                <p>Click "Create Plan" to generate your personalized workout plan</p>
              </div>
            ) : (
              <div className="space-y-4">
                <div className="text-slate-200">
                  <h3 className="font-semibold">{workoutPlan.name}</h3>
                  <p className="text-sm text-slate-400">{workoutPlan.description}</p>
                </div>
                <div className="text-center py-4 text-slate-400">
                  <p>Workout plan details will be displayed here after algorithm implementation</p>
                </div>
              </div>
            )}
          </CardContent>
        </Card>

        {/* Exercise Directory */}
        <Card className="bg-slate-800 border-slate-700">
          <CardHeader>
            <CardTitle className="text-white">Exercise Directory</CardTitle>
          </CardHeader>
          <CardContent>
            <div className="overflow-x-auto">
              <table className="w-full text-sm">
                <thead>
                  <tr className="border-b border-slate-600">
                    <th className="text-left py-2 text-slate-300">Muscle Group</th>
                    <th className="text-left py-2 text-slate-300">Exercise</th>
                    <th className="text-left py-2 text-slate-300">Equipment</th>
                  </tr>
                </thead>
                <tbody>
                  {EXERCISE_DATABASE.map((exercise) => (
                    <tr key={exercise.id} className="border-b border-slate-700 hover:bg-slate-700/50">
                      <td className="py-2 text-slate-300">{exercise.muscleGroup}</td>
                      <td className="py-2 text-white">{exercise.name}</td>
                      <td className="py-2 text-slate-400">{exercise.equipment}</td>
                    </tr>
                  ))}
                </tbody>
              </table>
            </div>
          </CardContent>
        </Card>
      </div>
    </div>
  )
}