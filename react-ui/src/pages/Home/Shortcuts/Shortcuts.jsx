//Shortcuts.jsx
import { Box } from "@mui/material";
import { GoPlus } from "react-icons/go";
import { useState, useEffect } from "react";
import ShortcutModal from "./ShortcutModal";
import { getShortcuts, saveShortcut } from "./localStorage";

const Shortcuts = () => {
  const [shortcuts, setShortcuts] = useState([]);

  useEffect(() => {
    console.log("useEffect() of Shortcuts.jsx");
    setShortcuts(getShortcuts());
  }, []);

  const handlePopup = () => {
    console.log("handlePopup() of Shortcuts.jsx");

    const modalDoc = document.getElementById("my_modal_5");
    if (modalDoc?.showModal) {
      modalDoc.showModal();
    }
  };

  const handleAdd = (newShortcut) => {
    console.log("handleAdd() of Shortcuts.jsx");

    saveShortcut(newShortcut); // save in localStorage
    setShortcuts((prev) => [...prev, newShortcut]); // update UI immediately
  };

  const handleDeleteShortcut = (indexToRemove) => {
    console.log("handleDeleteShortcut() of Shortcuts.jsx");

    const updatedShortcuts = shortcuts.filter(
      (_, idx) => idx !== indexToRemove
    );
    setShortcuts(updatedShortcuts);
    localStorage.setItem("shortcuts", JSON.stringify(updatedShortcuts));
  };

  return (
    <Box
      sx={{ mt: 5 }}
      className="flex flex-wrap gap-2 w-[400px] md:w-[600px] items-center justify-center"
    >
      {/* Show all shortcuts */}
      {shortcuts &&
        shortcuts.map((shortcut, idx) => (
          <Box
            key={idx}
            className="relative gap-2 text-black flex flex-col items-center border border-blue-500 shadow-xl rounded-xl cursor-pointer hover:shadow-blue-200 hover:bg-blue-100 w-30"
            sx={{ p: 1, py: 2 }}
          >
            {/* X icon to delete */}
            <span
              onClick={(e) => {
                e.stopPropagation(); // prevent triggering handlePopup
                handleDeleteShortcut(idx); // your delete handler
              }}
              className="absolute top-1 right-2 text-lg text-gray-500 hover:text-red-500 cursor-pointer"
            >
              &times;
            </span>

            <img src={shortcut.favicon} alt="" className="w-7" />
            <p>{shortcut.name}</p>
          </Box>
        ))}
      {shortcuts.length < 8 && (
        <Box
          onClick={handlePopup}
          className="text-black flex flex-col items-center border border-blue-500 shadow-xl rounded-xl cursor-pointer hover:shadow-blue-200 hover:bg-blue-100"
          sx={{ p: 1, py: 2 }}
        >
          <GoPlus className="text-4xl" />
          <p>Add a Shortcut</p>
        </Box>
      )}

      <ShortcutModal onAdd={handleAdd} />
    </Box>
  );
};

export default Shortcuts;
