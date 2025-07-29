import { InputAdornment, TextField } from "@mui/material";
import { useState } from "react";

const SearchBar = ({ onLoadUrl }) => {
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
        <div className="searchBar">
            <TextField
                placeholder="Search or enter address"
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

export default SearchBar;
