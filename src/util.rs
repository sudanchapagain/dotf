use dirs::home_dir;
use std::path::{Path, PathBuf};

pub fn dotfiles_dir(base_dir: Option<&Path>) -> PathBuf {
    match base_dir {
        Some(base) => base.join(".dotfiles"),
        None => home_dir().unwrap().join(".dotfiles"),
    }
}

pub fn expand_user_path(path: &str, home_dir_override: Option<&Path>) -> PathBuf {
    if let Some(stripped) = path.strip_prefix('~') {
        let home = home_dir_override
            .map(Path::to_path_buf)
            .unwrap_or_else(|| home_dir().unwrap());
        home.join(stripped.trim_start_matches('/'))
    } else {
        PathBuf::from(path)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    #[test]
    fn test_dotfiles_dir() {
        let temp_dir = tempdir().unwrap();
        assert_eq!(
            dotfiles_dir(Some(temp_dir.path())),
            temp_dir.path().join(".dotfiles")
        );
    }

    #[test]
    fn test_expand_user_path() {
        let temp_dir = tempdir().unwrap();
        assert_eq!(
            expand_user_path("~/test", Some(temp_dir.path())),
            temp_dir.path().join("test")
        );
        assert_eq!(
            expand_user_path("/tmp/test", None),
            Path::new("/tmp/test").to_path_buf()
        );
    }
}
