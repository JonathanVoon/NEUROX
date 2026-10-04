import { NavLink, Routes, Route, Navigate } from 'react-router-dom'
import PatientView from './pages/PatientView.jsx'
import DashboardView from './pages/DashboardView.jsx'

const navItems = [
  { to: '/patient', label: 'My Rehab' },
  { to: '/dashboard', label: 'Clinic Dashboard' },
]

function NavIcon({ label }) {
  // Small text-based marker instead of an icon library, keeps this dependency-free
  return (
    <span className="inline-flex h-2 w-2 rounded-full bg-current mr-2 align-middle" aria-hidden="true" />
  )
}

export default function App() {
  return (
    <div className="min-h-screen font-body flex flex-col md:flex-row">
      {/* Desktop sidebar */}
      <aside className="hidden md:flex md:flex-col md:w-56 md:min-h-screen border-r border-teal-pale bg-white px-5 py-8">
        <div className="font-display text-2xl font-semibold text-teal mb-10">NEUROX</div>
        <nav className="flex flex-col gap-1">
          {navItems.map((item) => (
            <NavLink
              key={item.to}
              to={item.to}
              className={({ isActive }) =>
                `flex items-center px-3 py-2 rounded-md text-sm font-medium transition-colors ${
                  isActive ? 'bg-teal-pale text-teal' : 'text-slate hover:bg-paper'
                }`
              }
            >
              <NavIcon label={item.label} />
              {item.label}
            </NavLink>
          ))}
        </nav>
      </aside>

      {/* Mobile top bar */}
      <header className="md:hidden flex items-center justify-between px-4 py-4 border-b border-teal-pale bg-white">
        <span className="font-display text-xl font-semibold text-teal">NEUROX</span>
      </header>

      <main className="flex-1 px-4 py-6 md:px-10 md:py-10 pb-24 md:pb-10">
        <Routes>
          <Route path="/" element={<Navigate to="/patient" replace />} />
          <Route path="/patient" element={<PatientView />} />
          <Route path="/dashboard" element={<DashboardView />} />
        </Routes>
      </main>

      {/* Mobile bottom tab bar - this is the shape Capacitor will wrap later */}
      <nav className="md:hidden fixed bottom-0 left-0 right-0 bg-white border-t border-teal-pale flex">
        {navItems.map((item) => (
          <NavLink
            key={item.to}
            to={item.to}
            className={({ isActive }) =>
              `flex-1 text-center py-3 text-xs font-medium ${
                isActive ? 'text-teal' : 'text-slate'
              }`
            }
          >
            {item.label}
          </NavLink>
        ))}
      </nav>
    </div>
  )
}
