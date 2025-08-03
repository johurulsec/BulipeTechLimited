// import { createRoot } from 'react-dom/client'
// import './index.css'
// import App from './App.jsx'

// createRoot(document.getElementById('root')).render(
//     <App />
// )

//main.jsx
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
