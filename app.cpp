#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

bool isIgnoredExe(const std::string& name) {
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    std::vector<std::string> ignored = {
        "dowser.exe",  // keep it, don't delete or use it
        "crashpad_handler.exe",
        "crashpad_handler32.exe",
        "crashpad_handler64.exe",
        "bootstrapper-v2.exe",
        "launcher-installer-windows_2024.10.exe",
        "paradox launcher.exe"
    };

    for (const auto& bad : ignored) {
        if (lower == bad)
            return true;
    }

    // skip anything with "launcher" in name
    if (lower.find("launcher") != std::string::npos)
        return true;

    return false;
}

int main() {
    std::cout << "=== Paradox Launcher Remover v4 ===\n";
    std::cout << "Cleans Paradox junk, keeps dowser.exe safe.\n\n";

    std::string folderPath;
    std::cout << "What folder? ";
    std::getline(std::cin, folderPath);

    std::filesystem::path targetDir(folderPath);

    if (!std::filesystem::exists(targetDir) || !std::filesystem::is_directory(targetDir)) {
        std::cerr << "Error: specified path is invalid or not a directory.\n";
        return 1;
    }

    std::cout << "\nScanning: " << targetDir << "\n";

    // 1. Remove known Paradox junk files (excluding dowser.exe)
    std::vector<std::string> BAD_FILES {
        "launcher-settings.json",
        "launcher-installer-windows_2024.10.exe",
        "bootstrapper-v2.exe",
        "Paradox Launcher.exe"
    };

    for (const auto& file : BAD_FILES) {
        std::filesystem::path badFile = targetDir / file;
        if (std::filesystem::exists(badFile)) {
            try {
                std::filesystem::remove(badFile);
                std::cout << "[Removed file] " << badFile.filename().string() << '\n';
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "[Error removing] " << badFile << ": " << e.what() << '\n';
            }
        }
    }

    // 2. Remove launcher folder only if it doesn’t contain dowser.exe
    std::filesystem::path launcherFolder = targetDir / "launcher";
    if (std::filesystem::exists(launcherFolder) && std::filesystem::is_directory(launcherFolder)) {
        bool safeToDelete = true;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(launcherFolder)) {
            if (entry.path().filename() == "dowser.exe") {
                safeToDelete = false;
                break;
            }
        }

        if (safeToDelete) {
            try {
                std::uintmax_t removed = std::filesystem::remove_all(launcherFolder);
                std::cout << "[Removed folder] launcher (" << removed << " items deleted)\n";
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "[Error removing folder] " << e.what() << '\n';
            }
        } else {
            std::cout << "[Skipped] launcher folder contains dowser.exe (kept intact)\n";
        }
    }

    // 3. Find the main game executable (ignore junk)
    std::string appName;
    for (const auto& entry : std::filesystem::directory_iterator(targetDir)) {
        if (!entry.is_regular_file())
            continue;

        auto path = entry.path();
        if (path.extension() != ".exe")
            continue;

        std::string filename = path.filename().string();
        if (!isIgnoredExe(filename)) {
            appName = path.stem().string();
            break;
        }
    }

    if (appName.empty()) {
        std::cerr << "No valid game executable found.\n";
        return 1;
    }

    // 4. Create Launcher.bat
    std::filesystem::path launcherPath = targetDir / "Launcher.bat";
    std::ofstream out(launcherPath);
    if (!out.is_open()) {
        std::cerr << "Failed to create Launcher.bat.\n";
        return 1;
    }

    out << "@echo off\n";
    out << "echo Starting " << appName << " without Paradox launcher...\n";
    out << "\"" << appName << ".exe\" %command%\n";
    out.close();

    std::cout << "\n[OK] Created Launcher.bat in " << targetDir << "\n";
    std::cout << "It runs: " << appName << ".exe %command%\n";
    std::cout << "\nDowser preserved, launcher-free gaming achieved.\n";

    return 0;
}
