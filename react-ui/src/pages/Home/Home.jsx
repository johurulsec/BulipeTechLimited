import { Box } from "@mui/material";
import bgImage from "../../assets/Home.png";
import logo from "../../assets/logo.png";
import Shortcuts from "./Shortcuts/Shortcuts";
import SearchBar from "./SearhcBar/TopSearchBar";
import HomeSearchBar from "./SearhcBar/HomeSearchBar";
import TopSearchBar from "./SearhcBar/TopSearchBar";

const Home = ({ url }) => {
    return (
        <div
            style={{ backgroundImage: `url(${bgImage})` }}
            className="home bg-cover bg-center h-screen flex items-center justify-center"
        >
            {url ? (
                <iframe
                    src={url}
                    style={{ width: "100%", height: "100%", border: "none" }}
                    title="BrowserView"
                />
            ) : (
                <Box sx={{ mt: -20 }} className="flex flex-col items-center content">
                    <img src={logo} alt="" />
                    <TopSearchBar/>
                    <div className="shortcut flex items-center justify-center">
                        <Shortcuts />
                    </div>
                </Box>
            )}
        </div>
    );
};

export default Home;
