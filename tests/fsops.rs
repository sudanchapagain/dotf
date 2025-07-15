use dotf::*;
use std::collections::HashMap;
use std::fs;
use tempfile::tempdir;

#[test]
fn test_link_remove_status() -> anyhow::Result<()> {
    // 1. Setup
    let temp_dir = tempdir()?;
    let home_dir = temp_dir.path();
    let dotfiles_dir = home_dir.join(".dotfiles");
    fs::create_dir(&dotfiles_dir)?;

    let mut mappings = HashMap::new();
    mappings.insert("vimrc".to_string(), "~/.vimrc".to_string());

    let vimrc_content = "set nu";
    fs::write(dotfiles_dir.join("vimrc"), vimrc_content)?;

    // 2. Link files
    fsops::link_files(&mappings, false, false, Some(home_dir))?;
    let dest_path = home_dir.join(".vimrc");
    assert!(dest_path.is_symlink());
    assert_eq!(fs::read_to_string(dest_path)?, vimrc_content);

    // 3. Check status
    fsops::status(&mappings, Some(home_dir))?;

    // 4. Remove links
    let state = state::load_state(Some(home_dir));
    fsops::remove_links(&state, Some(home_dir))?;
    assert!(!home_dir.join(".vimrc").exists());

    Ok(())
}
