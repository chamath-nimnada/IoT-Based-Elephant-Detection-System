import React, { useEffect, useState, useRef, useCallback } from 'react';
import axios from 'axios';
import { initializeApp } from 'firebase/app';
import { getDatabase, ref, onValue, set } from 'firebase/database';



import {
  GoogleMap,
  Marker,
  Polyline,
  Polygon,
  OverlayView,
  useJsApiLoader,
} from '@react-google-maps/api';

const firebaseConfig = {
   apiKey: "AIzaSyDDbwDuKX-tU_e4aVQhwq-ubuVm9Zvs5EM",
  authDomain: "iot-animal-detection-system.firebaseapp.com",
  databaseURL: "https://iot-animal-detection-system-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "iot-animal-detection-system",
  storageBucket: "iot-animal-detection-system.firebasestorage.app",
  messagingSenderId: "86859210246",
  appId: "1:86859210246:web:88bc9b2f69b8b855dd2d87"
};



const mapContainerStyle = {
  width: '100vw',
  height: '100vh',
};

const anuradhapuraBounds = {
  north: 8.436020071846132,
  south: 8.2468123865338,
  east: 80.53308329977378,
  west: 80.27353131409448,
};

// 🔴 Anuradhapura boundary polygon
const anuradhapuraBoundary = [
  { lat: 8.41072119656908, lng: 80.40315668122075 },
  { lat: 8.408135951323208, lng: 80.40723604600775 },
  { lat: 8.395430166009008, lng: 80.41195281229001 },
  { lat: 8.386003024365143, lng: 80.41523542631188 },
  { lat: 8.381115954501146, lng: 80.41826308009254 },
  { lat: 8.377426964245481, lng: 80.42055772309298 },
  { lat: 8.372350080742063, lng: 80.42456775570716 },
  { lat: 8.370275319154954, lng: 80.42544718701221 },
  { lat: 8.3697398950153, lng: 80.42828842661314 },
  { lat: 8.369606038865369, lng: 80.4281531294893 },
  { lat: 8.371212309627834, lng: 80.43140026046177 },
  { lat: 8.37134616522561, lng: 80.43437679718656 },
  { lat: 8.374491758524378, lng: 80.43586506554897 },
  { lat: 8.375294884654586, lng: 80.43796217096869 },
  { lat: 8.37395634018351, lng: 80.44154754475082 },
  { lat: 8.369204470139486, lng: 80.4455388099045 },
  { lat: 8.369338326427457, lng: 80.44391524441828 },
  { lat: 8.368133618179614, lng: 80.44438878435174 },
  { lat: 8.366795049089712, lng: 80.4442534872279 },
  { lat: 8.366192691498675, lng: 80.44520056709489 },
  { lat: 8.363247818759623, lng: 80.44865064375315 },
  { lat: 8.361190251825162, lng: 80.44083511130938 },
  { lat: 8.357070137516606, lng: 80.44124338148538 },
  { lat: 8.356585415329524, lng: 80.43871210687043 },
  { lat: 8.355535181857748, lng: 80.43814052873157 },
  { lat: 8.354081007767544, lng: 80.4388754149101 },
  { lat: 8.353847092174101, lng: 80.4421170505838 },
  { lat: 8.348681032104267, lng: 80.44311207053812 },
  { lat: 8.34603698891099, lng: 80.44483672058657 },
  { lat: 8.343747723015163, lng: 80.44703949138486 },
  { lat: 8.342032876661449, lng: 80.44931056521014 },
  { lat: 8.343069292694146, lng: 80.4525947110142 },
  { lat: 8.34387180578885, lng: 80.45396930822677 },
  { lat: 8.343829568298652, lng: 80.4539607703559 },
  { lat: 8.346135728611758, lng: 80.4551048451178 },
  { lat: 8.347749188001437, lng: 80.4566416619677 },
  { lat: 8.350422850735509, lng: 80.45659344736796 },
  { lat: 8.350835758995924, lng: 80.45819911790674 },
  { lat: 8.339221552169217, lng: 80.46761717887999 },
  { lat: 8.328346046124713, lng: 80.46493979493013 },
  { lat: 8.30723469395909, lng: 80.46433020147279 },
  { lat: 8.290043182733742, lng: 80.45457670034523 },
  { lat: 8.268929772369711, lng: 80.41769627420669 },
  { lat: 8.259819458346495, lng: 80.40460542838878 },
  { lat: 8.268921441313287, lng: 80.35383525486388 },
  { lat: 8.290765342660306, lng: 80.33727976349707 },
  { lat: 8.323892942794242, lng: 80.34721305831717 },
  { lat: 8.352285798732842, lng: 80.33764766330522 },
  { lat: 8.379948645983848, lng: 80.35641055352094 },
  { lat: 8.396327036557135, lng: 80.38584253817305 },
  { lat: 8.409092355933938, lng: 80.39396848703439 },
  { lat: 8.410901472274622, lng: 80.40280759743122 },
  { lat: 8.41072119656908, lng: 80.40315668122075 }
];

const observationLine = [
  { lat: 8.313882491505566, lng: 80.39035042072268 },
  { lat: 8.313631374694364, lng: 80.38982472573166 },
  { lat: 8.313263667644947, lng: 80.38912984154811 },
  { lat: 8.313094637210938, lng: 80.38880718464969 },
  { lat: 8.312872515815348, lng: 80.38847433494034 },
  { lat: 8.312604438120754, lng: 80.3881569667078 },
  { lat: 8.312052963429949, lng: 80.3876538219489 },
];

const towerLocations = [
  { id: 'B1', lat: 8.313571540120934, lng: 80.38971725199409 },
  { id: 'B2', lat: 8.313214391671801, lng: 80.38901297648759 },
  { id: 'B3', lat: 8.312953063323638, lng: 80.38856400079092 },
  { id: 'B4', lat: 8.312443472553896, lng: 80.3879829734859 },
  { id: 'B5', lat: 8.312286675252635, lng: 80.38782010974685 },
];



const Dashboard = () => {
  const [alertTowerId, setAlertTowerId] = useState(null);
  const [towerActive, setTowerActive] = useState(false); // 🔁 State for UI & Firebase

  const toggleTowerActivation = () => {
  const newStatus = !towerActive;
  setTowerActive(newStatus);

  const activationRef = ref(database, 'Tower-activation');
  set(activationRef, newStatus)
    .then(() => console.log(`📶 Tower status set to ${newStatus}`))
    .catch(err => console.error('❌ Error updating tower status:', err));
};


  const app = initializeApp(firebaseConfig); // ✅ Initialize Firebase App once
  const database = getDatabase(app);         // ✅ Initialize DB once

  const { isLoaded } = useJsApiLoader({
    googleMapsApiKey: 'AIzaSyDEkDcv-duYfwOVsToVd-alBi8sQHSrO4g',
  });

  useEffect(() => {
  try {
    console.log('✅ Firebase initialized and database connected.');
    
    const alertRef = ref(database, 'alerts');

    const unsubscribe = onValue(alertRef, (snapshot) => {
      const data = snapshot.val();
      console.log('📡 Firebase alert data:', data); // 🔍 Logs full snapshot

      const animalDetected = data?.['animal-Detected'];
      const towerId = data?.tower;

      console.log('🐘 Animal Detected:', animalDetected);
      console.log('📍 Tower ID:', towerId);

      if (animalDetected === true && towerId) {
        setAlertTowerId(towerId);
      } else {
        setAlertTowerId(null);
      }
    });

    return () => unsubscribe();
  } catch (err) {
    console.error('❌ Firebase connection error:', err);
  }
}, [database]);

useEffect(() => {
  const activationRef = ref(database, 'Tower-activation');
  const unsubscribe = onValue(activationRef, (snapshot) => {
    const active = snapshot.val();
    setTowerActive(!!active);
  });

  return () => unsubscribe();
}, []);



  const mapRef = useRef(null);
  const onLoad = useCallback((map) => {
    mapRef.current = map;
    const bounds = new window.google.maps.LatLngBounds();
    anuradhapuraBoundary.forEach(coord => bounds.extend(coord));
    map.fitBounds(bounds);
  }, []);

return (
  <div className="h-screen w-screen overflow-hidden flex flex-col">
    {/* HEADER */}
    <header className="bg-white shadow-md border-b border-gray-200 py-4 px-6">
      <h2 className="text-3xl font-bold text-center text-indigo-700">
        Anuradhapura District Boundary Map
      </h2>
    </header>

    {/* ANIMATION STYLE */}
   <style>
  {`
  

  .blink-ring {
      width: 60px;
      height: 60px;
      border: 5px solid yellow;
      border-radius: 50%;
      animation: blink 1s infinite ease-in-out;
      background-color: rgba(255, 255, 0, 0.15);
      box-shadow: 0 0 25px rgba(255, 255, 0, 0.8);
      transform: translate(-50%, -50%);
      position: absolute;
    }

    @keyframes blink {
      0% { transform: scale(0.7); opacity: 1; }
      50% { transform: scale(1.6); opacity: 0.25; }
      100% { transform: scale(0.7); opacity: 1; }
    }

    /* 📦 General Panel Layout */
    .panel-container {
      display: grid;
      gap: 2rem;
      padding: 2rem;
      height: 100vh;
      width: 100vw;
      box-sizing: border-box;
      overflow-y: auto;
      background-color: #f9fafb;
    }

    .card {
      background-color: white;
      border-radius: 1rem;
      box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
      padding: 2rem;
      width: 100%;
      max-width: 1200px;
      margin: 0 auto;
    }

    /* 🪄 Tower Activation Styles */
    .card-header {
      display: grid;
      grid-template-columns: 1fr auto;
      align-items: center;
      gap: 1rem;
    }

    .card-title {
      font-size: 2rem;
      font-weight: bold;
      color: #1f2937; /* Gray-800 */
    }

    .switch-wrapper {
      position: relative;
      display: inline-flex;
      align-items: center;
      cursor: pointer;
    }

    .switch-track {
      width: 96px;
      height: 48px;
      background-color: #d1d5db; /* gray-300 */
      border-radius: 9999px;
      transition: background-color 0.3s;
      position: relative;
    }

    .switch-thumb {
      content: '';
      position: absolute;
      top: 4px;
      left: 4px;
      width: 40px;
      height: 40px;
      background-color: white;
      border-radius: 9999px;
      box-shadow: 0 1px 4px rgba(0, 0, 0, 0.2);
      transition: transform 0.3s;
    }

    input[type="checkbox"]:checked + .switch-track {
      background-color: #10b981; /* green-500 */
    }

    input[type="checkbox"]:checked + .switch-track .switch-thumb {
      transform: translateX(48px);
    }

    .card-status {
      margin-top: 1.5rem;
      font-size: 1.25rem;
      color: #374151; /* Gray-700 */
    }

    .status-text {
      font-weight: bold;
    }

    .status-text.active {
      color: #10b981;
    }

    .status-text.inactive {
      color: #ef4444;
    }

    /* 🚆 Train Schedule Table */
    .schedule-title {
      font-size: 1.75rem;
      font-weight: bold;
      margin-bottom: 1.5rem;
      color: #1f2937;
    }

    .schedule-table {
      width: 100%;
      border-collapse: separate;
      border-spacing: 0 0.75rem;
      font-size: 1.1rem;
      color: #374151;
    }

    .schedule-table th {
      text-align: left;
      padding: 1rem;
      border-bottom: 2px solid #d1d5db;
    }

    .schedule-table td {
      padding: 1rem;
      background-color: white;
    }

    .status {
      font-weight: 600;
    }

    .on-time {
      color: #10b981;
    }

    .delayed {
      color: #f59e0b;
    }
  `}
</style>


    {/* MAIN CONTENT LAYOUT */}
    <div className="flex flex-grow overflow-hidden map">
      
      {/* LEFT: Google Map */}
      <div className="flex-grow map">
        {isLoaded ? (
          <GoogleMap
            onLoad={onLoad}
            mapContainerStyle={mapContainerStyle}
            mapTypeId="hybrid"
            options={{
              zoomControl: true,
              mapTypeControl: false,
              streetViewControl: false,
              fullscreenControl: false,
              restriction: {
                latLngBounds: anuradhapuraBounds,
                strictBounds: false,
              },
            }}
          >
            <Polygon
              path={anuradhapuraBoundary}
              options={{
                fillColor: '#000000',
                fillOpacity: 0.25,
                strokeColor: '#DC2626',
                strokeOpacity: 1,
                strokeWeight: 3,
                zIndex: 10,
                clickable: false,
              }}
            />

            <Polyline
              path={observationLine}
              options={{
                strokeColor: '#000000',
                strokeOpacity: 1,
                strokeWeight: 30,
              }}
            />

            {alertTowerId && (() => {
              const alertTower = towerLocations.find(t => t.id === alertTowerId);
              if (!alertTower) return null;
              return (
                <OverlayView
                  key="alert-blinker"
                  position={{ lat: alertTower.lat, lng: alertTower.lng }}
                  mapPaneName={OverlayView.OVERLAY_MOUSE_TARGET}
                >
                  <div className="blink-ring" />
                </OverlayView>
              );
            })()}

            {towerLocations.map((tower) => (
              <Marker
                key={tower.id}
                position={{ lat: tower.lat, lng: tower.lng }}
                label={{
                  text: tower.id,
                  color: '#FFFFFF',
                  fontWeight: 'bold',
                  fontSize: '14px',
                }}
                icon={{
                  path: window.google.maps.SymbolPath.CIRCLE,
                  fillColor: '#2563EB',
                  fillOpacity: 1,
                  strokeColor: '#FFFFFF',
                  strokeWeight: 1,
                  scale: 6,
                }}
                title={`Tower ${tower.id}`}
              />
            ))}
          </GoogleMap>
        ) : (
          <p className="text-center mt-4 text-lg">Loading map…</p>
        )}
      </div>
<div class="panel-container">
  
  <div class="card tower-card">
    <div class="card-header">
      <span class="card-title">Tower Activation</span>
      <label class="switch-wrapper">
        <input type="checkbox" id="towerToggle" class="sr-only" />
        <div class="switch-track">
          <span class="switch-thumb"></span>
        </div>
      </label>
    </div>
    <p class="card-status">
      Status:
      <span id="towerStatus" class="status-text">Deactivated</span>
    </p>
  </div>


  <div class="card schedule-card">
    <h3 class="schedule-title">Train Schedule</h3>
    <table class="schedule-table">
      <thead>
        <tr>
          <th>Train</th>
          <th>Time</th>
          <th>Status</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>Yal Devi</td>
          <td>10:15 AM</td>
          <td class="status on-time">On Time</td>
        </tr>
        <tr>
          <td>Uttara Devi</td>
          <td>12:45 PM</td>
          <td class="status delayed">Delayed</td>
        </tr>
        <tr>
          <td>Rajarata Rejini</td>
          <td>03:00 PM</td>
          <td class="status on-time">On Time</td>
        </tr>
      </tbody>
    </table>
  </div>
</div>

    </div>
  </div>
);
};


export default Dashboard;