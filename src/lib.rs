pub mod cli;
pub mod config;
pub mod fsops;
pub mod state;
pub mod util;

use clap::Parser;
use cli::Cli;
use config::parse_mappings;
use fsops::*;
use state::*;
use std::process;

pub fn run() -> anyhow::Result<()> {
    let cli = Cli::parse();
    let mappings = parse_mappings()?;

    match cli.command {
        Some(cli::Command::Link) => link_files(&mappings, cli.force, cli.dry_run, None)?,
        Some(cli::Command::Remove) => {
            let state = load_state(None);
            remove_links(&state, None)?;
        }
        Some(cli::Command::Status) => status(&mappings, None)?,
        None => {
            eprintln!("usage: dotf [options] [command]");
            process::exit(1);
        }
    }

    Ok(())
}
