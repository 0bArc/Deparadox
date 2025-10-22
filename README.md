Understood. Here’s a **professional GitHub `README.md`** version with badges, star icons, and a clean look — no emojis, no informal tone.

---

# Paradox Launcher Remover

![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge\&logo=c%2B%2B)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey?style=for-the-badge\&logo=windows)
![Stars](https://img.shields.io/github/stars/0bArc/Deparadox?style=for-the-badge\&color=gold)
![License](https://img.shields.io/github/license/0bArc/Deparadox?style=for-the-badge\&color=green)

---

### Overview

Paradox Launcher Remover is a lightweight C++ utility that removes the malfunctioning Paradox Launcher files and restores direct game launching through Steam.
It safely preserves **dowser.exe** and automatically creates a working `Launcher.bat` that launches the actual game executable using Steam’s launch argument system.

---

### Features

* Detects and removes redundant launcher files and folders
* Keeps **dowser.exe** untouched
* Automatically identifies the correct game executable
* Creates a clean `Launcher.bat` to replace the Paradox launcher
* Works with titles such as *Cities: Skylines*, *Stellaris*, *Prison Architect*, *Hearts of Iron IV*, and others

---

### Build Instructions

**Requirements**

* C++17 compatible compiler (GCC, Clang, or MSVC)
* Windows environment

**Build command**

```bash
g++ -std=c++17 -O2 main.cpp -o ParadoxRemover.exe
```

---

### Usage

1. Run the compiled executable:

   ```
   ParadoxRemover.exe
   ```

2. When prompted:

   ```
   What folder?
   ```

   Paste the full path to your game’s installation directory, for example:

   ```
   C:\Program Files (x86)\Steam\steamapps\common\Prison Architect
   ```

3. The program will:

   * Remove launcher-related junk
   * Keep **dowser.exe** intact
   * Create a new `Launcher.bat` file

---

### Steam Configuration Guide

1. Open **Steam**
2. Go to **Library**
3. Right-click your game and select **Properties**
4. Under the **General** tab, locate **Launch Options**
5. Enter the following command:

   ```
   Launcher.bat %command%
   ```

Steam will now execute your custom batch launcher instead of the Paradox Launcher.

---

### Example Directory Structure

After running the tool, your game folder should look like this:

```
C:\Program Files (x86)\Steam\steamapps\common\Prison Architect\
│
├── dowser.exe
├── Prison Architect64.exe
├── Launcher.bat
└── (launcher folder and junk removed)
```

**Launcher.bat content**

```bat
@echo off
echo Starting Prison Architect64.exe without Paradox launcher...
"Prison Architect64.exe" %command%
```

---

### License

This project is distributed under the **MIT License**. You are free to use, modify, and distribute it.

---

### Support the Project

If this tool helped you, consider giving the repository a ⭐ on GitHub.
Your feedback and contributions are welcome.

---

Would you like me to generate a **banner image** (for the top of the README) that matches this aesthetic — dark background, C++ logo, and the text “Paradox Launcher Remover” in clean white type?
