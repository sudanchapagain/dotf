use serde::Deserialize;
use std::collections::HashMap;
use std::fs;

use crate::util::dotfiles_dir;

#[derive(Debug, Deserialize)]
struct Mappings {
    general: HashMap<String, String>,
}

pub fn parse_mappings() -> anyhow::Result<HashMap<String, String>> {
    let mappings_path = dotfiles_dir(None).join(".mappings");
    let content = fs::read_to_string(mappings_path)?;
    let parsed: Mappings = toml::from_str(&content)?;
    Ok(parsed.general)
}
