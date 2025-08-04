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

//ToolBar.jsx
import Toolbar from "@mui/material/Toolbar";
import SearchBar from "../pages/Home/SearhcBar/SearchBar";
import { useState } from "react";
import {
  Box,
  Button,
  IconButton,
  TextField,
  InputAdornment,
} from "@mui/material";
import { IoMdArrowBack, IoMdArrowForward } from "react-icons/io";
import { IoReload } from "react-icons/io5";
import { MdSearch } from "react-icons/md";

const ToolBar = ({ onLoadUrl }) => {
  const [inputUrl, setInputUrl] = useState("");

  const handleKeyDown = (e) => {
    console.log("handleKeyDown() of toolBar.jsx");

    if (e.key === "Enter") {
      let formatted = inputUrl.trim();
      console.log("before fomated-url of toolBar.jsx user-input:", formatted);

      if (!formatted.startsWith("http")) {
        formatted = "https://" + formatted;
      }
      console.log("after fomatted-url of toolBar.jsx:", formatted);

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
        {/* <IconButton size="small">
          <IoMdArrowBack />
        </IconButton> */}

        <IconButton size="small" onClick={() => window.bridge?.goBack()}>
          <IoMdArrowBack />
        </IconButton>

        {/* <IconButton size="small">
          <IoReload />
        </IconButton>
        <IconButton size="small">
          <IoMdArrowForward />
        </IconButton> */}

        <IconButton size="small" onClick={() => window.bridge?.reload()}>
          <IoReload />
        </IconButton>
        <IconButton size="small" onClick={() => window.bridge?.goForward()}>
          <IoMdArrowForward />
        </IconButton>
      </Box>
      <TextField
        placeholder="enter address..."
        variant="outlined"
        size="small"
        fullWidth
        sx={{
          width: {
            xs: "100%", // mobile
            sm: "400px", // small screens
            md: "500px", // medium+
          },
        }}
        InputProps={{
          startAdornment: (
            <InputAdornment position="start">
              <MdSearch />
            </InputAdornment>
          ),
        }}
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
