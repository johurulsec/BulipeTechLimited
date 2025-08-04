// import { createRoot } from 'react-dom/client'
// import './index.css'
// import App from './App.jsx'

// createRoot(document.getElementById('root')).render(
//     <App />
// )

//main.jsx
// Redirect console logs to Qt bridge
(function () {
  const originalLog = console.log;
  console.log = function (...args) {
    originalLog(...args); // Keep browser log

    try {
      const message = args.map(arg => typeof arg === 'string' ? arg : JSON.stringify(arg)).join(' ');
      if (window.bridge?.log) {
        window.bridge.log(message);
      }
    } catch (e) {
      originalLog("Error sending log to Qt:", e);
    }
  };
})();

// Add more if you want full logging (optional)
["warn", "error"].forEach((level) => {
  const original = console[level];
  console[level] = function (...args) {
    original(...args);
    try {
      const msg = args.map(arg => typeof arg === "string" ? arg : JSON.stringify(arg)).join(" ");
      window.bridge?.log?.(`[${level.toUpperCase()}] ${msg}`);
    } catch (e) {}
  };
});

console.log("✅ main.jsx loaded");

import { createRoot } from 'react-dom/client'
import './index.css'
import App from './App.jsx'

const rootEl = document.getElementById('root');
console.log("✅ Root element:", rootEl);

createRoot(rootEl).render(<App />);
console.log("✅ App rendered");

if (typeof QWebChannel === 'undefined') {
    console.error("QWebChannel is not defined — script failed to load.");
} else {
    console.log("✅ QWebChannel loaded.");
}
