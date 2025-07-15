use clap::{Parser, Subcommand};

#[derive(Parser)]
#[command(version, about = "dotf - manage dotfiles with symlinks")]
pub struct Cli {
    #[arg(short, long, global = true)]
    pub force: bool,

    #[arg(short = 'd', long, global = true)]
    pub dry_run: bool,

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
