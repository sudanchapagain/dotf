#include <cstdlib>
#include <exception>
#include <fmt/base.h>
#include <argparse/argparse.hpp>
#include <toml++/toml.hpp>

#include "include/path.hpp"
#include "include/config.hpp"
#include "include/commands.hpp"

#include <iostream>
#include <string>

int
main(int argc, char *argv[]) {
#ifndef DOTF_VERSION
#define DOTF_VERSION "unknown"
#endif
    argparse::ArgumentParser program("dotf", DOTF_VERSION);
    program.add_description("dotf - manage dotfiles with symlinks");

    argparse::ArgumentParser sync_command("sync");
    sync_command.add_description("Sync everything to defined paths.");

    auto &flag = sync_command.add_mutually_exclusive_group(true);
    flag.add_argument("--force", "-f")
        .help("Forcefully link by renaming existing files when conflicts arise.")
        .flag();
    flag.add_argument("--skip-conflicts", "-s")
        .help("Skip linking for all conflicts that arise.")
        .flag();

    sync_command.add_argument("--dry-run", "-d")
        .help("Skip destructive actions while appearing to perform said action.")
        .flag();

    argparse::ArgumentParser remove_command("remove");
    remove_command.add_description("Remove linked dotfiles.");

    argparse::ArgumentParser status_command("status");
    status_command.add_description("Show the status of dotfiles.");

    program.add_subparser(sync_command);
    program.add_subparser(remove_command);
    program.add_subparser(status_command);

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        fmt::println("{}", err.what());

        std::cerr << program;
        std::exit(1);
    }

    auto is_command_sync = program.is_subcommand_used("sync");
    auto is_dry_run_enabled = sync_command.is_used("--dry-run");
    auto is_force_enabled = sync_command.is_used("--force");
    auto is_skip_conflicts_enabled = sync_command.is_used("--skip-conflicts");

    auto is_command_remove = program.is_subcommand_used("remove");
    auto is_command_status = program.is_subcommand_used("status");

    toml::parse_result config_tbl;
    toml::parse_result config_old_tbl;
    try {
        config_tbl = toml::parse_file(
           (std::string) get_config_file_path()
        );
    } catch (const std::exception& e) {
        fmt::println("Fatal: failed to load config: {}", e.what());
        std::exit(EXIT_FAILURE);
    }
    try {
        config_old_tbl = toml::parse_file(
            (std::string) get_state_file_path()
        );
    } catch (const std::exception& e) {
        fmt::println("Fatal: failed to load state file: {}", e.what());
        std::exit(EXIT_FAILURE);
    }

    auto config = load_mappings(config_tbl["general"]);
    auto abs_config = resolve_links_to_absolute(config);
    auto config_old = load_mappings(config_old_tbl["general"]);
    auto abs_config_old = resolve_links_to_absolute(config_old);

    if (is_command_sync) {
        sync(
            is_dry_run_enabled,
            is_force_enabled,
            is_skip_conflicts_enabled,
            abs_config,
            abs_config_old
        );
    } else if (is_command_remove) {
        remove(abs_config_old);
    } else if (is_command_status) {
        status(abs_config, abs_config_old);
    } else {
        std::cout << program << std::endl;
    }

    return 0;
}
