import { currentPatient, recentSessions } from '../data/mockData.js'

function WeeklyProgress({ done, goal }) {
  const pct = Math.min(100, Math.round((done / goal) * 100))
  return (
    <div>
      <div className="flex items-baseline justify-between mb-2">
        <span className="text-sm font-medium text-slate">This week</span>
        <span className="text-sm font-medium text-ink">{done} of {goal} sessions</span>
      </div>
      <div className="h-2 w-full rounded-full bg-teal-pale overflow-hidden">
        <div
          className="h-full rounded-full bg-amber"
          style={{ width: `${pct}%` }}
        />
      </div>
    </div>
  )
}

function SessionRow({ session }) {
  const date = new Date(session.date)
  const label = date.toLocaleDateString(undefined, { weekday: 'short', month: 'short', day: 'numeric' })

  return (
    <div className="flex items-center justify-between py-3 border-b border-teal-pale last:border-b-0">
      <div>
        <div className="text-sm font-medium text-ink">{label}</div>
        <div className="text-xs text-slate">
          {session.completed ? `${session.durationMin} min · ROM score ${session.romScore}` : 'Session skipped'}
        </div>
      </div>
      <span
        className={`text-xs font-medium px-2 py-1 rounded-full ${
          session.completed ? 'bg-teal-pale text-teal' : 'bg-paper text-slate border border-teal-pale'
        }`}
      >
        {session.completed ? 'Done' : 'Missed'}
      </span>
    </div>
  )
}

export default function PatientView() {
  return (
    <div className="max-w-2xl">
      <p className="text-sm text-slate mb-1">Welcome back</p>
      <h1 className="font-display text-3xl font-semibold text-ink mb-8">{currentPatient.name}</h1>

      {/* Hero: today's session */}
      <section className="bg-teal text-white rounded-xl px-6 py-7 mb-8">
        <p className="text-sm text-teal-pale mb-1">{currentPatient.streakDays}-day streak</p>
        <h2 className="font-display text-xl font-semibold mb-4">Ready for today's session?</h2>
        <button className="bg-amber hover:bg-amber-light transition-colors text-ink font-semibold px-5 py-2.5 rounded-lg text-sm">
          Start session
        </button>
      </section>

      <section className="mb-8">
        <WeeklyProgress done={currentPatient.weeklySessionsDone} goal={currentPatient.weeklyGoalSessions} />
      </section>

      <section className="mb-8">
        <h3 className="text-sm font-medium text-slate mb-3">Badges earned</h3>
        <div className="flex flex-wrap gap-2">
          {currentPatient.badges.map((badge) => (
            <span
              key={badge}
              className="text-xs font-medium px-3 py-1.5 rounded-full border border-amber text-amber"
            >
              {badge}
            </span>
          ))}
        </div>
      </section>

      <section>
        <h3 className="text-sm font-medium text-slate mb-1">Recent sessions</h3>
        <div>
          {recentSessions
            .slice()
            .reverse()
            .map((session) => (
              <SessionRow key={session.date} session={session} />
            ))}
        </div>
      </section>
    </div>
  )
}
