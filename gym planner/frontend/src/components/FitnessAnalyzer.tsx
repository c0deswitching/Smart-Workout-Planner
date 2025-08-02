import { useState, useEffect } from 'react'
import type { ChangeEvent } from 'react'
import { Button } from '@/components/ui/button'
import { Input } from '@/components/ui/input'
import { Card, CardContent, CardHeader, CardTitle } from '@/components/ui/card'
import { Select } from '@/components/ui/select'
import { Search, Filter, Download, Calendar, Dumbbell, Target, TrendingUp, Clock, Flame, Check, X, ChevronDown, ChevronUp, Star, Heart, Zap, Play } from 'lucide-react'
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
  weeklySchedule?: { [day: string]: any }
  weeklyCalories?: number
  status?: string
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
  'Leg Extension Machine', 'Lat Pulldown Machine', 'Shoulder Press Machine',
  'Smith Machine', 'Kettlebell', 'EZ Bar', 'Trap Bar', 'T-Bar Machine',
  'Preacher Machine', 'Triceps Machine', 'Chest Press Machine', 'Pec Deck Machine',
  'Lying Leg Curl Machine', 'Seated Hamstring Curl Machine', 'Seated Calf Raise Machine',
  'Hack Squat Machine', 'Donkey Calf Raise Machine', 'Assisted Pull-Up Machine',
  'Landmine', 'Rack', 'Box', 'Incline Bench', 'Flat Bench', 'Decline Bench',
  'Dip Station', 'Dip Bars', 'Cable Row Machine', 'Rower'
]

const EXERCISE_DATABASE: Exercise[] = [
  // Chest
  { id: 'chest-1', name: 'Bench Press', muscleGroup: 'Chest', equipment: 'Barbell + Bench' },
  { id: 'chest-2', name: 'Incline Dumbbell Press', muscleGroup: 'Chest', equipment: 'Dumbbells + Incline Bench' },
  { id: 'chest-3', name: 'Dumbbell Chest Fly', muscleGroup: 'Chest', equipment: 'Dumbbells + Flat Bench' },
  { id: 'chest-4', name: 'Push-Up', muscleGroup: 'Chest', equipment: 'Bodyweight' },
  { id: 'chest-5', name: 'Cable Chest Fly', muscleGroup: 'Chest', equipment: 'Cable Machine' },
  { id: 'chest-6', name: 'Machine Chest Press', muscleGroup: 'Chest', equipment: 'Chest Press Machine' },
  { id: 'chest-7', name: 'Incline Push-Up', muscleGroup: 'Chest', equipment: 'Bodyweight' },
  { id: 'chest-8', name: 'Machine Chest Fly', muscleGroup: 'Chest', equipment: 'Pec Deck Machine' },
  { id: 'chest-9', name: 'Clap Push-Up', muscleGroup: 'Chest', equipment: 'Bodyweight' },

  // Shoulders
  { id: 'shoulders-1', name: 'Dumbbell Shoulder Press', muscleGroup: 'Shoulders', equipment: 'Dumbbells' },
  { id: 'shoulders-2', name: 'Arnold Press', muscleGroup: 'Shoulders', equipment: 'Dumbbells' },
  { id: 'shoulders-3', name: 'Dumbbell Lateral Raise', muscleGroup: 'Shoulders', equipment: 'Dumbbells' },
  { id: 'shoulders-4', name: 'Reverse Dumbbell Fly', muscleGroup: 'Shoulders', equipment: 'Dumbbells' },
  { id: 'shoulders-5', name: 'Machine Shoulder Press', muscleGroup: 'Shoulders', equipment: 'Shoulder Press Machine' },
  { id: 'shoulders-6', name: 'Cable Lateral Raise', muscleGroup: 'Shoulders', equipment: 'Cable Machine' },
  { id: 'shoulders-7', name: 'Overhead Press', muscleGroup: 'Shoulders', equipment: 'Barbell' },
  { id: 'shoulders-8', name: 'Landmine Press', muscleGroup: 'Shoulders', equipment: 'Landmine' },
  { id: 'shoulders-9', name: 'Cuban Press', muscleGroup: 'Shoulders', equipment: 'Dumbbells' },
  { id: 'shoulders-10', name: 'Reverse Cable Fly', muscleGroup: 'Shoulders', equipment: 'Cable Machine' },
  { id: 'shoulders-11', name: 'Wall Walk', muscleGroup: 'Shoulders', equipment: 'Bodyweight' },

  // Back
  { id: 'back-1', name: 'Pull-Up / Chin-Up', muscleGroup: 'Back', equipment: 'Pull-Up Bar' },
  { id: 'back-2', name: 'Inverted Row', muscleGroup: 'Back', equipment: 'Bar or Smith Machine' },
  { id: 'back-3', name: 'Lat Pulldown', muscleGroup: 'Back', equipment: 'Lat Pulldown Machine' },
  { id: 'back-4', name: 'Dumbbell Row', muscleGroup: 'Back', equipment: 'Dumbbells' },
  { id: 'back-5', name: 'Barbell Row', muscleGroup: 'Back', equipment: 'Barbell + Bench' },
  { id: 'back-6', name: 'Seated Cable Row', muscleGroup: 'Back', equipment: 'Cable Row Machine' },
  { id: 'back-7', name: 'Assisted Pull-Up', muscleGroup: 'Back', equipment: 'Assisted Pull-Up Machine' },
  { id: 'back-8', name: 'Deadlift', muscleGroup: 'Back', equipment: 'Barbell' },
  { id: 'back-9', name: 'Good Morning', muscleGroup: 'Back', equipment: 'Barbell' },
  { id: 'back-10', name: 'Pendlay Row', muscleGroup: 'Back', equipment: 'Barbell' },
  { id: 'back-11', name: 'Trap Bar Deadlift', muscleGroup: 'Back', equipment: 'Trap Bar' },
  { id: 'back-12', name: 'T-Bar Row', muscleGroup: 'Back', equipment: 'T-Bar Machine' },
  { id: 'back-13', name: 'Chin-Up', muscleGroup: 'Back', equipment: 'Pull-Up Bar' },
  { id: 'back-14', name: 'Scapular Pull-Up', muscleGroup: 'Back', equipment: 'Pull-Up Bar' },
  { id: 'back-15', name: 'Renegade Row', muscleGroup: 'Back', equipment: 'Dumbbells' },

  // Arms - Biceps
  { id: 'biceps-1', name: 'Dumbbell Curl', muscleGroup: 'Arms', equipment: 'Dumbbells' },
  { id: 'biceps-2', name: 'Hammer Curl', muscleGroup: 'Arms', equipment: 'Dumbbells' },
  { id: 'biceps-3', name: 'Barbell Curl', muscleGroup: 'Arms', equipment: 'Barbell' },
  { id: 'biceps-4', name: 'Cable Curl', muscleGroup: 'Arms', equipment: 'Cable Machine' },
  { id: 'biceps-5', name: 'Resistance Band Curl', muscleGroup: 'Arms', equipment: 'Resistance Bands' },
  { id: 'biceps-6', name: 'Concentration Curl', muscleGroup: 'Arms', equipment: 'Dumbbells' },
  { id: 'biceps-7', name: 'Preacher Curl', muscleGroup: 'Arms', equipment: 'EZ Bar or Preacher Machine' },
  { id: 'biceps-8', name: 'Zottman Curl', muscleGroup: 'Arms', equipment: 'Dumbbells' },

  // Arms - Triceps
  { id: 'triceps-1', name: 'Tricep Dips', muscleGroup: 'Arms', equipment: 'Bench / Dip Bars' },
  { id: 'triceps-2', name: 'Dumbbell Overhead Triceps Extension', muscleGroup: 'Arms', equipment: 'Dumbbells' },
  { id: 'triceps-3', name: 'Tricep Pushdown', muscleGroup: 'Arms', equipment: 'Cable Machine' },
  { id: 'triceps-4', name: 'Close-Grip Push-Up', muscleGroup: 'Arms', equipment: 'Bodyweight' },
  { id: 'triceps-5', name: 'Machine Triceps Extension', muscleGroup: 'Arms', equipment: 'Triceps Machine' },
  { id: 'triceps-6', name: 'Skull Crushers', muscleGroup: 'Arms', equipment: 'EZ Bar' },
  { id: 'triceps-7', name: 'Overhead Cable Triceps Extension', muscleGroup: 'Arms', equipment: 'Cable Machine' },
  { id: 'triceps-8', name: 'Tate Press', muscleGroup: 'Arms', equipment: 'Dumbbells' },
  { id: 'triceps-9', name: 'Close-Grip Bench Press', muscleGroup: 'Arms', equipment: 'Barbell' },

  // Legs - Quads
  { id: 'quads-1', name: 'Bodyweight Squat', muscleGroup: 'Legs', equipment: 'Bodyweight' },
  { id: 'quads-2', name: 'Goblet Squat', muscleGroup: 'Legs', equipment: 'Dumbbell' },
  { id: 'quads-3', name: 'Bulgarian Split Squat', muscleGroup: 'Legs', equipment: 'Dumbbells / Bodyweight' },
  { id: 'quads-4', name: 'Barbell Back Squat', muscleGroup: 'Legs', equipment: 'Barbell + Rack' },
  { id: 'quads-5', name: 'Leg Extension', muscleGroup: 'Legs', equipment: 'Leg Extension Machine' },
  { id: 'quads-6', name: 'Leg Press', muscleGroup: 'Legs', equipment: 'Leg Press Machine' },
  { id: 'quads-7', name: 'Hack Squat', muscleGroup: 'Legs', equipment: 'Hack Squat Machine' },
  { id: 'quads-8', name: 'Step-Up', muscleGroup: 'Legs', equipment: 'Box + Dumbbells' },
  { id: 'quads-9', name: 'Smith Machine Front Squat', muscleGroup: 'Legs', equipment: 'Smith Machine' },

  // Legs - Hamstrings
  { id: 'hamstrings-1', name: 'Romanian Deadlift', muscleGroup: 'Legs', equipment: 'Barbell / Dumbbells' },
  { id: 'hamstrings-2', name: 'Glute Bridge (Hamstring Focus)', muscleGroup: 'Legs', equipment: 'Bodyweight' },
  { id: 'hamstrings-3', name: 'Lying Leg Curl', muscleGroup: 'Legs', equipment: 'Lying Leg Curl Machine' },
  { id: 'hamstrings-4', name: 'Seated Leg Curl', muscleGroup: 'Legs', equipment: 'Seated Hamstring Curl Machine' },
  { id: 'hamstrings-5', name: 'Stability Ball Leg Curl', muscleGroup: 'Legs', equipment: 'Exercise Ball' },
  { id: 'hamstrings-6', name: 'Nordic Hamstring Curl', muscleGroup: 'Legs', equipment: 'Bodyweight' },
  { id: 'hamstrings-7', name: 'Good Morning', muscleGroup: 'Legs', equipment: 'Barbell' },

  // Legs - Glutes
  { id: 'glutes-1', name: 'Glute Bridge', muscleGroup: 'Legs', equipment: 'Bodyweight / Dumbbells' },
  { id: 'glutes-2', name: 'Hip Thrust', muscleGroup: 'Legs', equipment: 'Barbell / Dumbbells + Bench' },
  { id: 'glutes-3', name: 'Dumbbell Romanian Deadlift', muscleGroup: 'Legs', equipment: 'Dumbbells' },
  { id: 'glutes-4', name: 'Cable Kickbacks', muscleGroup: 'Legs', equipment: 'Cable Machine' },
  { id: 'glutes-5', name: 'Step-Ups', muscleGroup: 'Legs', equipment: 'Box + Dumbbells / Bodyweight' },
  { id: 'glutes-6', name: 'Cable Pull Through', muscleGroup: 'Legs', equipment: 'Cable Machine' },
  { id: 'glutes-7', name: 'Donkey Kicks', muscleGroup: 'Legs', equipment: 'Bodyweight / Cable' },
  { id: 'glutes-8', name: 'Smith Machine Hip Thrust', muscleGroup: 'Legs', equipment: 'Smith Machine' },
  { id: 'glutes-9', name: 'Cable Kickback', muscleGroup: 'Legs', equipment: 'Cable Machine' },
  { id: 'glutes-10', name: 'Kettlebell Swing', muscleGroup: 'Legs', equipment: 'Kettlebell' },

  // Legs - Calves
  { id: 'calves-1', name: 'Standing Calf Raise', muscleGroup: 'Legs', equipment: 'Bodyweight / Smith Machine' },
  { id: 'calves-2', name: 'Seated Calf Raise', muscleGroup: 'Legs', equipment: 'Seated Calf Raise Machine' },
  { id: 'calves-3', name: 'Dumbbell Calf Raise', muscleGroup: 'Legs', equipment: 'Dumbbells' },
  { id: 'calves-4', name: 'Donkey Calf Raise', muscleGroup: 'Legs', equipment: 'Donkey Calf Raise Machine' },
  { id: 'calves-5', name: 'Barbell Calf Raise', muscleGroup: 'Legs', equipment: 'Barbell' },
  { id: 'calves-6', name: 'Smith Machine Calf Raise', muscleGroup: 'Legs', equipment: 'Smith Machine' },

  // Core
  { id: 'core-1', name: 'Plank', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-2', name: 'Crunch', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-3', name: 'Hanging Leg Raise', muscleGroup: 'Core', equipment: 'Pull-Up Bar' },
  { id: 'core-4', name: 'Cable Crunch', muscleGroup: 'Core', equipment: 'Cable Machine' },
  { id: 'core-5', name: 'Russian Twists', muscleGroup: 'Core', equipment: 'Bodyweight / Dumbbell' },
  { id: 'core-6', name: 'Bicycle Crunch', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-7', name: 'Sit-Up', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-8', name: 'Leg Raise', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-9', name: 'Side Plank', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-10', name: 'Hollow Body Hold', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-11', name: 'Dragon Flag', muscleGroup: 'Core', equipment: 'Bodyweight' },
  { id: 'core-12', name: 'Russian Twists', muscleGroup: 'Core', equipment: 'Dumbbell' },
  { id: 'core-13', name: 'Cable Woodchopper', muscleGroup: 'Core', equipment: 'Cable Machine' },

  // Cardio
  { id: 'cardio-1', name: 'Treadmill Run/Walk', muscleGroup: 'Cardio', equipment: 'Treadmill' },
  { id: 'cardio-2', name: 'Stationary Bike', muscleGroup: 'Cardio', equipment: 'Stationary Bike' },
  { id: 'cardio-3', name: 'StairMaster', muscleGroup: 'Cardio', equipment: 'StairMaster' },
  { id: 'cardio-4', name: 'Rowing Machine', muscleGroup: 'Cardio', equipment: 'Rower' },
  { id: 'cardio-5', name: 'Jump Rope', muscleGroup: 'Cardio', equipment: 'Jump Rope' },
  { id: 'cardio-6', name: 'HIIT Bodyweight Circuit', muscleGroup: 'Cardio', equipment: 'Bodyweight' },

  // Full Body / Compound
  { id: 'fullbody-1', name: 'Clean and Press', muscleGroup: 'Full Body', equipment: 'Barbell' },
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
  const [exerciseSearchTerm, setExerciseSearchTerm] = useState('')
  const [selectedMuscleFilter, setSelectedMuscleFilter] = useState('All')
  const [selectedEquipmentFilter, setSelectedEquipmentFilter] = useState('All')
  const [expandedWorkoutDay, setExpandedWorkoutDay] = useState<string | null>(null)
  const [showSuccessMessage, setShowSuccessMessage] = useState(false)
  const [formErrors, setFormErrors] = useState<{[key: string]: string}>({})
  const [isAnalyzing, setIsAnalyzing] = useState(false)
  const [showExerciseDetails, setShowExerciseDetails] = useState<{[key: string]: boolean}>({})

  // Form validation
  const validateForm = () => {
    const errors: {[key: string]: string} = {}

    if (userData.height < 100 || userData.height > 250) {
      errors.height = 'Height must be between 100-250 cm'
    }
    if (userData.weight < 30 || userData.weight > 300) {
      errors.weight = 'Weight must be between 30-300 kg'
    }
    if (userData.age < 16 || userData.age > 100) {
      errors.age = 'Age must be between 16-100 years'
    }
    if (userData.availableDays.length === 0) {
      errors.availableDays = 'Please select at least one training day'
    }
    if (userData.availableEquipment.length === 0) {
      errors.availableEquipment = 'Please select at least one equipment type'
    }

    setFormErrors(errors)
    return Object.keys(errors).length === 0
  }

  // Filtered exercise database
  const filteredExercises = EXERCISE_DATABASE.filter(exercise => {
    const matchesSearch = exercise.name.toLowerCase().includes(exerciseSearchTerm.toLowerCase()) ||
        exercise.muscleGroup.toLowerCase().includes(exerciseSearchTerm.toLowerCase()) ||
        exercise.equipment.toLowerCase().includes(exerciseSearchTerm.toLowerCase())

    const matchesMuscle = selectedMuscleFilter === 'All' || exercise.muscleGroup === selectedMuscleFilter
    const matchesEquipment = selectedEquipmentFilter === 'All' || exercise.equipment.includes(selectedEquipmentFilter)

    return matchesSearch && matchesMuscle && matchesEquipment
  })

  // Get unique muscle groups and equipment types for filters
  const uniqueMuscleGroups = ['All', ...Array.from(new Set(EXERCISE_DATABASE.map(ex => ex.muscleGroup)))]
  const uniqueEquipmentTypes = ['All', ...Array.from(new Set(EXERCISE_DATABASE.flatMap(ex =>
      ex.equipment.split(/[,+/]/).map(eq => eq.trim()).filter(eq => eq.length > 0)
  )))]

  // Export workout plan as JSON
  const exportWorkoutPlan = () => {
    if (!workoutPlan) return

    const dataStr = JSON.stringify({
      ...workoutPlan,
      analysis: results,
      exportDate: new Date().toISOString()
    }, null, 2)

    const dataBlob = new Blob([dataStr], { type: 'application/json' })
    const url = URL.createObjectURL(dataBlob)
    const link = document.createElement('a')
    link.href = url
    link.download = `workout-plan-${workoutPlan.id}.json`
    document.body.appendChild(link)
    link.click()
    document.body.removeChild(link)
    URL.revokeObjectURL(url)
  }

  // Show success message
  useEffect(() => {
    if (showSuccessMessage) {
      const timer = setTimeout(() => setShowSuccessMessage(false), 3000)
      return () => clearTimeout(timer)
    }
  }, [showSuccessMessage])

  const analyzeData = async () => {
    if (!validateForm()) {
      return
    }

    setLoading(true)
    setIsAnalyzing(true)
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

      // Parse and set the REAL workout plan from backend
      try {
        const workoutPlanData = response.data.workoutPlan
        console.log('Raw workout plan data:', workoutPlanData)

        // The weekly schedule should now be a proper JSON object (not string)
        let weeklySchedule = workoutPlanData.weeklySchedule || {};

        console.log('Weekly schedule from backend:', weeklySchedule)

        setWorkoutPlan({
          id: workoutPlanData.id || 'plan-1',
          name: workoutPlanData.name || 'Smart Workout Plan',
          description: workoutPlanData.description || 'AI-generated workout plan',
          exercises: [], // Will be populated from weekly schedule
          weeklySchedule: weeklySchedule,
          weeklyCalories: workoutPlanData.weeklyCalories || 0,
          status: workoutPlanData.status || 'Generated successfully'
        })

        console.log('Workout plan set:', {
          name: workoutPlanData.name,
          weeklyCalories: workoutPlanData.weeklyCalories,
          scheduleKeys: Object.keys(weeklySchedule),
          status: workoutPlanData.status
        })

        setShowSuccessMessage(true)

      } catch (planError) {
        console.error('Error processing workout plan:', planError)
        // Fallback to basic plan
        setWorkoutPlan({
          id: 'plan-fallback',
          name: 'Workout Plan (Processing Error)',
          description: 'Plan generated but display error occurred',
          exercises: []
        })
      }

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
      setIsAnalyzing(false)
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
      <div className="min-h-screen bg-gradient-to-br from-slate-900 via-slate-800 to-slate-900 text-white relative overflow-hidden">
        {/* Background decoration */}
        <div className="absolute inset-0 opacity-20">
          <div className="w-full h-full" style={{
            backgroundImage: `url("data:image/svg+xml,%3Csvg width='60' height='60' viewBox='0 0 60 60' xmlns='http://www.w3.org/2000/svg'%3E%3Cg fill='none' fill-rule='evenodd'%3E%3Cg fill='%23374151' fill-opacity='0.1'%3E%3Ccircle cx='30' cy='30' r='4'/%3E%3C/g%3E%3C/g%3E%3C/svg%3E")`
          }}></div>
        </div>

        {/* Success notification */}
        {showSuccessMessage && (
            <div className="fixed top-4 right-4 z-50 bg-green-600 text-white px-6 py-3 rounded-lg shadow-lg animate-slide-in-right flex items-center gap-2">
              <Check className="w-5 h-5" />
              <span>Workout plan generated successfully!</span>
            </div>
        )}

        {/* Header */}
        <header className="border-b border-slate-700/50 backdrop-blur-sm bg-slate-900/80 sticky top-0 z-40">
          <div className="container mx-auto px-6 py-4">
            <div className="flex items-center justify-between">
              <div className="flex items-center gap-4">
                <div className="relative">
                  <div className="w-12 h-12 bg-gradient-to-br from-blue-500 via-purple-500 to-teal-500 rounded-xl flex items-center justify-center text-2xl shadow-lg">
                    <Dumbbell className="w-6 h-6 text-white" />
                  </div>
                  <div className="absolute -top-1 -right-1 w-4 h-4 bg-green-500 rounded-full animate-pulse"></div>
                </div>
                <div>
                  <h1 className="text-2xl font-bold bg-gradient-to-r from-blue-400 to-teal-400 bg-clip-text text-transparent">
                    Smart Workout Planner
                  </h1>
                  <p className="text-slate-400 text-sm">AI-powered personalized fitness</p>
                </div>
              </div>
              <div className="flex items-center gap-3">
                {workoutPlan && (
                    <Button
                        onClick={exportWorkoutPlan}
                        variant="outline"
                        size="sm"
                        className="border-slate-600 hover:border-blue-500 transition-colors"
                    >
                      <Download className="w-4 h-4 mr-2" />
                      Export Plan
                    </Button>
                )}
                <div className="flex items-center gap-2 bg-slate-800 px-3 py-2 rounded-lg">
                  <div className="w-2 h-2 bg-green-500 rounded-full animate-pulse"></div>
                  <span className="text-xs text-slate-400">Live</span>
                </div>
              </div>
            </div>
          </div>
        </header>

        {/* Main Content */}
        <main className="container mx-auto p-4 sm:p-6 relative z-10">
          <div className="grid grid-cols-1 xl:grid-cols-2 gap-6 lg:gap-8">
            {/* Personal Information Input */}
            <Card className="bg-slate-800/50 backdrop-blur-sm border-slate-700/50 shadow-2xl hover:shadow-blue-500/10 transition-all duration-300">
              <CardHeader className="border-b border-slate-700/50 p-4 sm:p-6">
                <div className="flex items-center gap-3">
                  <div className="w-10 h-10 sm:w-12 sm:h-12 bg-gradient-to-br from-blue-500 to-purple-500 rounded-lg flex items-center justify-center">
                    <Target className="w-5 h-5 sm:w-6 sm:h-6 text-white" />
                  </div>
                  <div>
                    <CardTitle className="text-white text-lg sm:text-xl">Personal Information</CardTitle>
                    <p className="text-slate-400 text-sm">Tell us about yourself to get personalized recommendations</p>
                  </div>
                </div>
              </CardHeader>
              <CardContent className="space-y-6 sm:space-y-8 p-4 sm:p-6">
                {/* Height, Weight, Age */}
                <div className="grid grid-cols-1 sm:grid-cols-3 gap-4 sm:gap-6">
                  <div className="space-y-3">
                    <label className="text-sm font-medium text-slate-300 flex items-center gap-2">
                      <TrendingUp className="w-4 h-4" />
                      Height (cm)
                    </label>
                    <Input
                        type="number"
                        value={userData.height || ''}
                        onChange={(e: ChangeEvent<HTMLInputElement>) => updateField('height', parseInt(e.target.value) || 0)}
                        className={`bg-slate-700/50 border-slate-600 text-white transition-all focus:border-blue-500 focus:ring-2 focus:ring-blue-500/20 h-12 sm:h-14 text-base ${formErrors.height ? 'border-red-500' : ''}`}
                        min="100"
                        max="250"
                        placeholder="175"
                    />
                    {formErrors.height && <p className="text-red-400 text-xs mt-1">{formErrors.height}</p>}
                  </div>
                  <div className="space-y-3">
                    <label className="text-sm font-medium text-slate-300 flex items-center gap-2">
                      <Heart className="w-4 h-4" />
                      Weight (kg)
                    </label>
                    <Input
                        type="number"
                        value={userData.weight || ''}
                        onChange={(e: ChangeEvent<HTMLInputElement>) => updateField('weight', parseInt(e.target.value) || 0)}
                        className={`bg-slate-700/50 border-slate-600 text-white transition-all focus:border-blue-500 focus:ring-2 focus:ring-blue-500/20 h-12 sm:h-14 text-base ${formErrors.weight ? 'border-red-500' : ''}`}
                        min="30"
                        max="300"
                        placeholder="70"
                    />
                    {formErrors.weight && <p className="text-red-400 text-xs mt-1">{formErrors.weight}</p>}
                  </div>
                  <div className="space-y-3">
                    <label className="text-sm font-medium text-slate-300 flex items-center gap-2">
                      <Calendar className="w-4 h-4" />
                      Age
                    </label>
                    <Input
                        type="number"
                        value={userData.age || ''}
                        onChange={(e: ChangeEvent<HTMLInputElement>) => updateField('age', parseInt(e.target.value) || 0)}
                        className={`bg-slate-700/50 border-slate-600 text-white transition-all focus:border-blue-500 focus:ring-2 focus:ring-blue-500/20 h-12 sm:h-14 text-base ${formErrors.age ? 'border-red-500' : ''}`}
                        min="16"
                        max="100"
                        placeholder="25"
                    />
                    {formErrors.age && <p className="text-red-400 text-xs mt-1">{formErrors.age}</p>}
                  </div>
                </div>

                {/* Gender */}
                <div className="space-y-2">
                  <label className="text-sm font-medium text-slate-300 flex items-center gap-2">
                    <Star className="w-4 h-4" />
                    Gender
                  </label>
                  <Select
                      value={userData.gender}
                      onChange={(e: ChangeEvent<HTMLSelectElement>) => updateField('gender', e.target.value)}
                      className="bg-slate-700/50 border-slate-600 text-white focus:border-blue-500 focus:ring-2 focus:ring-blue-500/20 transition-all"
                  >
                    <option value="male">Male</option>
                    <option value="female">Female</option>
                    <option value="prefer-not-to-say">Prefer not to say</option>
                  </Select>
                </div>

                {/* Training Days */}
                <div className="space-y-3">
                  <label className="text-sm font-medium text-slate-300 flex items-center gap-2">
                    <Calendar className="w-4 h-4" />
                    Available Training Days
                  </label>
                  {formErrors.availableDays && <p className="text-red-400 text-xs">{formErrors.availableDays}</p>}
                  <div className="grid grid-cols-2 sm:grid-cols-4 lg:grid-cols-7 gap-2">
                    {DAYS_OF_WEEK.map((day) => (
                        <button
                            key={day}
                            onClick={() => toggleDay(day)}
                            className={`p-3 rounded-lg text-sm font-medium transition-all duration-200 border ${
                                userData.availableDays.includes(day)
                                    ? 'bg-gradient-to-r from-blue-500 to-purple-500 border-transparent text-white shadow-lg transform scale-105'
                                    : 'bg-slate-700/50 border-slate-600 text-slate-300 hover:bg-slate-600/50 hover:border-blue-500/50'
                            }`}
                        >
                          {day.slice(0, 3)}
                        </button>
                    ))}
                  </div>
                </div>

                {/* Training Goal */}
                <div className="space-y-3">
                  <label className="text-sm font-medium text-slate-300 flex items-center gap-2">
                    <Zap className="w-4 h-4" />
                    Training Goal & Intensity
                  </label>
                  <div className="space-y-3">
                    <select
                        value={userData.intensity.toString()}
                        onChange={(e: ChangeEvent<HTMLSelectElement>) => updateField('intensity', parseInt(e.target.value))}
                        className="w-full bg-slate-700/50 border border-slate-600 text-white rounded-lg px-4 py-3 focus:border-purple-500 focus:ring-2 focus:ring-purple-500/20 transition-all"
                    >
                      <option value="1">🏃 Endurance Training (12-20 reps, 3-4 sets)</option>
                      <option value="2">💪 Light Hypertrophy (12-15 reps, 3-4 sets)</option>
                      <option value="3">🎯 Standard Hypertrophy (8-12 reps, 3 sets)</option>
                      <option value="4">⚡ Strength Hypertrophy (6-10 reps, 2-3 sets)</option>
                      <option value="5">🔥 Pure Strength (4-6 reps, 2 sets)</option>
                    </select>

                    {/* Intensity Visual Indicator */}
                    <div className="bg-slate-700/30 rounded-lg p-3 border border-slate-600/30">
                      <div className="flex items-center justify-between mb-2">
                        <span className="text-xs text-slate-400 font-medium">Intensity Level</span>
                        <span className="text-xs font-bold text-white">{userData.intensity}/5</span>
                      </div>
                      <div className="flex gap-1">
                        {[1, 2, 3, 4, 5].map(level => (
                            <div
                                key={level}
                                className={`flex-1 h-2 rounded-full transition-all duration-300 ${
                                    level <= userData.intensity
                                        ? level <= 2 ? 'bg-gradient-to-r from-green-500 to-emerald-500' :
                                            level <= 3 ? 'bg-gradient-to-r from-yellow-500 to-orange-500' :
                                                'bg-gradient-to-r from-red-500 to-pink-500'
                                        : 'bg-slate-600'
                                }`}
                            />
                        ))}
                      </div>
                      <div className="flex justify-between text-xs text-slate-500 mt-1">
                        <span>Light</span>
                        <span>Intense</span>
                      </div>
                    </div>
                  </div>
                </div>

                {/* Available Equipment */}
                <div className="space-y-3">
                  <div className="flex items-center justify-between">
                    <label className="text-sm font-medium text-slate-300 flex items-center gap-2">
                      <Dumbbell className="w-4 h-4" />
                      Available Equipment
                    </label>
                    <button
                        onClick={() => setShowEquipmentDropdown(!showEquipmentDropdown)}
                        className="text-sm text-blue-400 hover:text-blue-300 font-medium flex items-center gap-1 transition-colors"
                    >
                      {showEquipmentDropdown ? 'Close' : 'Edit'}
                      {showEquipmentDropdown ? <ChevronUp className="w-3 h-3" /> : <ChevronDown className="w-3 h-3" />}
                    </button>
                  </div>

                  {formErrors.availableEquipment && <p className="text-red-400 text-xs">{formErrors.availableEquipment}</p>}

                  {/* Selected Equipment Display */}
                  <div className="flex flex-wrap gap-2">
                    {userData.availableEquipment.slice(0, 6).map((equipment) => (
                        <div key={equipment} className="bg-gradient-to-r from-blue-500/20 to-purple-500/20 text-blue-300 px-3 py-1 rounded-full text-xs font-medium border border-blue-500/30">
                          {equipment}
                        </div>
                    ))}
                    {userData.availableEquipment.length > 6 && (
                        <div className="bg-slate-700/50 text-slate-400 px-3 py-1 rounded-full text-xs font-medium border border-slate-600">
                          +{userData.availableEquipment.length - 6} more
                        </div>
                    )}
                  </div>

                  {showEquipmentDropdown && (
                      <div className="bg-slate-700/50 backdrop-blur-sm border border-slate-600/50 rounded-xl p-4 space-y-3 animate-in slide-in-from-top-2 duration-200">
                        <div className="relative">
                          <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 w-4 h-4 text-slate-400" />
                          <Input
                              type="text"
                              placeholder="Search equipment..."
                              value={equipmentSearchTerm}
                              onChange={(e) => setEquipmentSearchTerm(e.target.value)}
                              className="pl-10 bg-slate-800/50 border-slate-600 text-white focus:border-blue-500 focus:ring-2 focus:ring-blue-500/20 transition-all"
                          />
                        </div>

                        <div className="max-h-48 overflow-y-auto space-y-2">
                          {filteredEquipment.map((equipment) => {
                            const isSelected = userData.availableEquipment.includes(equipment)
                            return (
                                <label key={equipment} className="flex items-center space-x-3 cursor-pointer group">
                                  <div className="relative">
                                    <input
                                        type="checkbox"
                                        checked={isSelected}
                                        onChange={() => toggleEquipment(equipment)}
                                        className="sr-only"
                                    />
                                    <div className={`w-4 h-4 rounded border-2 transition-all duration-200 ${
                                        isSelected
                                            ? 'bg-gradient-to-r from-blue-500 to-purple-500 border-transparent'
                                            : 'border-slate-500 group-hover:border-blue-400'
                                    }`}>
                                      {isSelected && (
                                          <Check className="w-3 h-3 text-white absolute top-0.5 left-0.5 transform -translate-x-0.5 -translate-y-0.5" />
                                      )}
                                    </div>
                                  </div>
                                  <span className={`text-sm transition-colors ${
                                      isSelected ? 'text-white font-medium' : 'text-slate-300 group-hover:text-white'
                                  }`}>
                              {equipment}
                            </span>
                                </label>
                            )
                          })}
                        </div>

                        <div className="flex items-center justify-between pt-2 border-t border-slate-600/30">
                          <div className="text-xs text-slate-400">
                            {userData.availableEquipment.length} of {EQUIPMENT_OPTIONS.length} selected
                          </div>
                          <div className="flex gap-2">
                            <Button
                                onClick={() => updateField('availableEquipment', [])}
                                variant="outline"
                                size="sm"
                                className="text-xs border-slate-600 hover:border-red-500 text-slate-300 hover:text-red-400 transition-colors"
                            >
                              Clear All
                            </Button>
                            <Button
                                onClick={() => updateField('availableEquipment', EQUIPMENT_OPTIONS)}
                                variant="outline"
                                size="sm"
                                className="text-xs border-slate-600 hover:border-green-500 text-slate-300 hover:text-green-400 transition-colors"
                            >
                              Select All
                            </Button>
                          </div>
                        </div>
                      </div>
                  )}
                </div>

                {/* Analyze Button */}
                <div className="pt-2">
                  <Button
                      onClick={analyzeData}
                      disabled={loading || isAnalyzing}
                      className="w-full bg-gradient-to-r from-blue-600 via-purple-600 to-teal-600 hover:from-blue-700 hover:via-purple-700 hover:to-teal-700 text-white font-semibold py-4 text-base shadow-xl hover:shadow-2xl transition-all duration-300 transform hover:scale-[1.02] disabled:opacity-50 disabled:cursor-not-allowed disabled:transform-none"
                  >
                    {isAnalyzing ? (
                        <div className="flex items-center justify-center gap-3">
                          <div className="relative">
                            <div className="w-5 h-5 border-2 border-white/30 border-t-white rounded-full animate-spin"></div>
                            <div className="absolute inset-0 w-5 h-5 border-2 border-transparent border-b-white/50 rounded-full animate-pulse"></div>
                          </div>
                          <span>Analyzing Your Data...</span>
                          <div className="flex gap-1">
                            <div className="w-1 h-1 bg-white rounded-full animate-bounce" style={{animationDelay: '0ms'}}></div>
                            <div className="w-1 h-1 bg-white rounded-full animate-bounce" style={{animationDelay: '150ms'}}></div>
                            <div className="w-1 h-1 bg-white rounded-full animate-bounce" style={{animationDelay: '300ms'}}></div>
                          </div>
                        </div>
                    ) : (
                        <div className="flex items-center justify-center gap-2">
                          <Zap className="w-5 h-5" />
                          <span>Generate Smart Workout Plan</span>
                          <TrendingUp className="w-5 h-5" />
                        </div>
                    )}
                  </Button>
                </div>

                {error && (
                    <div className="p-3 bg-red-900/20 border border-red-500 rounded-md">
                      <p className="text-red-400 text-sm">{error}</p>
                    </div>
                )}
              </CardContent>
            </Card>

            {/* Target Muscle Groups */}
            <Card className="bg-slate-800/50 backdrop-blur-sm border-slate-700/50 shadow-2xl hover:shadow-purple-500/10 transition-all duration-300">
              <CardHeader className="border-b border-slate-700/50">
                <div className="flex items-center gap-3">
                  <div className="w-8 h-8 bg-gradient-to-br from-purple-500 to-pink-500 rounded-lg flex items-center justify-center">
                    <Zap className="w-4 h-4 text-white" />
                  </div>
                  <div>
                    <CardTitle className="text-white text-lg">Muscle Group Priorities</CardTitle>
                    <p className="text-slate-400 text-sm">Set your training focus for optimal results</p>
                  </div>
                </div>
              </CardHeader>
              <CardContent className="space-y-6 p-6">
                <Button
                    onClick={setAllBalanced}
                    className="w-full bg-gradient-to-r from-purple-600 to-pink-600 hover:from-purple-700 hover:to-pink-700 text-white font-medium py-2.5 transition-all duration-200 shadow-lg hover:shadow-purple-500/25"
                >
                  <Star className="w-4 h-4 mr-2" />
                  Set All Balanced
                </Button>

                <div className="space-y-4">
                  {MUSCLE_GROUPS.map((muscle) => {
                    const getIcon = (muscle: string) => {
                      switch(muscle) {
                        case 'Chest': return '💪'
                        case 'Back': return '🏋️'
                        case 'Shoulders': return '🦾'
                        case 'Arms': return '💪'
                        case 'Legs': return '🦵'
                        case 'Core': return '🔥'
                        case 'Cardio': return '❤️'
                        default: return '💪'
                      }
                    }

                    const getPriorityColor = (priority: string) => {
                      switch(priority) {
                        case 'High': return 'from-red-500 to-orange-500'
                        case 'Medium': return 'from-yellow-500 to-amber-500'
                        case 'Low': return 'from-blue-500 to-cyan-500'
                        default: return 'from-slate-600 to-slate-700'
                      }
                    }

                    return (
                        <div key={muscle} className="bg-slate-700/30 rounded-xl p-4 border border-slate-600/50 hover:border-purple-500/50 transition-all duration-200">
                          <div className="flex items-center justify-between mb-3">
                            <div className="flex items-center gap-3">
                              <span className="text-2xl">{getIcon(muscle)}</span>
                              <span className="text-slate-200 font-semibold text-sm">{muscle}</span>
                            </div>
                            {userData.muscleGroupPriorities[muscle] && (
                                <div className={`px-2 py-1 rounded-full text-xs font-medium text-white bg-gradient-to-r ${getPriorityColor(userData.muscleGroupPriorities[muscle])}`}>
                                  {userData.muscleGroupPriorities[muscle]}
                                </div>
                            )}
                          </div>
                          <div className="flex gap-2">
                            {['High', 'Medium', 'Low'].map((priority) => {
                              const isSelected = userData.muscleGroupPriorities[muscle] === priority
                              return (
                                  <button
                                      key={priority}
                                      onClick={() => setPriorityLevel(muscle, priority)}
                                      className={`flex-1 px-3 py-2 text-xs font-medium rounded-lg border transition-all duration-200 ${
                                          isSelected
                                              ? `bg-gradient-to-r ${getPriorityColor(priority)} border-transparent text-white shadow-lg transform scale-105`
                                              : 'bg-slate-700/50 border-slate-600 text-slate-300 hover:bg-slate-600/50 hover:border-purple-500/50 hover:text-white'
                                      }`}
                                  >
                                    {priority}
                                  </button>
                              )
                            })}
                          </div>
                        </div>
                    )
                  })}
                </div>
              </CardContent>
            </Card>
          </div>

          {/* Analysis Results */}
          {results && (
              <Card className="bg-slate-800/50 backdrop-blur-sm border-slate-700/50 shadow-2xl hover:shadow-green-500/10 transition-all duration-300">
                <CardHeader className="border-b border-slate-700/50">
                  <div className="flex items-center gap-3">
                    <div className="w-8 h-8 bg-gradient-to-br from-green-500 to-emerald-500 rounded-lg flex items-center justify-center">
                      <TrendingUp className="w-4 h-4 text-white" />
                    </div>
                    <div>
                      <CardTitle className="text-white text-lg">Analysis Results</CardTitle>
                      <p className="text-slate-400 text-sm">Your personalized fitness metrics</p>
                    </div>
                  </div>
                </CardHeader>
                <CardContent className="p-6">
                  <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
                    {/* BMI Index */}
                    <div className="relative group">
                      <div className="absolute inset-0 bg-gradient-to-r from-blue-500/20 to-cyan-500/20 rounded-xl blur-xl group-hover:blur-2xl transition-all duration-300"></div>
                      <div className="relative bg-slate-700/50 backdrop-blur-sm p-6 rounded-xl border border-slate-600/50 hover:border-blue-500/50 transition-all duration-300">
                        <div className="flex items-center gap-3 mb-3">
                          <div className="w-10 h-10 bg-gradient-to-br from-blue-500 to-cyan-500 rounded-lg flex items-center justify-center">
                            <Heart className="w-5 h-5 text-white" />
                          </div>
                          <div>
                            <div className="text-sm text-slate-400 font-medium">BMI Index</div>
                            <div className="text-xs text-slate-500">Body Mass Index</div>
                          </div>
                        </div>
                        <div className="space-y-2">
                          <div className="text-3xl font-bold text-white">{results.bmi}</div>
                          <div className={`text-sm font-medium px-2 py-1 rounded-full inline-block ${
                              results.bmiStatus.toLowerCase().includes('normal') ? 'bg-green-500/20 text-green-400' :
                                  results.bmiStatus.toLowerCase().includes('underweight') ? 'bg-blue-500/20 text-blue-400' :
                                      results.bmiStatus.toLowerCase().includes('overweight') ? 'bg-yellow-500/20 text-yellow-400' :
                                          'bg-red-500/20 text-red-400'
                          }`}>
                            {results.bmiStatus}
                          </div>
                        </div>
                      </div>
                    </div>

                    {/* Daily Calorie Intake */}
                    <div className="relative group">
                      <div className="absolute inset-0 bg-gradient-to-r from-green-500/20 to-emerald-500/20 rounded-xl blur-xl group-hover:blur-2xl transition-all duration-300"></div>
                      <div className="relative bg-slate-700/50 backdrop-blur-sm p-6 rounded-xl border border-slate-600/50 hover:border-green-500/50 transition-all duration-300">
                        <div className="flex items-center gap-3 mb-3">
                          <div className="w-10 h-10 bg-gradient-to-br from-green-500 to-emerald-500 rounded-lg flex items-center justify-center">
                            <Flame className="w-5 h-5 text-white" />
                          </div>
                          <div>
                            <div className="text-sm text-slate-400 font-medium">Daily Calories</div>
                            <div className="text-xs text-slate-500">Recommended Intake</div>
                          </div>
                        </div>
                        <div className="space-y-2">
                          <div className="text-3xl font-bold text-white">{results.dailyCalories.toLocaleString()}</div>
                          <div className="text-sm text-green-400 font-medium">kcal per day</div>
                        </div>
                      </div>
                    </div>

                    {/* Weekly Calorie Burn */}
                    <div className="relative group">
                      <div className="absolute inset-0 bg-gradient-to-r from-purple-500/20 to-pink-500/20 rounded-xl blur-xl group-hover:blur-2xl transition-all duration-300"></div>
                      <div className="relative bg-slate-700/50 backdrop-blur-sm p-6 rounded-xl border border-slate-600/50 hover:border-purple-500/50 transition-all duration-300">
                        <div className="flex items-center gap-3 mb-3">
                          <div className="w-10 h-10 bg-gradient-to-br from-purple-500 to-pink-500 rounded-lg flex items-center justify-center">
                            <Zap className="w-5 h-5 text-white" />
                          </div>
                          <div>
                            <div className="text-sm text-slate-400 font-medium">Weekly Burn</div>
                            <div className="text-xs text-slate-500">Exercise Target</div>
                          </div>
                        </div>
                        <div className="space-y-2">
                          <div className="text-3xl font-bold text-white">{results.weeklyBurn.toLocaleString()}</div>
                          <div className="text-sm text-purple-400 font-medium">kcal per week</div>
                        </div>
                      </div>
                    </div>
                  </div>

                  {/* Additional Metrics Row */}
                  {results.bmr && (
                      <div className="mt-6 p-4 bg-slate-700/30 rounded-xl border border-slate-600/30">
                        <div className="flex items-center justify-between">
                          <div className="flex items-center gap-2">
                            <Clock className="w-4 h-4 text-slate-400" />
                            <span className="text-sm text-slate-300 font-medium">Basal Metabolic Rate (BMR)</span>
                          </div>
                          <div className="text-lg font-bold text-white">{results.bmr.toLocaleString()} kcal/day</div>
                        </div>
                      </div>
                  )}
                </CardContent>
              </Card>
          )}

          {/* Workout Planner */}
          <Card className="bg-slate-800/50 backdrop-blur-sm border-slate-700/50 shadow-2xl hover:shadow-teal-500/10 transition-all duration-300">
            <CardHeader className="border-b border-slate-700/50 p-4 sm:p-6">
              <div className="flex items-center justify-between flex-wrap gap-4">
                <div className="flex items-center gap-3">
                  <div className="w-10 h-10 sm:w-12 sm:h-12 bg-gradient-to-br from-teal-500 to-cyan-500 rounded-lg flex items-center justify-center">
                    <Calendar className="w-5 h-5 sm:w-6 sm:h-6 text-white" />
                  </div>
                  <div>
                    <CardTitle className="text-white text-lg sm:text-xl">Workout Planner</CardTitle>
                    <p className="text-slate-400 text-sm">Your personalized weekly training schedule</p>
                  </div>
                </div>
                <div className="flex items-center gap-2 sm:gap-3 flex-wrap">
                  {!workoutPlan ? (
                      <Button
                          onClick={analyzeData}
                          disabled={loading || isAnalyzing}
                          className="bg-gradient-to-r from-teal-600 to-cyan-600 hover:from-teal-700 hover:to-cyan-700 text-white font-medium px-4 sm:px-6 py-3 sm:py-4 text-sm sm:text-base transition-all duration-200 shadow-lg hover:shadow-teal-500/25 touch-manipulation"
                      >
                        {isAnalyzing ? (
                            <>
                              <div className="w-4 h-4 border-2 border-white/30 border-t-white rounded-full animate-spin mr-2"></div>
                              Generating...
                            </>
                        ) : (
                            <>
                              <Play className="w-4 h-4 mr-2" />
                              Create Plan
                            </>
                        )}
                      </Button>
                  ) : (
                      <>
                        <Button
                            onClick={analyzeData}
                            disabled={loading || isAnalyzing}
                            className="bg-gradient-to-r from-blue-600 to-indigo-600 hover:from-blue-700 hover:to-indigo-700 text-white font-medium px-4 sm:px-6 py-3 sm:py-4 text-sm sm:text-base transition-all duration-200 shadow-lg hover:shadow-blue-500/25 touch-manipulation"
                        >
                          {isAnalyzing ? (
                              <>
                                <div className="w-4 h-4 border-2 border-white/30 border-t-white rounded-full animate-spin mr-2"></div>
                                Regenerating...
                              </>
                          ) : (
                              <>
                                <Play className="w-4 h-4 mr-2" />
                                Regenerate Plan
                              </>
                          )}
                        </Button>
                        <Button
                            onClick={exportWorkoutPlan}
                            variant="outline"
                            className="border-slate-600 hover:border-teal-500 text-slate-300 hover:text-teal-400 transition-colors px-4 sm:px-6 py-3 sm:py-4 text-sm sm:text-base touch-manipulation"
                        >
                          <Download className="w-4 h-4 mr-2" />
                          Export
                        </Button>
                      </>
                  )}
                </div>
              </div>
            </CardHeader>
            <CardContent className="p-6">
              {!workoutPlan ? (
                  <div className="text-center py-12">
                    <div className="relative">
                      <div className="w-20 h-20 bg-gradient-to-br from-teal-500/20 to-cyan-500/20 rounded-full flex items-center justify-center mx-auto mb-6">
                        <div className="w-12 h-12 bg-gradient-to-br from-teal-500 to-cyan-500 rounded-full flex items-center justify-center">
                          <Dumbbell className="w-6 h-6 text-white" />
                        </div>
                      </div>
                      <div className="absolute top-0 left-1/2 transform -translate-x-1/2 w-16 h-16 bg-teal-500/10 rounded-full animate-ping"></div>
                    </div>
                    <h3 className="text-xl font-semibold text-white mb-2">Ready to Create Your Plan?</h3>
                    <p className="text-slate-400 mb-6 max-w-md mx-auto">
                      Fill out your personal information and preferences above, then click "Create Plan" to generate your personalized workout schedule.
                    </p>
                    <div className="flex items-center justify-center gap-6 text-sm text-slate-500">
                      <div className="flex items-center gap-2">
                        <Check className="w-4 h-4 text-green-500" />
                        <span>AI-Powered</span>
                      </div>
                      <div className="flex items-center gap-2">
                        <Check className="w-4 h-4 text-green-500" />
                        <span>Personalized</span>
                      </div>
                      <div className="flex items-center gap-2">
                        <Check className="w-4 h-4 text-green-500" />
                        <span>Science-Based</span>
                      </div>
                    </div>
                  </div>
              ) : (
                  <div className="space-y-6">
                    {/* Plan Overview */}
                    <div className="bg-slate-700/30 rounded-xl p-5 border border-slate-600/50">
                      <div className="flex items-start justify-between mb-4">
                        <div>
                          <h3 className="text-xl font-semibold text-white mb-1">{workoutPlan.name}</h3>
                          <p className="text-slate-400 text-sm">{workoutPlan.description}</p>
                        </div>
                        {workoutPlan.status && (
                            <div className="flex items-center gap-2 bg-green-500/20 text-green-400 px-3 py-1 rounded-full text-sm font-medium">
                              <Check className="w-3 h-3" />
                              {workoutPlan.status}
                            </div>
                        )}
                      </div>

                      {workoutPlan.weeklyCalories && workoutPlan.weeklyCalories > 0 && (
                          <div className="flex items-center gap-4 text-sm">
                            <div className="flex items-center gap-2 bg-orange-500/20 text-orange-400 px-3 py-2 rounded-lg">
                              <Flame className="w-4 h-4" />
                              <span className="font-medium">{workoutPlan.weeklyCalories.toLocaleString()} kcal/week</span>
                            </div>
                            <div className="flex items-center gap-2 bg-blue-500/20 text-blue-400 px-3 py-2 rounded-lg">
                              <Calendar className="w-4 h-4" />
                              <span className="font-medium">{userData.availableDays.length} days/week</span>
                            </div>
                            <div className="flex items-center gap-2 bg-purple-500/20 text-purple-400 px-3 py-2 rounded-lg">
                              <Target className="w-4 h-4" />
                              <span className="font-medium">Level {userData.intensity}</span>
                            </div>
                          </div>
                      )}
                    </div>

                    {/* Weekly Schedule Display */}
                    {workoutPlan.weeklySchedule && Object.keys(workoutPlan.weeklySchedule).length > 0 ? (
                        <div className="space-y-4">
                          <div className="flex items-center gap-2">
                            <h4 className="text-lg font-semibold text-white">Weekly Schedule</h4>
                            <div className="h-px bg-gradient-to-r from-slate-600 to-transparent flex-1"></div>
                          </div>

                          <div className="grid gap-4">
                            {Object.entries(workoutPlan.weeklySchedule).map(([day, workout]: [string, any]) => {
                              const isExpanded = expandedWorkoutDay === day
                              const getDayColor = (index: number) => {
                                const colors = ['from-red-500 to-pink-500', 'from-blue-500 to-cyan-500', 'from-green-500 to-emerald-500', 'from-yellow-500 to-orange-500', 'from-purple-500 to-indigo-500', 'from-teal-500 to-cyan-500', 'from-pink-500 to-rose-500']
                                return colors[index % colors.length]
                              }
                              const dayIndex = Object.keys(workoutPlan.weeklySchedule!).indexOf(day)

                              return (
                                  <div key={day} className="bg-slate-700/30 rounded-xl border border-slate-600/50 hover:border-blue-500/50 transition-all duration-200 overflow-hidden">
                                    <div
                                        className="p-4 cursor-pointer"
                                        onClick={() => setExpandedWorkoutDay(isExpanded ? null : day)}
                                    >
                                      <div className="flex items-center justify-between">
                                        <div className="flex items-center gap-4">
                                          <div className={`w-12 h-12 bg-gradient-to-br ${getDayColor(dayIndex)} rounded-lg flex items-center justify-center font-bold text-white text-sm`}>
                                            {day.slice(0, 3).toUpperCase()}
                                          </div>
                                          <div>
                                            <h5 className="text-white font-semibold text-lg">{day}</h5>
                                            <p className="text-slate-300 text-sm">{workout.name || 'Workout Session'}</p>
                                          </div>
                                        </div>

                                        <div className="flex items-center gap-4">
                                          <div className="flex items-center gap-4 text-sm text-slate-400">
                                            {workout.duration && (
                                                <div className="flex items-center gap-1">
                                                  <Clock className="w-4 h-4" />
                                                  <span>{workout.duration}min</span>
                                                </div>
                                            )}
                                            {workout.calories && (
                                                <div className="flex items-center gap-1">
                                                  <Flame className="w-4 h-4" />
                                                  <span>{workout.calories}cal</span>
                                                </div>
                                            )}
                                            {workout.type && (
                                                <div className={`px-2 py-1 rounded-full text-xs font-medium text-white bg-gradient-to-r ${
                                                    workout.type.toLowerCase().includes('strength') ? 'from-red-500 to-orange-500' :
                                                        workout.type.toLowerCase().includes('cardio') ? 'from-pink-500 to-rose-500' :
                                                            'from-blue-500 to-cyan-500'
                                                }`}>
                                                  {workout.type}
                                                </div>
                                            )}
                                          </div>

                                          <button className="text-slate-400 hover:text-white transition-colors">
                                            {isExpanded ? <ChevronUp className="w-5 h-5" /> : <ChevronDown className="w-5 h-5" />}
                                          </button>
                                        </div>
                                      </div>
                                    </div>

                                    {isExpanded && (
                                        <div className="px-4 pb-4 border-t border-slate-600/50 animate-in slide-in-from-top-2 duration-200">
                                          <div className="pt-4 space-y-3">
                                            {workout.exercises && (
                                                <div>
                                                  <h6 className="text-sm font-medium text-slate-300 mb-2 flex items-center gap-2">
                                                    <Dumbbell className="w-4 h-4" />
                                                    Exercises
                                                  </h6>
                                                  <p className="text-sm text-slate-400 bg-slate-800/50 p-3 rounded-lg">{workout.exercises}</p>
                                                </div>
                                            )}
                                            {workout.muscles && (
                                                <div>
                                                  <h6 className="text-sm font-medium text-slate-300 mb-2 flex items-center gap-2">
                                                    <Target className="w-4 h-4" />
                                                    Target Muscles
                                                  </h6>
                                                  <p className="text-sm text-slate-400 bg-slate-800/50 p-3 rounded-lg">{workout.muscles}</p>
                                                </div>
                                            )}

                                            <div className="flex items-center justify-between pt-2">
                                              <div className="text-xs text-slate-500">Click to start workout</div>
                                              <Button size="sm" className="bg-gradient-to-r from-green-600 to-emerald-600 hover:from-green-700 hover:to-emerald-700">
                                                <Play className="w-3 h-3 mr-1" />
                                                Start
                                              </Button>
                                            </div>
                                          </div>
                                        </div>
                                    )}
                                  </div>
                              )
                            })}
                          </div>
                        </div>
                    ) : (
                        <div className="text-center py-8 bg-slate-700/20 rounded-xl border border-slate-600/30">
                          <Calendar className="w-12 h-12 text-slate-500 mx-auto mb-3" />
                          <p className="text-slate-400 font-medium">Weekly schedule will appear here</p>
                          <p className="text-sm text-slate-500">Complete the analysis to see your personalized workout plan</p>
                        </div>
                    )}
                  </div>
              )}
            </CardContent>
          </Card>

          {/* Exercise Directory */}
          <Card className="bg-slate-800/50 backdrop-blur-sm border-slate-700/50 shadow-2xl hover:shadow-blue-500/10 transition-all duration-300">
            <CardHeader className="border-b border-slate-700/50 p-4 sm:p-6">
              <div className="flex items-center justify-between flex-wrap gap-4">
                <div className="flex items-center gap-3">
                  <div className="w-10 h-10 sm:w-12 sm:h-12 bg-gradient-to-br from-blue-500 to-indigo-500 rounded-lg flex items-center justify-center">
                    <Dumbbell className="w-5 h-5 sm:w-6 sm:h-6 text-white" />
                  </div>
                  <div>
                    <CardTitle className="text-white text-lg sm:text-xl">Exercise Directory</CardTitle>
                    <p className="text-slate-400 text-sm">Browse {EXERCISE_DATABASE.length} exercises in our database</p>
                  </div>
                </div>
                <div className="text-sm text-slate-400">
                  Showing {filteredExercises.length} of {EXERCISE_DATABASE.length}
                </div>
              </div>
            </CardHeader>
            <CardContent className="p-4 sm:p-6">
              {/* Search and Filter Controls */}
              <div className="space-y-4 sm:space-y-6 mb-6">
                {/* Search Bar */}
                <div className="relative">
                  <Search className="absolute left-3 top-1/2 transform -translate-y-1/2 w-4 h-4 sm:w-5 sm:h-5 text-slate-400" />
                  <Input
                      type="text"
                      placeholder="Search exercises, muscle groups, or equipment..."
                      value={exerciseSearchTerm}
                      onChange={(e) => setExerciseSearchTerm(e.target.value)}
                      className="pl-10 sm:pl-12 bg-slate-700/50 border-slate-600 text-white focus:border-blue-500 focus:ring-2 focus:ring-blue-500/20 transition-all h-12 sm:h-14 text-base"
                  />
                </div>

                {/* Filters */}
                <div className="flex flex-wrap gap-3 sm:gap-4">
                  <div className="flex items-center gap-2">
                    <Filter className="w-4 h-4 sm:w-5 sm:h-5 text-slate-400" />
                    <span className="text-sm text-slate-300 font-medium">Filters:</span>
                  </div>

                  {/* Muscle Group Filter */}
                  <select
                      value={selectedMuscleFilter}
                      onChange={(e) => setSelectedMuscleFilter(e.target.value)}
                      className="bg-slate-700/50 border border-slate-600 text-white text-sm rounded-lg px-3 py-2 sm:py-3 focus:border-blue-500 focus:ring-2 focus:ring-blue-500/20 transition-all min-w-0 flex-1 sm:flex-none touch-manipulation"
                  >
                    {uniqueMuscleGroups.map(group => (
                        <option key={group} value={group}>{group === 'All' ? 'All Muscles' : group}</option>
                    ))}
                  </select>

                  {/* Equipment Filter */}
                  <select
                      value={selectedEquipmentFilter}
                      onChange={(e) => setSelectedEquipmentFilter(e.target.value)}
                      className="bg-slate-700/50 border border-slate-600 text-white text-sm rounded-lg px-3 py-2 sm:py-3 focus:border-blue-500 focus:ring-2 focus:ring-blue-500/20 transition-all min-w-0 flex-1 sm:flex-none touch-manipulation"
                  >
                    {uniqueEquipmentTypes.map(equipment => (
                        <option key={equipment} value={equipment}>{equipment === 'All' ? 'All Equipment' : equipment}</option>
                    ))}
                  </select>

                  {/* Clear Filters */}
                  {(exerciseSearchTerm || selectedMuscleFilter !== 'All' || selectedEquipmentFilter !== 'All') && (
                      <Button
                          onClick={() => {
                            setExerciseSearchTerm('')
                            setSelectedMuscleFilter('All')
                            setSelectedEquipmentFilter('All')
                          }}
                          variant="outline"
                          size="sm"
                          className="border-slate-600 hover:border-red-500 text-slate-300 hover:text-red-400 transition-colors px-4 py-2 sm:py-3 touch-manipulation"
                      >
                        <X className="w-3 h-3 mr-1" />
                        Clear
                      </Button>
                  )}
                </div>
              </div>

              {/* Exercise Grid */}
              {filteredExercises.length > 0 ? (
                  <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-4 sm:gap-6 max-h-96 overflow-y-auto">
                    {filteredExercises.map((exercise) => {
                      const getMuscleIcon = (muscle: string) => {
                        switch(muscle) {
                          case 'Chest': return '💪'
                          case 'Back': return '🏋️'
                          case 'Shoulders': return '🤸'
                          case 'Arms': return '💪'
                          case 'Legs': return '🦵'
                          case 'Core': return '🔥'
                          case 'Cardio': return '❤️'
                          default: return '💪'
                        }
                      }

                      const getMuscleColor = (muscle: string) => {
                        switch(muscle) {
                          case 'Chest': return 'from-red-500 to-pink-500'
                          case 'Back': return 'from-green-500 to-emerald-500'
                          case 'Shoulders': return 'from-yellow-500 to-orange-500'
                          case 'Arms': return 'from-purple-500 to-indigo-500'
                          case 'Legs': return 'from-blue-500 to-cyan-500'
                          case 'Core': return 'from-orange-500 to-red-500'
                          case 'Cardio': return 'from-pink-500 to-rose-500'
                          default: return 'from-slate-500 to-slate-600'
                        }
                      }

                      return (
                          <div key={exercise.id} className="bg-slate-700/30 rounded-lg p-4 sm:p-5 border border-slate-600/50 hover:border-blue-500/50 transition-all duration-200 group touch-manipulation">
                            <div className="flex items-start justify-between mb-3">
                              <div className="flex items-center gap-2">
                                <span className="text-lg sm:text-xl">{getMuscleIcon(exercise.muscleGroup)}</span>
                                <div className={`px-2 py-1 rounded-full text-xs font-medium text-white bg-gradient-to-r ${getMuscleColor(exercise.muscleGroup)}`}>
                                  {exercise.muscleGroup}
                                </div>
                              </div>
                              <button
                                  onClick={() => setShowExerciseDetails(prev => ({ ...prev, [exercise.id]: !prev[exercise.id] }))}
                                  className="text-slate-400 hover:text-blue-400 transition-colors p-2 -m-2 touch-manipulation"
                              >
                                {showExerciseDetails[exercise.id] ? <ChevronUp className="w-4 h-4 sm:w-5 sm:h-5" /> : <ChevronDown className="w-4 h-4 sm:w-5 sm:h-5" />}
                              </button>
                            </div>

                            <h4 className="text-white font-medium text-base sm:text-lg mb-2 group-hover:text-blue-400 transition-colors">{exercise.name}</h4>

                            <div className="text-xs sm:text-sm text-slate-400">
                              <span className="font-medium">Equipment:</span> {exercise.equipment}
                            </div>

                            {showExerciseDetails[exercise.id] && (
                                <div className="mt-3 pt-3 border-t border-slate-600/50 space-y-2 animate-in slide-in-from-top-2 duration-200">
                                  {exercise.sets && (
                                      <div className="text-xs text-slate-300">
                                        <span className="font-medium">Sets:</span> {exercise.sets}
                                      </div>
                                  )}
                                  {exercise.reps && (
                                      <div className="text-xs text-slate-300">
                                        <span className="font-medium">Reps:</span> {exercise.reps}
                                      </div>
                                  )}
                                  {exercise.restTime && (
                                      <div className="text-xs text-slate-300">
                                        <span className="font-medium">Rest:</span> {exercise.restTime}
                                      </div>
                                  )}
                                </div>
                            )}
                          </div>
                      )
                    })}
                  </div>
              ) : (
                  <div className="text-center py-12 text-slate-400">
                    <Search className="w-12 h-12 mx-auto mb-4 opacity-50" />
                    <p className="text-lg font-medium mb-2">No exercises found</p>
                    <p className="text-sm">Try adjusting your search terms or filters</p>
                  </div>
              )}
            </CardContent>
          </Card>
        </main>
      </div>
  )
}
