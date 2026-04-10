#include "config.hpp"
#include <fmt/core.h>

static std::string status_to_string(EntryStatus s) {
    switch (s) {
        case EntryStatus::InSync:
            return fmt::format("\x1b[32mInSync\x1b[0m");

        case EntryStatus::MissingSymlink:
            return fmt::format("\x1b[31mMissingSymlink\x1b[0m");

        case EntryStatus::NewEntry:
            return fmt::format("\x1b[33mNewEntry\x1b[0m");

        case EntryStatus::Untracked:
            return fmt::format("\x1b[36mUntracked\x1b[0m");

        case EntryStatus::StaleEntry:
            return fmt::format("\x1b[35mStaleEntry\x1b[0m");

        case EntryStatus::RemovedExternally:
            return fmt::format("\x1b[31mRemovedExternally\x1b[0m");

        default:
            return "Unknown";
    }
}

void print_status(EvaluatedEntry e) {
    fmt::print("{}: {} \t--> {}\n",  status_to_string(e.status), e.mapping.source, e.mapping.destination);
}

void status(Mappings ns, Mappings os) {
    auto entries = evaluate(ns, os);

    for (const auto& e : entries) {
        print_status(e);
    }
}
