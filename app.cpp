#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <regex>
#include <algorithm>

bool isIgnored(const std::string& name) {
    static const std::vector<std::string> ignored = {
        "dowser.exe", "crashpad_handler.exe", "crashpad_handler32.exe",
        "crashpad_handler64.exe", "bootstrapper-v2.exe",
        "launcher-installer-windows_2024.10.exe", "paradox launcher.exe"
    };
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    for (const auto& bad : ignored)
        if (lower == bad || lower.find("launcher") != std::string::npos)
            return true;
    return false;
}

bool matchesParadoxGame(const std::string& exe) {
    static const std::vector<std::regex> paradoxGamePatterns = {
        std::regex("cities|skylines", std::regex_constants::icase),
        std::regex("ck[23]|crusader", std::regex_constants::icase),
        std::regex("eu[34]|europa", std::regex_constants::icase),
        std::regex("hoi[34]|hearts of iron", std::regex_constants::icase),
        std::regex("stellaris", std::regex_constants::icase),
        std::regex("victoria", std::regex_constants::icase),
        std::regex("imperator|rome", std::regex_constants::icase),
        std::regex("ageofwonders|aow", std::regex_constants::icase),
        std::regex("prisonarchitect|prison architect", std::regex_constants::icase),
        std::regex("survivingmars|surviving mars", std::regex_constants::icase),
        std::regex("tyranny", std::regex_constants::icase),
        std::regex("empireofsin|empire of sin", std::regex_constants::icase),
    };
    for (const auto& pattern : paradoxGamePatterns) {
        if (std::regex_search(exe, pattern))
            return true;
    }
    return false;
}

int main() {
    std::cout << "Paradox Launcher Bypass\nFolder? ";
    std::string folder;
    std::getline(std::cin, folder);
    std::filesystem::path dir(folder);
    if (!std::filesystem::exists(dir) || !std::filesystem::is_directory(dir)) {
        std::cerr << "Invalid directory\n";
        return 1;
    }

    const std::vector<std::string> junkFiles = {
        "launcher-settings.json",
        "launcher-installer-windows_2024.10.exe",
        "bootstrapper-v2.exe",
        "Paradox Launcher.exe"
    };
    for (const auto& file : junkFiles) {
        try {
            std::filesystem::remove(dir / file);
        } catch (...) {}
    }

    auto launcherDir = dir / "launcher";
    if (std::filesystem::exists(launcherDir) && std::filesystem::is_directory(launcherDir)) {
        bool keep = false;
        for (const auto& e : std::filesystem::recursive_directory_iterator(launcherDir)) {
            if (e.path().filename() == "dowser.exe") {
                keep = true;
                break;
            }
        }
        if (!keep) {
            try {
                std::filesystem::remove_all(launcherDir);
            } catch (...) {}
        }
    }

    std::string exeName;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".exe") continue;
        auto name = entry.path().filename().string();
        if (isIgnored(name)) continue;
        if (matchesParadoxGame(name)) {
            exeName = entry.path().stem().string();
            break;
        }
    }

    if (exeName.empty()) {
        std::cerr << "No matching executable found\n";
        return 1;
    }

    std::ofstream out(dir / "Launcher.bat");
    if (!out.is_open()) {
        std::cerr << "Failed to create Launcher.bat\n";
        return 1;
    }
    out << "@echo off\n";
    out << "echo Starting " << exeName << " without Paradox launcher...\n";
    out << '\"' << exeName << ".exe\" %command%\n";

    std::cout << "Launcher.bat created for " << exeName << "\n";
    return 0;
}
