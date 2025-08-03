// // App.jsx
// import React, { useState } from "react";
// import PropTypes from "prop-types";
// import Tabs from "@mui/material/Tabs";
// import Tab from "@mui/material/Tab";
// import Box from "@mui/material/Box";
// import IconButton from "@mui/material/IconButton";
// import CloseIcon from "@mui/icons-material/Close";
// import ToolBar from "./components/ToolBar";
// import Home from "./pages/Home/Home";
// import WindowControls from "./components/WindowControls";
// import logo from "./assets/logo.png";
// import "./index.css";

// function CustomTabPanel(props) {
//   const { children, value, index, ...other } = props;
//   return (
//     <div
//       role="tabpanel"
//       hidden={value !== index}
//       id={`simple-tabpanel-${index}`}
//       {...other}
//     >
//       {value === index && children}
//     </div>
//   );
// }

// CustomTabPanel.propTypes = {
//   children: PropTypes.node,
//   index: PropTypes.number.isRequired,
//   value: PropTypes.number.isRequired,
// };

// export default function BasicTabs() {
//   const [tabs, setTabs] = useState([{ url: "" }]);
//   const [value, setValue] = useState(0);
//   const [bridgeReady, setBridgeReady] = useState(false);

//   //   React.useEffect(() => {
//   //     if (typeof QWebChannel === "undefined") {
//   //       console.error(
//   //         "QWebChannel is not defined — make sure qwebchannel.js is loaded!"
//   //       );
//   //       return;
//   //     }
//   //     new QWebChannel(qt.webChannelTransport, (channel) => {
//   //       window.bridge = channel.objects.bridge;

//   //       window.bridge.updateTitle.connect((title) => {
//   //         console.log("Page title changed:", title);
//   //         setTabs((prevTabs) =>
//   //           prevTabs.map((tab, index) =>
//   //             index === value ? { ...tab, label: title } : tab
//   //           )
//   //         );
//   //       });
//   //     });
//   //   }, [value, setTabs]);

//   React.useEffect(() => {
//     console.log("Waiting for qt and QWebChannel...");

//     const waitForQt = () => {
//       if (window.qt && typeof QWebChannel !== "undefined") {
//         console.log("Qt + QWebChannel available, initializing...");

//         new QWebChannel(window.qt.webChannelTransport, (channel) => {
//           window.bridge = channel.objects.bridge;
//           setBridgeReady(true);

//           console.log("✅ Qt bridge ready:", window.bridge);

//           // Optional signal: Page title change
//           if (window.bridge.updateTitle) {
//             window.bridge.updateTitle.connect((title) => {
//               console.log("Title updated:", title);
//               setTabs((prevTabs) =>
//                 prevTabs.map((tab, index) =>
//                   index === value ? { ...tab, label: title } : tab
//                 )
//               );
//             });
//           }
//         });
//       } else {
//         // Retry after short delay
//         setTimeout(waitForQt, 100);
//       }
//     };

//     waitForQt(); // start polling
//   }, []);

//   const handleLoadUrl = (url) => {
//     console.log("handleLoadUrl() of App.jsx", url);

//     setTabs((prev) =>
//       prev.map((tab, i) => (i === value ? { ...tab, url } : tab))
//     );

//     const trySendToBridge = (attempt = 0) => {

//       if (window.bridge?.loadUrl(url)) {
//         console.log("✅ Sending to Qt bridge:", url);
//         // window.bridge.requestLoadUrl(url);
//         //window.bridge?.closeWindow?.();
//         window.bridge?.loadUrl(url);

//       } else if (attempt < 5) {
//         console.log(`⏳ Waiting for bridge... attempt ${attempt}`);
//         setTimeout(() => trySendToBridge(attempt + 1), 200);

//       } else {
//         console.warn("❌ Failed to send to Qt bridge — bridge not ready");
//       }
//     };

//     trySendToBridge();

//     // if (!url && window.bridge?.requestLoadUrl) {
//     //   window.bridge.requestLoadUrl(""); // hide content
//     // }
//     if (!url && window.bridge?.loadUrl()) {
//       window.bridge?.loadUrl(""); // hide content
//     }

//     console.log("next step ...?");
//   };

//   const handleChange = (event, newValue) => {
//     if (newValue === tabs.length) {
//       handleAddTab();
//     } else {
//       setValue(newValue);
//     }
//   };

//   const handleAddTab = () => {
//     const newIndex = tabs.length;
//     setTabs((prev) => [...prev, { url: "" }]);
//     setValue(newIndex);
//   };

//   const handleCloseTab = (indexToClose) => {
//     if (tabs.length === 1) return;

//     setTabs((prevTabs) => {
//       const newTabs = prevTabs.filter((_, i) => i !== indexToClose);
//       if (value >= newTabs.length) {
//         setValue(newTabs.length - 1);
//       }
//       return newTabs;
//     });
//   };

//   return (
//     <Box sx={{ width: "100%" }}>
//       <Box
//         sx={{
//           display: "flex",
//           alignItems: "center",
//           justifyContent: "space-between",
//           borderBottom: 1,
//           borderColor: "divider",
//           backgroundColor: "#f5f5f5",
//         }}
//       >
//         <Tabs
//           value={value}
//           onChange={handleChange}
//           variant="scrollable"
//           scrollButtons="auto"
//           sx={{ flexGrow: 1 }}
//         >
//           {tabs.map((tab, index) => (
//             <Tab
//               key={index}
//               label={
//                 <Box sx={{ display: "flex", alignItems: "center" }}>
//                   <img
//                     src={logo}
//                     alt="logo"
//                     style={{ width: 20, height: 20, marginRight: 6 }}
//                   />
//                   <span style={{ fontSize: "0.75rem" }}>New Tab</span>
//                   <Box
//                     component="span"
//                     onClick={(e) => {
//                       e.stopPropagation();
//                       handleCloseTab(index);
//                     }}
//                     sx={{
//                       ml: 1,
//                       cursor: "pointer",
//                       borderRadius: "50%",
//                       padding: "2px",
//                       "&:hover": {
//                         backgroundColor: "#f0f0f0",
//                       },
//                     }}
//                   >
//                     <CloseIcon fontSize="small" />
//                   </Box>
//                 </Box>
//               }
//             />
//           ))}
//           <Tab label="+" />
//         </Tabs>

//         <Box sx={{ flexShrink: 0, pr: 1 }}>
//           <WindowControls />
//         </Box>
//       </Box>

//       <ToolBar onLoadUrl={handleLoadUrl} />

//       {tabs.map((tab, index) => (
//         <CustomTabPanel key={index} value={value} index={index}>
//           <Home url={tab.url} onLoadUrl={handleLoadUrl} />
//         </CustomTabPanel>
//       ))}
//     </Box>
//   );
// }

import React, { useState, useEffect } from "react";
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
  const defaultHomeUrl = ""; // or use your homepage URL here
  const [tabs, setTabs] = useState([{ url: defaultHomeUrl, label: "New Tab" }]);
  const [value, setValue] = useState(0);
  const [bridgeReady, setBridgeReady] = useState(false);

  // Setup QWebChannel
  useEffect(() => {
    const waitForQt = () => {
      if (window.qt && typeof QWebChannel !== "undefined") {
        new QWebChannel(window.qt.webChannelTransport, (channel) => {
          window.bridge = channel.objects.bridge;
          setBridgeReady(true);

          // Handle title updates
          console.log("before if() cond, Handle title updates");

          if (window.bridge.updateTitle) {
            console.log("window.bridge.updateTitle");

            window.bridge.updateTitle.connect((title) => {
              setTabs((prevTabs) =>
                prevTabs.map((tab, index) =>
                  index === value ? { ...tab, label: title } : tab
                )
              );
            });
          }
        });
      } else {
        setTimeout(waitForQt, 10);
      }
    };

    waitForQt();
  }, []);

  // Load current tab's URL on switch or bridge ready
  useEffect(() => {
    const currentTab = tabs[value];
    if (!bridgeReady || !window.bridge || !currentTab) return;

    const url = currentTab.url || "";
    console.log("Loading URL for current tab:", url);

    window.bridge.loadUrl(url);
  }, [value, bridgeReady]);

  const handleLoadUrl = (url) => {
    console.log("handleLoadUrl() of app.jsx");

    // Update current tab's url
    setTabs((prev) =>
      prev.map((tab, i) => (i === value ? { ...tab, url } : tab))
    );

    // Load immediately to Qt
    if (bridgeReady && window.bridge?.loadUrl) {
      console.log("Load immediately to Qt");

      window.bridge.loadUrl(url);
    }
  };

  const handleChange = (event, newValue) => {
    console.log("handleChange() of app.jsx");

    if (newValue === tabs.length) {
      console.log("before calling handleAddTab() of app.jsx");
      handleAddTab();
    } else {
      setValue(newValue);
    }
  };

  const handleAddTab = () => {
    console.log("handleAddTab() of app.jsx");

    const newIndex = tabs.length;
    const newTab = { url: defaultHomeUrl, label: "New Tab" };
    setTabs((prev) => [...prev, newTab]);
    setValue(newIndex); // select new tab
  };

  const handleCloseTab = (indexToClose) => {
    console.log("handleCloseTab() of app.jsx");

    if (tabs.length === 1) return;

    setTabs((prevTabs) => {
      const newTabs = prevTabs.filter((_, i) => i !== indexToClose);
      if (value >= newTabs.length) {
        setValue(newTabs.length - 1);
      } else if (indexToClose < value) {
        setValue((prev) => prev - 1);
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
                <Box sx={{ display: "flex", alignItems: "center" }}>
                  <img
                    src={logo}
                    alt="logo"
                    style={{ width: 20, height: 20, marginRight: 6 }}
                  />
                  <span style={{ fontSize: "0.75rem" }}>
                    {tab.label || "New Tab"}
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
          <Home url={tab.url} onLoadUrl={handleLoadUrl} />
        </CustomTabPanel>
      ))}
    </Box>
  );
}
