#include <vector>
#include <unordered_map>
#include <filesystem>

#include "include/config.hpp"

std::vector<EvaluatedEntry>
evaluate(
    const Mappings& ns,
    const Mappings& os
) {
    std::vector<EvaluatedEntry> result;

    std::unordered_map<std::string, Mapping> map_ns;
    std::unordered_map<std::string, Mapping> map_os;

    for (const auto& m : ns) {
        map_ns[m.destination] = m;
    }

    for (const auto& m : os) {
        map_os[m.destination] = m;
    }

    std::unordered_map<std::string, bool> all_keys;

    for (const auto& [k, _] : map_ns) {
        all_keys[k] = true;
    }
    for (const auto& [k, _] : map_os) {
        all_keys[k] = true;
    }

    for (const auto& [dest, _] : all_keys) {
        bool in_mapping = map_ns.find(dest) != map_ns.end();
        bool in_state = map_os.find(dest) != map_os.end();

        const Mapping* m = nullptr;

        if (in_mapping) {
            m = &map_ns.find(dest)->second;
        } else {
            m = &map_os.find(dest)->second;
        }

        bool symlink_exists = false;

        std::error_code ec;
        std::filesystem::path dest_path = dest;
        if (
            std::filesystem::exists(dest_path, ec) &&
            std::filesystem::is_symlink(dest_path, ec)
        ) {
            auto target = std::filesystem::read_symlink(dest_path, ec);
            if (!ec) {
                auto expected = std::filesystem::path(m->source);

                std::error_code ec2;
                auto norm_target   = std::filesystem::weakly_canonical(target, ec2);
                auto norm_expected = std::filesystem::weakly_canonical(expected, ec2);

                if (!ec2 && norm_target == norm_expected) {
                    symlink_exists = true;
                }
            }
        }

        EntryStatus status;

        if (in_mapping && in_state && symlink_exists) {
            status = EntryStatus::InSync;
        } else if (in_mapping && in_state && !symlink_exists) {
            status = EntryStatus::MissingSymlink;
        } else if (in_mapping && !in_state && !symlink_exists) {
            status = EntryStatus::NewEntry;
        } else if (in_mapping && !in_state && symlink_exists) {
            status = EntryStatus::Untracked;
        } else if (!in_mapping && in_state && symlink_exists) {
            status = EntryStatus::StaleEntry;
        } else if (!in_mapping && in_state && !symlink_exists) {
            status = EntryStatus::RemovedExternally;
        } else {
            continue;
        }

        EvaluatedEntry ee;
        ee.mapping = *m;
        ee.in_mapping = in_mapping;
        ee.in_state = in_state;
        ee.symlink_exists = symlink_exists;
        ee.status = status;

        result.push_back(std::move(ee));
    }

    return result;
}
