#pragma once

#include "fmt/base.h"
#include <filesystem>

#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#ifndef _WIN32
    #include <pwd.h>
    #include <unistd.h>
#endif

inline std::filesystem::path get_home_directory() {
#ifdef _WIN32
    if (const char* userProfile = std::getenv("USERPROFILE")) {
        return std::filesystem::path(userProfile);
    }

    const char* homeDrive = std::getenv("HOMEDRIVE");
    const char* homePath = std::getenv("HOMEPATH");

    if (homeDrive && homePath) {
        return std::filesystem::path(std::string(homeDrive) + homePath);
    }

    throw std::runtime_error("Unable to determine home directory on Windows");
#else
    if (const char* home = std::getenv("HOME")) {
        return std::filesystem::path(home);
    }

    struct passwd pwd;
    struct passwd* result = nullptr;

    long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize < 0) {
        bufsize = 16384;
    }

    std::vector<char> buffer(static_cast<size_t>(bufsize));

    if (getpwuid_r(getuid(), &pwd, buffer.data(), buffer.size(), &result) != 0 || !result) {
        throw std::runtime_error("Unable to determine home directory on POSIX");
    }

    if (!result->pw_dir) {
        throw std::runtime_error("Home directory not set in passwd entry");
    }

    return std::filesystem::path(result->pw_dir);
#endif
}

inline std::filesystem::path get_dotfiles_path() {
    auto home_dir = get_home_directory();
    auto dotfiles_dir = home_dir / ".dotfiles";

    if (!std::filesystem::exists(dotfiles_dir)) {
        fmt::println("Fatal: {} does not exist.", (std::string) dotfiles_dir);
        std::exit(EXIT_FAILURE);
    }

    return dotfiles_dir;
}

inline std::filesystem::path get_config_file_path() {
    auto dotfiles_dir = get_dotfiles_path();
    auto config_file = dotfiles_dir / ".mappings.toml";

    if (!std::filesystem::exists(config_file)) {
        fmt::println("Fatal: {} does not exist.", (std::string) config_file);
        std::exit(EXIT_FAILURE);
    }

    return config_file;
}

inline std::filesystem::path get_state_file_path() {
    auto dotfiles_dir = get_dotfiles_path();
    auto state_file = dotfiles_dir / ".state";

    if (!std::filesystem::exists(state_file)) {
        std::ofstream state(state_file);
    }

    return state_file;
}
