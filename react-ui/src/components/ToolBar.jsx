// import TextField from "@mui/material/TextField";
// import InputAdornment from "@mui/material/InputAdornment";
// import { MdSearch } from "react-icons/md";
// import Toolbar from "@mui/material/Toolbar";
// import { IoMdArrowBack, IoMdArrowForward } from "react-icons/io";
// import { IoReload } from "react-icons/io5";
// import IconButton from "@mui/material/IconButton";
// import Box from "@mui/material/Box";
// import { Button } from "@mui/material";
// const ToolBar = () => {
//     return (
//         <div>
//             <Toolbar
//                 sx={{
//                     backgroundColor: "#f5f5f5",
//                     borderBottom: 1,
//                     borderColor: "divider",
//                     display: "flex",
//                     justifyContent: "space-between",
//                     gap:"10px"
//                 }}
//             >
//                 <Box sx={{ display: "flex", alignItems: "center" }}>
//                     <IconButton size="small">
//                         <IoMdArrowBack />
//                     </IconButton>
//                     <IconButton size="small">
//                         <IoReload />
//                     </IconButton>
//                     <IconButton size="small">
//                         <IoMdArrowForward />
//                     </IconButton>
//                 </Box>
//                 <TextField
//                     placeholder="enter address..."
//                     variant="outlined"
//                     size="small"
//                     sx={{ width: "500px" }}
//                     InputProps={{
//                         startAdornment: (
//                             <InputAdornment position="start">
//                                 <MdSearch />
//                             </InputAdornment>
//                         ),
//                     }}
//                 />
//                 <Button variant="contained" sx={{ px: 5 }}>
//                     Menus
//                 </Button>
//             </Toolbar>
//         </div>
//     );
// };

// export default ToolBar;

import Toolbar from "@mui/material/Toolbar";
import SearchBar from "../pages/Home/SearhcBar/TopSearchBar";
import { useState } from "react";
import { Box, Button, IconButton, TextField } from "@mui/material";
import { IoMdArrowBack, IoMdArrowForward } from "react-icons/io";
import { IoReload } from "react-icons/io5";

const ToolBar = ({ onLoadUrl }) => {
    const [inputUrl, setInputUrl] = useState("");

    const handleKeyDown = (e) => {
        if (e.key === "Enter") {
            let formatted = inputUrl.trim();
            if (!formatted.startsWith("http")) {
                formatted = "https://" + formatted;
            }
            onLoadUrl(formatted); // pass to App
            setInputUrl(""); // clear input
        }
    };

    return (
        <Toolbar
            sx={{
                backgroundColor: "#f5f5f5",
                borderBottom: 1,
                borderColor: "divider",
                display: "flex",
                justifyContent: "space-between",
                gap: "10px",
            }}
        >
            <Box sx={{ display: "flex", alignItems: "center" }}>
                <IconButton size="small">
                    <IoMdArrowBack />
                </IconButton>
                <IconButton size="small">
                    <IoReload />
                </IconButton>
                <IconButton size="small">
                    <IoMdArrowForward />
                </IconButton>
            </Box>
            <TextField
                placeholder="Search or enter address"
                variant="outlined"
                size="small"
                sx={{ width: "300px" }}
                value={inputUrl}
                onChange={(e) => setInputUrl(e.target.value)}
                onKeyDown={handleKeyDown}
            />
            <Button variant="contained" sx={{ px: 5 }}>
                Menus
            </Button>
        </Toolbar>
    );
};

export default ToolBar;
