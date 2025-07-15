use std::{
    collections::HashMap,
    fs::{self},
    io::{self},
    os::unix::fs::symlink,
    path::Path,
};

use crate::{
    state::{load_state, save_state},
    util::{dotfiles_dir, expand_user_path},
};
use colored::*;

pub fn link_files(
    mappings: &HashMap<String, String>,
    force: bool,
    dry_run: bool,
    base_dir: Option<&Path>,
) -> anyhow::Result<()> {
    for (src_rel, dest_str) in mappings {
        let src = dotfiles_dir(base_dir).join(src_rel);
        let dest = expand_user_path(dest_str, base_dir);

        if dest.exists() {
            if !force && !prompt_overwrite(&dest) {
                println!("Skipped: {}", dest.display());
                continue;
            } else if force {
                backup_existing(&dest, dry_run);
            }
        }

        if dry_run {
            println!(
                "[Dry-run] Would link: {} -> {}",
                src.display(),
                dest.display()
            );
        } else {
            if let Some(parent) = dest.parent() {
                fs::create_dir_all(parent)?;
            }

            symlink(&src, &dest)?;
            println!("Linked: {} -> {}", src.display(), dest.display());
        }
    }

    if !dry_run {
        save_state(mappings, base_dir)?;
    }

    Ok(())
}

pub fn remove_links(
    state: &HashMap<String, String>,
    base_dir: Option<&Path>,
) -> anyhow::Result<()> {
    for dest_str in state.keys() {
        let dest = Path::new(dest_str);
        if dest.is_symlink() {
            fs::remove_file(dest)?;
            println!("Removed: {}", dest.display());
        } else {
            println!("Not a symlink: {}", dest.display());
        }
    }

    fs::write(crate::state::state_path(base_dir), "")?;
    Ok(())
}

pub fn status(mappings: &HashMap<String, String>, base_dir: Option<&Path>) -> anyhow::Result<()> {
    let state = load_state(base_dir);
    let mut seen = HashMap::new();

    for (src_rel, dest_str) in mappings {
        let src = dotfiles_dir(base_dir).join(src_rel);
        let dest = expand_user_path(dest_str, base_dir);
        let dest_str = dest.to_string_lossy().to_string();

        seen.insert(dest_str.clone(), true);

        if dest.exists() {
            if dest.is_symlink() && fs::read_link(&dest)? == src {
                println!("{} {} -> {}", "[OK]".green(), dest.display(), src.display());
            } else {
                println!("{} {}", "[Mismatch]".red(), dest.display());
            }
        } else {
            println!("{} {}", "[Missing]".red(), dest.display());
        }
    }

    for (dest_str, _) in state {
        if !seen.contains_key(&dest_str) && Path::new(&dest_str).is_symlink() {
            println!("{} {}", "[Orphan]".yellow(), dest_str);
        }
    }

    Ok(())
}

fn backup_existing(dest: &Path, dry_run: bool) {
    let mut suffix = 1;
    let mut backup_path;

    loop {
        backup_path = dest.with_extension(format!(
            "bak{}",
            if suffix == 1 {
                "".to_string()
            } else {
                suffix.to_string()
            }
        ));
        if !backup_path.exists() {
            break;
        }
        suffix += 1;
    }

    if dry_run {
        println!(
            "[Dry-run] Would back up {} to {}",
            dest.display(),
            backup_path.display()
        );
    } else {
        fs::rename(dest, &backup_path).unwrap_or_else(|e| {
            eprintln!("Failed to back up {}: {}", dest.display(), e);
        });
        println!("Backed up {} to {}", dest.display(), backup_path.display());
    }
}

fn prompt_overwrite(dest: &Path) -> bool {
    println!("Destination {} already exists.", dest.display());
    println!("  [s]kip, [b]ackup, [f]orce overwrite, [a]bort?");
    let mut input = String::new();
    io::stdin().read_line(&mut input).unwrap();
    match input.trim() {
        "s" => false,
        "b" => true,
        "f" => {
            fs::remove_file(dest).unwrap_or_else(|e| {
                eprintln!("Failed to remove {}: {}", dest.display(), e);
            });
            true
        }
        "a" => std::process::exit(1),
        _ => false,
    }
}
