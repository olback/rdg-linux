use serde::{Serialize, Deserialize};
use serde_json;
use dirs;
use std::{fs, path::PathBuf};
use crate::error::RdgResult;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Settings {
    pub allow_untrusted_cert: bool,
    pub rdesktop_path: PathBuf,
    pub keymap_path: PathBuf
}

impl Settings {

    pub fn load() -> RdgResult<Self> {

        let conf_dir = dirs::config_dir().expect("config dir does not exist").join("rdg");
        let settings_path = conf_dir.join("settings.json");

        if !conf_dir.exists() {
            fs::create_dir_all(&conf_dir)?;
        }

        if !settings_path.exists() {
            let default = Self::default();
            let data_str = serde_json::to_string_pretty(&default)?;
            fs::write(&settings_path, &data_str)?;
            return Ok(default);
        }

        let data_str = fs::read_to_string(&settings_path)?;
        let settings: Self = serde_json::from_str(&data_str)?;

        Ok(settings)

    }

    pub fn save(&self) -> RdgResult<()> {

        let conf_dir = dirs::config_dir().expect("config dir does not exist").join("rdg");
        let settings_path = conf_dir.join("settings.json");

        if !conf_dir.exists() {
            fs::create_dir_all(&conf_dir)?;
        }

        let settings_str = serde_json::to_string_pretty(&self)?;
        fs::write(&settings_path, settings_str)?;

        Ok(())

    }

}

impl Default for Settings {

    fn default() -> Self {

        Self {
            allow_untrusted_cert: true,
            rdesktop_path: PathBuf::from("/usr/bin/rdesktop"),
            keymap_path: PathBuf::from("/usr/share/rdesktop/keymaps")
        }

    }

}
