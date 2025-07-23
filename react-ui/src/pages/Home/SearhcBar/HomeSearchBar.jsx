import { InputAdornment, TextField } from "@mui/material";
import { useState } from "react";
import { MdSearch } from "react-icons/md";

const HomeSearchBar = () => {
    const [inputUrl, setInputUrl] = useState("");

    const handleKeyDown = (e) => {
        if (e.key === "Enter") {
            if (window.bridge?.loadUrl) {
                let formatted = inputUrl.trim();

                // If not a valid URL, prepend https://
                if (!formatted.startsWith("http")) {
                    formatted = "https://" + formatted;
                }

                window.bridge.loadUrl(formatted);
            } else {
                console.warn("window.bridge.loadUrl not available");
            }
        }
    };
    return (
        <div className="searchBar">
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
                onChange={e=>setInputUrl(e.target.value)}
                onKeyDown={handleKeyDown}
            />
        </div>
    );
};

export default HomeSearchBar;
