import React from "react";
import IconButton from "@mui/material/IconButton";
import CloseIcon from "@mui/icons-material/Close";
import CropSquareIcon from "@mui/icons-material/CropSquare"; // Maximize
import RemoveIcon from "@mui/icons-material/Remove"; // Minimize
import Box from "@mui/material/Box";

export default function WindowControls() {
	const handleMinimize = () => {
		window.bridge?.minimizeWindow?.();
		console.log("qt fn: ",window.bridge?.minimizeWindow?.());
		
	};

	const handleMaximize = () => {
		window.bridge?.maximizeWindow?.();
	};

	const handleClose = () => {
		window.bridge?.closeWindow?.();
	};

	return (
		<Box sx={{ display: "flex", alignItems: "center" }}>
			<IconButton size="small" onClick={handleMinimize}>
				<RemoveIcon fontSize="small" />
			</IconButton>
			<IconButton size="small" onClick={handleMaximize}>
				<CropSquareIcon fontSize="small" />
			</IconButton>
			<IconButton size="small" onClick={handleClose}>
				<CloseIcon fontSize="small" />
			</IconButton>
		</Box>
	);
}
