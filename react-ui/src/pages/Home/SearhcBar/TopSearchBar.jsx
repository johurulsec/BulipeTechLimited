import { InputAdornment, TextField } from "@mui/material";
import { useState } from "react";
import { MdSearch } from "react-icons/md";

const TopSearchBar = () => {
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
                placeholder="Search with Google or Enter address"
                variant="outlined"
                size="small"
                sx={{ width: "300px" }}
                value={inputUrl}
                onChange={(e) => setInputUrl(e.target.value)}
                onKeyDown={handleKeyDown}
            />
        </div>
    );
};

export default TopSearchBar;
