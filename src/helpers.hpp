#include <string_view>
#include <filesystem>

inline bool starts_with(std::string_view s, std::string_view prefix) {
    return s.substr(0, prefix.size()) == prefix;
}

inline std::filesystem::path make_backup_path(const std::filesystem::path& p) {
    std::filesystem::path backup = p;
    backup += ".bak";

    while (std::filesystem::exists(backup)) {
        backup += ".bak";
    }

    return backup;
}
