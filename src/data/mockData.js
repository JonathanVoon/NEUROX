// This file is a stand-in for real data. Once the Firestore schema is agreed
// with the hardware team (see step 5 of the build plan), replace the
// functions below with real Firestore reads - the shape of the data
// returned should stay the same so the UI components don't need to change.

export const currentPatient = {
  name: 'Amara Lee',
  streakDays: 6,
  weeklyGoalSessions: 5,
  weeklySessionsDone: 3,
  badges: ['First Session', '5-Day Streak', 'Full Range Reached'],
}

export const recentSessions = [
  { date: '2026-09-21', durationMin: 22, romScore: 61, completed: true },
  { date: '2026-09-22', durationMin: 24, romScore: 64, completed: true },
  { date: '2026-09-24', durationMin: 20, romScore: 63, completed: true },
  { date: '2026-09-25', durationMin: 0, romScore: null, completed: false },
  { date: '2026-09-26', durationMin: 25, romScore: 68, completed: true },
]

// Range-of-motion trend across the last 14 sessions, for the physio dashboard chart
export const romTrend = [
  { session: 1, romScore: 42 },
  { session: 2, romScore: 45 },
  { session: 3, romScore: 47 },
  { session: 4, romScore: 46 },
  { session: 5, romScore: 51 },
  { session: 6, romScore: 53 },
  { session: 7, romScore: 55 },
  { session: 8, romScore: 54 },
  { session: 9, romScore: 58 },
  { session: 10, romScore: 61 },
  { session: 11, romScore: 64 },
  { session: 12, romScore: 63 },
  { session: 13, romScore: 66 },
  { session: 14, romScore: 68 },
]

export const clinicPatients = [
  {
    id: 'p1',
    name: 'Amara Lee',
    completionRate: 0.86,
    lastSession: '2026-09-26',
    status: 'on-track',
  },
  {
    id: 'p2',
    name: 'Farid Hassan',
    completionRate: 0.52,
    lastSession: '2026-09-20',
    status: 'stagnating',
  },
  {
    id: 'p3',
    name: 'Wei Ling Chan',
    completionRate: 0.94,
    lastSession: '2026-09-27',
    status: 'on-track',
  },
]
