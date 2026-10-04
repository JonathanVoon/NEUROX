import { useState } from 'react'
import { LineChart, Line, XAxis, YAxis, Tooltip, CartesianGrid, ResponsiveContainer } from 'recharts'
import { clinicPatients, romTrend } from '../data/mockData.js'

function StatBlock({ label, value }) {
  return (
    <div>
      <div className="font-display text-2xl font-semibold text-ink">{value}</div>
      <div className="text-xs text-slate mt-1">{label}</div>
    </div>
  )
}

function PatientRow({ patient, isActive, onSelect }) {
  const isStagnating = patient.status === 'stagnating'
  return (
    <button
      onClick={() => onSelect(patient.id)}
      className={`w-full text-left flex items-center justify-between py-3 px-3 rounded-lg transition-colors ${
        isActive ? 'bg-teal-pale' : 'hover:bg-paper'
      }`}
    >
      <div>
        <div className="text-sm font-medium text-ink">{patient.name}</div>
        <div className="text-xs text-slate">Last session {patient.lastSession}</div>
      </div>
      {isStagnating && (
        <span className="text-xs font-medium px-2 py-1 rounded-full bg-amber-light text-ink">
          Stagnating
        </span>
      )}
    </button>
  )
}

export default function DashboardView() {
  const [selectedId, setSelectedId] = useState(clinicPatients[0].id)
  const selectedPatient = clinicPatients.find((p) => p.id === selectedId)

  return (
    <div>
      <p className="text-sm text-slate mb-1">Clinic overview</p>
      <h1 className="font-display text-3xl font-semibold text-ink mb-8">Patient Dashboard</h1>

      <div className="grid grid-cols-1 lg:grid-cols-[280px_1fr] gap-8">
        {/* Patient list */}
        <aside>
          <h3 className="text-sm font-medium text-slate mb-2">Patients</h3>
          <div className="flex flex-col gap-1">
            {clinicPatients.map((patient) => (
              <PatientRow
                key={patient.id}
                patient={patient}
                isActive={patient.id === selectedId}
                onSelect={setSelectedId}
              />
            ))}
          </div>
        </aside>

        {/* Selected patient detail */}
        <section>
          {selectedPatient.status === 'stagnating' && (
            <div className="mb-6 border border-amber bg-amber-light/30 rounded-lg px-4 py-3 text-sm text-ink">
              <span className="font-medium">Stagnation alert:</span> {selectedPatient.name}'s range-of-motion
              score hasn't improved over the last 4 sessions. Consider reviewing their exercise targets.
            </div>
          )}

          <div className="flex gap-10 mb-8">
            <StatBlock label="Completion rate" value={`${Math.round(selectedPatient.completionRate * 100)}%`} />
            <StatBlock label="Sessions logged" value={romTrend.length} />
            <StatBlock label="Latest ROM score" value={romTrend[romTrend.length - 1].romScore} />
          </div>

          <h3 className="text-sm font-medium text-slate mb-3">Range-of-motion trend</h3>
          <div className="h-64 border border-teal-pale rounded-xl p-4">
            <ResponsiveContainer width="100%" height="100%">
              <LineChart data={romTrend}>
                <CartesianGrid stroke="#CFE0D6" strokeDasharray="3 3" />
                <XAxis dataKey="session" tick={{ fontSize: 12, fill: '#64726F' }} />
                <YAxis tick={{ fontSize: 12, fill: '#64726F' }} />
                <Tooltip />
                <Line type="monotone" dataKey="romScore" stroke="#12514E" strokeWidth={2} dot={{ r: 3 }} />
              </LineChart>
            </ResponsiveContainer>
          </div>
        </section>
      </div>
    </div>
  )
}
