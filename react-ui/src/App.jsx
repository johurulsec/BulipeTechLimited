// // App.jsx
// import React, { useState, useEffect } from "react";
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
//   const nextIdRef = React.useRef(1);
//   const defaultHomeUrl = ""; // or use your homepage URL here
//   const [tabs, setTabs] = useState([{ url: defaultHomeUrl, label: "New Tab" }]);

//   const [value, setValue] = useState(0);
//   const [bridgeReady, setBridgeReady] = useState(false);

//   // Setup QWebChannel
//   useEffect(() => {
//     const waitForQt = () => {
//       if (window.qt && typeof QWebChannel !== "undefined") {
//         new QWebChannel(window.qt.webChannelTransport, (channel) => {
//           window.bridge = channel.objects.bridge;
//           setBridgeReady(true);

//           // Handle title updates
//           console.log(
//             "before if() cond, Handle title updates, webchannel useEffect() of app.jsx"
//           );

//           if (window.bridge.updateTitle) {
//             console.log(
//               "window.bridge.updateTitle, webchannel useEffect() of app.jsx"
//             );

//             window.bridge.updateTitle.connect((title) => {
//               setTabs((prevTabs) =>
//                 prevTabs.map((tab, index) =>
//                   index === value ? { ...tab, label: title } : tab
//                 )
//               );
//             });
//           }
//           // tabUrl change for back, forward button
//           // if (window.bridge.updateTabUrl) {
//           //   console.log(
//           //     "window.bridge.updateTabUrl, webchannel useEffect() of app.jsx"
//           //   );
//           //   window.bridge.updateTabUrl.connect((tabId, newUrl) => {
//           //     setTabs((prev) =>
//           //       prev.map((tab) =>
//           //         tab.id === tabId
//           //           ? { ...tab, url: newUrl, label: newUrl }
//           //           : tab
//           //       )
//           //     );
//           //   });
//           // }

//           if (window.bridge.updateTabUrl) {
//             window.bridge.updateTabUrl.connect((tabId, newUrl) => {
//               console.log("updateTabUrl from Qt:", tabId, newUrl);
//               setTabs((prevTabs) =>
//                 prevTabs.map((tab) =>
//                   tab.id === tabId
//                     ? { ...tab, url: newUrl, label: newUrl }
//                     : tab
//                 )
//               );
//             });
//           }

//           // after setting up QWebChannel:
//           if (window.bridge.openInNewTab) {
//             window.bridge.openInNewTab.connect((url) => {
//               const newId = nextIdRef.current++;
//               setTabs((prev) => {
//                 const newTabs = [...prev, { id: newId, url, label: url }];
//                 setValue(newTabs.length - 1);
//                 return newTabs;
//               });

//               // tell Qt to create the view and switch current tab then load the url
//               if (window.bridge.createTab) window.bridge.createTab(newId);
//               if (window.bridge.setCurrentTab)
//                 window.bridge.setCurrentTab(newId);
//               if (window.bridge.loadUrl) window.bridge.loadUrl(url);
//             });
//           }
//         });
//       } else {
//         setTimeout(waitForQt, 10);
//       }
//     };

//     waitForQt();
//   }, []);

//   // Load current tab's URL on switch or bridge ready
//   useEffect(() => {
//     //-------------------------------- start
//     const currentTab = tabs[value];

//     if (!bridgeReady || !window.bridge || !currentTab) {
//       console.log(
//         "!bridgeReady || !window.bridge || !currentTab of loadUrl useEffect() of app.jsx"
//       );
//       return;
//     }
//     const url = currentTab.url || "";
//     console.log(
//       "Loading URL for current tab, loadUrl useEffect() of app.jsx:",
//       url
//     );

//     window.bridge.loadUrl(url);

//     //-------------------------------- end
//   }, [value, bridgeReady]);

//   const handleLoadUrl = (url) => {
//     console.log("handleLoadUrl() of app.jsx");

//     // Update current tab's url AND label
//     setTabs((prev) =>
//       prev.map((tab, i) => (i === value ? { ...tab, url, label: url } : tab))
//     );

//     // Load immediately to Qt
//     if (bridgeReady && window.bridge?.loadUrl) {
//       console.log("Load immediately to Qt of app.jsx");

//       window.bridge.loadUrl(url);
//     }
//   };

//   const handleChange = (event, newValue) => {
//     console.log("handleChange() of app.jsx");
//     if (newValue === tabs.length) {
//       console.log("handleChange(), Clicked the plus tab of app.jsx");
//       // Clicked the "+" tab
//       handleAddTab();
//       return;
//     }
//     setValue(newValue);
//     const tab = tabs[newValue];
//     if (tab && window.bridge?.setCurrentTab) {
//       window.bridge.setCurrentTab(tab.id);
//     }
//   };

//   const handleAddTab = () => {
//     const newId = nextIdRef.current++;
//     console.log("handleAddTab() of app.jsx, newId:", newId);

//     setTabs((prev) => [...prev, { id: newId, url: "", label: "New Tab" }]);
//     setValue(tabs.length); // new index
//     if (window.bridge?.createTab) window.bridge.createTab(newId);
//     if (window.bridge?.setCurrentTab) window.bridge.setCurrentTab(newId);
//   };

//   const handleCloseTab = (indexToClose) => {
//     console.log("handleCloseTab() of app.jsx");

//     const idToClose = tabs[indexToClose].id;
//     if (window.bridge?.closeTab) window.bridge.closeTab(idToClose);

//     setTabs((prevTabs) => {
//       const newTabs = prevTabs.filter((t, i) => i !== indexToClose);
//       // maintain value as you already handled...
//       if (value >= newTabs.length) setValue(newTabs.length - 1);
//       else if (indexToClose < value) setValue((prev) => prev - 1);
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
//                   <span style={{ fontSize: "0.75rem" }}>
//                     {tab.label || "New Tab"}
//                   </span>
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

// === App.jsx ===
import React, { useState, useEffect, useRef } from "react";
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
  const nextIdRef = useRef(1); // start at 1, reserve 0 for initial tab
  const defaultHomeUrl = ""; // put your homepage here

  // initialize first tab with id 0
  const [tabs, setTabs] = useState([
    { id: 0, url: defaultHomeUrl, label: "New Tab" },
  ]);

  const [value, setValue] = useState(0);
  const [bridgeReady, setBridgeReady] = useState(false);

  // toolbar input is controlled by App so ToolBar shows current tab url
  const [toolbarInput, setToolbarInput] = useState("");

  // Setup QWebChannel
  useEffect(() => {
    const waitForQt = () => {
      if (window.qt && typeof QWebChannel !== "undefined") {
        new QWebChannel(window.qt.webChannelTransport, (channel) => {
          window.bridge = channel.objects.bridge;
          setBridgeReady(true);

          // Handle title updates from pages
          if (window.bridge.updateTitle) {
            window.bridge.updateTitle.connect((title) => {
              setTabs((prevTabs) =>
                prevTabs.map((tab, index) =>
                  index === value ? { ...tab, label: title || tab.label } : tab
                )
              );
            });
          }

          // Handle open-in-new-tab requests from C++ pages
          if (window.bridge.openInNewTab) {
            console.log("openInNewTab of app.jsx useEffect webchannel");

            window.bridge.openInNewTab.connect((url) => {
              const newId = nextIdRef.current++;
              setTabs((prev) => {
                const newTabs = [...prev, { id: newId, url, label: url }];
                return newTabs;
              });

              // switch to the new tab after Qt creates it
              setValue((prevTabsCount) => {
                // we'll compute index later in useEffect that syncs with tabs
                return tabs.length; // optimistic; will be corrected when tabs state updates
              });

              if (window.bridge.createTab) {
                console.log(
                  "before calling window.bridge.createTab(newId) of app.jsx useEffect webchannel"
                );
                window.bridge.createTab(newId);
              }
              if (window.bridge.setCurrentTab) {
                console.log(
                  "before calling window.bridge.setCurrentTab(newId) of app.jsx useEffect webchannel"
                );
                window.bridge.setCurrentTab(newId);
              }
              if (window.bridge.loadUrl) {
                console.log(
                  "before calling window.bridge.loadUrl(url) of app.jsx useEffect webchannel"
                );
                window.bridge.loadUrl(url);
              }
            });
          }

          // receive URL updates from Qt for a specific tab
          if (window.bridge.updateTabUrl) {
            window.bridge.updateTabUrl.connect((tabId, newUrl) => {
              console.log(
                "inside Setup QWebChannel useEffect(), updateTabUrl back/forward press from Qt, app.jsx:",
                tabId,
                newUrl
              );
              setTabs((prevTabs) =>
                prevTabs.map((tab) =>
                  tab.id === tabId
                    ? {
                        ...tab,
                        url: newUrl,
                        // keep an existing title if it's not equal to the old url, otherwise fall back to URL
                        label:
                          tab.label && tab.label !== tab.url
                            ? tab.label
                            : newUrl,
                      }
                    : tab
                )
              );

              // if the updated tab is the active one, update the toolbar input
              setTabs((prevTabs) => {
                const activeTab = prevTabs[value];
                if (activeTab && activeTab.id === tabId) {
                  setToolbarInput(newUrl);
                }
                return prevTabs;
              });
            });
          }

          // (optional) handle Qt asking React to create/set current tab
          if (window.bridge.requestCreateTabFromQt) {
            window.bridge.requestCreateTabFromQt.connect((tabId) => {
              // minimal handling: ensure there's a tab with that id
              setTabs((prev) =>
                prev.some((t) => t.id === tabId)
                  ? prev
                  : [...prev, { id: tabId, url: "", label: "New Tab" }]
              );
            });
          }
        });
      } else {
        setTimeout(waitForQt, 10);
      }
    };

    waitForQt();
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  // Load current tab's URL on switch or bridge ready
  useEffect(() => {
    const currentTab = tabs[value];

    if (!bridgeReady || !window.bridge || !currentTab) return;

    const url = currentTab.url || "";
    console.log(
      "Loading URL for current tab, loadUrl useEffect() of app.jsx:",
      url
    );

    // Make sure Qt knows which tab is current
    if (window.bridge.setCurrentTab) {
      console.log(
        "before calling window.bridge.setCurrentTab(currentTab.id), loadUrl useEffect() of app.jsx:"
      );
      window.bridge.setCurrentTab(currentTab.id);
    }
    // load the URL in C++ webview for the current tab
    if (window.bridge.loadUrl) {
      console.log(
        "before calling window.bridge.loadUrl(url), loadUrl useEffect() of app.jsx:"
      );
      window.bridge.loadUrl(url);
    }
    // update the toolbar input to reflect the active tab
    console.log(
      "before calling setToolbarInput(url), loadUrl useEffect() of app.jsx:"
    );
    setToolbarInput(url);
  }, [value, bridgeReady, tabs]);

  const handleLoadUrl = (url) => {
    console.log("handleLoadUrl() of app.jsx", url);

    setTabs((prev) =>
      prev.map((tab, i) => (i === value ? { ...tab, url, label: url } : tab))
    );

    // Load immediately to Qt
    if (bridgeReady && window.bridge?.loadUrl) {
      window.bridge.loadUrl(url);
    }
  };

  const handleChange = (event, newValue) => {
    console.log("handleChange() of app.jsx, newValue:", newValue);
    if (newValue === tabs.length) {
      handleAddTab();
      return;
    }
    setValue(newValue);
    const tab = tabs[newValue];
    if (tab && window.bridge?.setCurrentTab) {
      window.bridge.setCurrentTab(tab.id);
    }
  };

  const handleAddTab = () => {
    const newId = nextIdRef.current++;
    console.log("handleAddTab() of app.jsx, newId:", newId);

    setTabs((prev) => [...prev, { id: newId, url: "", label: "New Tab" }]);
    setValue(tabs.length); // new index (optimistic)
    if (window.bridge?.createTab) window.bridge.createTab(newId);
    if (window.bridge?.setCurrentTab) window.bridge.setCurrentTab(newId);
  };

  const handleCloseTab = (indexToClose) => {
    console.log("handleCloseTab() of app.jsx");

    const idToClose = tabs[indexToClose].id;
    if (window.bridge?.closeTab) window.bridge.closeTab(idToClose);

    setTabs((prevTabs) => {
      const newTabs = prevTabs.filter((t, i) => i !== indexToClose);
      if (value >= newTabs.length) setValue(newTabs.length - 1);
      else if (indexToClose < value) setValue((prev) => prev - 1);
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
              key={tab.id}
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

      <ToolBar
        onLoadUrl={handleLoadUrl}
        inputUrl={toolbarInput}
        setInputUrl={setToolbarInput}
      />

      {tabs.map((tab, index) => (
        <CustomTabPanel key={tab.id} value={value} index={index}>
          <Home url={tab.url} onLoadUrl={handleLoadUrl} />
        </CustomTabPanel>
      ))}
    </Box>
  );
}
