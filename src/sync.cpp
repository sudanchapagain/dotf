#include "config.hpp"
#include "helpers.hpp"

#include <fmt/core.h>
#include <filesystem>
#include <system_error>
#include <cstdlib>
#include "state.hpp"
#include <algorithm>

void sync(
    bool is_dry_run_enabled,
    bool is_force_enabled,
    bool is_skip_conflicts_enabled,
    Mappings ns,
    Mappings os
) {
    auto entries = evaluate(ns, os);
    Mappings resulting = os;

    for (const auto& e : entries) {
        switch (e.status) {
            case EntryStatus::NewEntry:
            case EntryStatus::MissingSymlink: {
                std::filesystem::path src  = e.mapping.source;
                std::filesystem::path dest = e.mapping.destination;

                if (std::filesystem::exists(dest)) {
                    if (is_skip_conflicts_enabled) {
                        fmt::println("Skip (conflict): {}", dest.string());
                        break;
                    }

                    if (is_force_enabled) {
                        auto backup = make_backup_path(dest);

                        fmt::println("Backup: {} -> {}", dest.string(), backup.string());

                        if (!is_dry_run_enabled) {
                            std::error_code ec;
                            std::filesystem::rename(dest, backup, ec);
                            if (ec) {
                                fmt::println("Warning: failed to backup {}: {}", dest.string(), ec.message());
                                break;
                            }
                        }
                    }
                }

                fmt::println("Link: {} -> {}", src.string(), dest.string());

                bool created = false;

                if (!is_dry_run_enabled) {
                    std::error_code ec;

                    ec.clear();
                    std::filesystem::create_directories(dest.parent_path(), ec);
                    if (ec) {
                        fmt::println("Warning: failed to create directory: {}", ec.message());
                        break;
                    }

                    ec.clear();
                    std::filesystem::create_symlink(src, dest, ec);
                    if (ec) {
                        fmt::println("Warning: symlink failed: {}", ec.message());
                        break;
                    }

                    created = true;
                }

                if (created) {
                    auto it = std::find_if(resulting.begin(), resulting.end(), [&](const Mapping& m){
                        return m.destination == e.mapping.destination;
                    });
                    if (it != resulting.end()) *it = e.mapping;
                    else resulting.push_back(e.mapping);
                }

                break;
            }

            case EntryStatus::Untracked:
                fmt::println("Untracked: {}", e.mapping.destination);
                break;

            case EntryStatus::StaleEntry:
                fmt::println("Stale: {}", e.mapping.destination);

                if (!is_dry_run_enabled) {
                    std::error_code ec;
                    std::filesystem::remove(e.mapping.destination, ec);
                    if (ec) {
                        fmt::println("Warning: failed to remove {}: {}", e.mapping.destination, ec.message());
                    } else {
                        resulting.erase(
                            std::remove_if(resulting.begin(), resulting.end(), [&](const Mapping& m){
                                return m.destination == e.mapping.destination;
                            }),
                            resulting.end()
                        );
                    }
                }
                break;

            case EntryStatus::RemovedExternally:
                fmt::println("Removed externally: {}", e.mapping.destination);
                if (!is_dry_run_enabled) {
                    resulting.erase(
                        std::remove_if(resulting.begin(), resulting.end(), [&](const Mapping& m){
                            return m.destination == e.mapping.destination;
                        }),
                        resulting.end()
                    );
                }
                break;

            case EntryStatus::InSync:
            default:
                break;
        }
    }

    if (!is_dry_run_enabled) {
        try {
            save_state(resulting);
        } catch (const std::exception& ex) {
            fmt::println("Warning: failed to save state: {}", ex.what());
        }
    }
}
