//Home.jsx
import { Box } from "@mui/material";
import bgImage from "../../assets/Home.png";
import logo from "../../assets/logo.png";

const Home = ({ url, onLoadUrl }) => {
    console.log("Home() of home.jsx");
    
    return (
        <div
            id="webview-slot" // critical ID for QWebEngineView positioning
            style={{ backgroundImage: `url(${bgImage})`,}}
            className="home bg-cover bg-center h-screen flex items-center justify-center"
        >
            {!url && (
                <Box sx={{ mt: -20 }} className="flex flex-col items-center content">
                    <img src={logo} alt="logo" />
                </Box>
                
            )}
        </div>
    );
};

export default Home;

// import Shortcuts from "./Shortcuts/Shortcuts";
//import SearchBar from "./SearhcBar/SearchBar";

// const Home = ({ url,onLoadUrl }) => {
//     return (
//         <div
//             style={{ backgroundImage: `url(${bgImage})` }}
//             className="home bg-cover bg-center h-screen flex items-center justify-center"
//         >
//             {url ? (
//                 <iframe
//                     src={url}
//                     style={{ width: "100%", height: "100%", border: "none" }}
//                     title="BrowserView"

//                     sandbox="allow-same-origin allow-scripts allow-popups allow-forms"
//                     referrerPolicy="no-referrer-when-downgrade"
//                 />
//             ) : (
//                 <Box sx={{ mt: -20 }} className="flex flex-col items-center content">
//                     <img src={logo} alt="" />
//                     <SearchBar onLoadUrl={onLoadUrl}/>
//                     {/* <div className="shortcut flex items-center justify-center">
//                         <Shortcuts />
//                     </div> */}
//                 </Box>
//             )}            

//         </div>
//     );
// };

// export default Home;
