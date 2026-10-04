# NEUROX - starter web app

A starting point for the Patient view + Physiotherapist dashboard from the
NEUROX proposal. Everything here runs on fake data in `src/data/mockData.js`
so you can see it working immediately, before Firebase or the hardware team's
data feed is ready.

## Run it

```bash
npm install
npm run dev
```

Then open the URL it prints (usually http://localhost:5173). Resize your
browser window down to phone width to see the mobile layout (bottom tab bar
instead of the sidebar) - this is the layout Capacitor will wrap later.

## What's here

- `/patient` - the patient-facing screen: streak, start-session button,
  weekly progress, badges, recent sessions.
- `/dashboard` - the physiotherapist screen: patient list, stagnation alert,
  completion rate, and a range-of-motion trend chart (Recharts).
- `src/data/mockData.js` - all the fake data. Replace this file's contents
  with real Firestore reads once your backend is set up; keep the same
  shape (field names) so the page components don't need to change.

## Next steps (matches the build plan)

1. Tweak the mock data / copy to match your real content.
2. Set up a Firebase project (Firestore + Auth).
3. Agree the data schema with whoever's coding the ESP32 firmware - the
   field names in `mockData.js` are a reasonable starting proposal
   (sessionId, romScore, completed, etc.) but confirm before building on them.
4. Swap the imports in `PatientView.jsx` / `DashboardView.jsx` from
   `mockData.js` to real Firestore queries.
5. Add Firebase Auth so a patient login lands on `/patient` and a
   physiotherapist login lands on `/dashboard`.
6. Deploy with Firebase Hosting (`firebase deploy`) to get a shareable link.
7. Once it looks right at phone width, wrap it with Capacitor for an
   installable mobile app.

## Design notes

Palette is teal/ink/paper with an amber accent - deliberately not the
generic cream-and-terracotta or dark-mode-neon look, and picked to read as
calm and clinical rather than sterile. Headings use Fraunces (a warm serif),
body/UI text uses Inter. Feel free to swap fonts/colors in
`tailwind.config.js` - they're all defined as named tokens there, not
scattered through the components.
