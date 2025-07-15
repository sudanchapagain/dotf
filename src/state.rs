use crate::util::{dotfiles_dir, expand_user_path};
use std::{
    collections::HashMap,
    fs,
    path::{Path, PathBuf},
};

use serde::{Deserialize, Serialize};

#[derive(Debug, Serialize, Deserialize, Default)]
pub struct State {
    pub state: HashMap<String, String>,
}

pub fn state_path(base_dir: Option<&Path>) -> PathBuf {
    dotfiles_dir(base_dir).join(".state")
}

pub fn load_state(base_dir: Option<&Path>) -> HashMap<String, String> {
    let path = state_path(base_dir);
    fs::read_to_string(path)
        .ok()
        .and_then(|content| toml::from_str::<State>(&content).ok())
        .unwrap_or_default()
        .state
}

pub fn save_state(
    mappings: &HashMap<String, String>,
    base_dir: Option<&Path>,
) -> anyhow::Result<()> {
    let mut state = HashMap::new();
    for (src_rel, dest_str) in mappings {
        let dest_path = expand_user_path(dest_str, base_dir)
            .to_string_lossy()
            .to_string();
        state.insert(dest_path, src_rel.clone());
    }

    let content = toml::to_string(&State { state })?;
    fs::write(state_path(base_dir), content)?;
    Ok(())
}
