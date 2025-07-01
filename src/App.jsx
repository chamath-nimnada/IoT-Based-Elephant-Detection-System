import { useState } from 'react';
import reactLogo from './assets/react.svg';
import viteLogo from '/vite.svg';
import './App.css';
import Dashboard from './Components/Dashboard'; // 👈 Import your Dashboard component

function App() {
  const [count, setCount] = useState(0);

  return (
    <>
      
      <div className="mt-10">
        <Dashboard />
      </div>
    </>
  );
}

export default App;
