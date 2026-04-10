#include <filesystem>
#include <fmt/core.h>

#include "include/config.hpp"

void
delete_symlink(const std::string& link) {
    auto path = std::filesystem::path(link);
    if (std::filesystem::exists(path)) {
        std::error_code ec;
        std::filesystem::remove(path, ec);

        if (ec) {
            fmt::println("Warning: failed to remove {}: {}\n", link, ec.message());
            fmt::println("Please remove it manually. dotf will not remove it automatically.\n");
        } else {
            fmt::println("Removed: {}", link);
        }
    } else {
        fmt::println("Not found: {}", link);
    }
}

void
remove(Mappings os) {
    for (const auto& i : os) {
        delete_symlink(i.destination);
    }
}
