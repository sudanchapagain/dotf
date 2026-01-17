use clap::{Parser, Subcommand};

#[derive(Parser)]
#[command(version, about = "dotf - manage dotfiles with symlinks")]
pub struct Cli {
    /// forcefully link by renaming existing files when conflicts arise
    #[arg(short, long, global = true)]
    pub force: bool,

    /// skip destructive actions while appearing to perform said action
    #[arg(short = 'd', long, global = true)]
    pub dry_run: bool,

    /// skip linking for all conflicts that arise
    #[arg(short = 's', long, global = true)]
    pub skip_conflicts: bool,

    #[command(subcommand)]
    pub command: Option<Command>,
}

#[derive(Subcommand)]
pub enum Command {
    #[command(about = "Link dotfiles from .dotfiles directory to defined paths")]
    Link,
    #[command(about = "Remove linked dotfiles")]
    Remove,
    #[command(about = "Show the status of dotfiles")]
    Status,
}
