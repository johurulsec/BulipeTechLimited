// App.jsx
import React, { useState } from "react";
import PropTypes from "prop-types";
import Tabs from "@mui/material/Tabs";
import Tab from "@mui/material/Tab";
import Box from "@mui/material/Box";
import IconButton from "@mui/material/IconButton";
import CloseIcon from "@mui/icons-material/Close";
import ToolBar from "./components/ToolBar";
import Home from "./pages/Home/Home";
import WindowControls from "./components/WindowControls";
import logo from "./assets/logo.png";
import "./index.css";

function CustomTabPanel(props) {
    const { children, value, index, ...other } = props;
    return (
        <div
            role="tabpanel"
            hidden={value !== index}
            id={`simple-tabpanel-${index}`}
            {...other}
        >
            {value === index && children}
        </div>
    );
}

CustomTabPanel.propTypes = {
    children: PropTypes.node,
    index: PropTypes.number.isRequired,
    value: PropTypes.number.isRequired,
};

export default function BasicTabs() {
    const [tabs, setTabs] = useState([{ url: "" }]);
    const [value, setValue] = useState(0);

    React.useEffect(() => {
        if (typeof QWebChannel === "undefined") {
            console.error(
                "QWebChannel is not defined — make sure qwebchannel.js is loaded!"
            );
            return;
        }
        new QWebChannel(qt.webChannelTransport, (channel) => {
            window.bridge = channel.objects.bridge;

            window.bridge.updateTitle.connect((title) => {
                console.log("Page title changed:", title);
                setTabs((prevTabs) =>
                    prevTabs.map((tab, index) =>
                        index === value ? { ...tab, label: title } : tab
                    )
                );
            });
        });
    }, [value, setTabs]);

    const handleLoadUrl = (url) => {
        setTabs((prev) =>
            prev.map((tab, i) => (i === value ? { ...tab, url } : tab))
        );
    };

    const handleChange = (event, newValue) => {
        if (newValue === tabs.length) {
            handleAddTab();
        } else {
            setValue(newValue);
        }
    };

    const handleAddTab = () => {
        const newIndex = tabs.length;
        setTabs((prev) => [...prev, { url: "" }]);
        setValue(newIndex);
    };

    const handleCloseTab = (indexToClose) => {
        if (tabs.length === 1) return;

        setTabs((prevTabs) => {
            const newTabs = prevTabs.filter((_, i) => i !== indexToClose);
            if (value >= newTabs.length) {
                setValue(newTabs.length - 1);
            }
            return newTabs;
        });
    };

    return (
        <Box sx={{ width: "100%" }}>
            <Box
                sx={{
                    display: "flex",
                    alignItems: "center",
                    justifyContent: "space-between",
                    borderBottom: 1,
                    borderColor: "divider",
                    backgroundColor: "#f5f5f5",
                }}
            >
                <Tabs
                    value={value}
                    onChange={handleChange}
                    variant="scrollable"
                    scrollButtons="auto"
                    sx={{ flexGrow: 1 }}
                >
                    {tabs.map((tab, index) => (
                        <Tab
                            key={index}
                            label={
                                <Box
                                    sx={{
                                        display: "flex",
                                        alignItems: "center",
                                    }}
                                >
                                    <img
                                        src={logo}
                                        alt="logo"
                                        style={{
                                            width: 20,
                                            height: 20,
                                            marginRight: 6,
                                        }}
                                    />
                                    <span style={{ fontSize: "0.75rem" }}>
                                        New Tab
                                    </span>
                                    <Box
                                        component="span"
                                        onClick={(e) => {
                                            e.stopPropagation();
                                            handleCloseTab(index);
                                        }}
                                        sx={{
                                            ml: 1,
                                            cursor: "pointer",
                                            borderRadius: "50%",
                                            padding: "2px",
                                            "&:hover": {
                                                backgroundColor: "#f0f0f0",
                                            },
                                        }}
                                    >
                                        <CloseIcon fontSize="small" />
                                    </Box>
                                </Box>
                            }
                        />
                    ))}
                    <Tab label="+" />
                </Tabs>

                <Box sx={{ flexShrink: 0, pr: 1 }}>
                    <WindowControls />
                </Box>
            </Box>

            <ToolBar onLoadUrl={handleLoadUrl} />

            {tabs.map((tab, index) => (
                <CustomTabPanel key={index} value={value} index={index}>
                    {!tab.url && <Home />}
                </CustomTabPanel>
            ))}
        </Box>
    );
}
