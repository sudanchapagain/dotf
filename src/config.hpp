#pragma once

#include <toml++/toml.hpp>
#include <string>
#include <vector>

struct Mapping { std::string source; std::string destination; };
using Mappings = std::vector<Mapping>;

enum class EntryStatus {
    // new entry: exists, old entry: exists, symlink: exists
    InSync,
    // new entry: exists, old entry: exists, symlink: no
    MissingSymlink,
    // new entry: exists, old entry: no, symlink: no
    NewEntry,
    // new entry: exists, old entry: no, symlink: exists
    Untracked,
    // new entry: no, old entry: exists, symlink: exists
    StaleEntry,
    // new entry: no, old entry: exists, symlink: no
    RemovedExternally
};

struct EvaluatedEntry {
    Mapping mapping;
    bool in_mapping;
    bool in_state;
    bool symlink_exists;
    EntryStatus status;
};

std::vector<EvaluatedEntry> evaluate(const Mappings& new_state, const Mappings& old_state);

Mappings load_mappings(toml::node_view<toml::node> table);
Mappings resolve_links_to_absolute(Mappings map);
