#pragma once

#include <fstream>
#include <toml++/toml.hpp>
#include <filesystem>
#include <stdexcept>

#include "include/config.hpp"
#include "include/path.hpp"

inline void save_state(const Mappings& map) {
    toml::table root;
    toml::table general;

    auto dotfiles = get_dotfiles_path();
    auto home = get_home_directory().string();

    for (const auto& m : map) {
        std::filesystem::path s_path(m.source);
        std::string rel;
        try {
            rel = std::filesystem::relative(s_path, dotfiles).string();
        } catch (...) {
            rel = s_path.string();
        }

        std::string dest = m.destination;
        if (starts_with(dest, home)) {
            dest = std::string("~") + dest.substr(home.size());
        }

        general.insert(rel, dest);
    }

    root.insert("general", std::move(general));

    std::ofstream file(get_state_file_path().string(), std::ios::trunc);
    if (!file) {
        throw std::runtime_error("failed to open state file");
    }

    file << root;
}