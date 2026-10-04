/** @type {import('tailwindcss').Config} */
export default {
  content: ['./index.html', './src/**/*.{js,jsx}'],
  theme: {
    extend: {
      colors: {
        ink: '#1B2A28',
        teal: {
          DEFAULT: '#12514E',
          light: '#1C6E69',
          pale: '#CFE0D6',
        },
        amber: {
          DEFAULT: '#E2914F',
          light: '#F0B98A',
        },
        slate: {
          DEFAULT: '#64726F',
        },
        paper: '#F6F4EF',
      },
      fontFamily: {
        display: ['Fraunces', 'Georgia', 'serif'],
        body: ['Inter', 'system-ui', 'sans-serif'],
      },
    },
  },
  plugins: [],
}
