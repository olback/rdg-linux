use serde::{Serialize, Deserialize};
use dirs;
use std::{fs, collections::HashMap, path::PathBuf};
use crate::{error::RdgResult, settings::Settings};
use serde_json;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Profile {
    pub host: String,
    pub port: u16,
    pub username: String,
    pub domain: String,
    pub width: u32,
    pub height: u32,
    pub keymap: String,
    pub experience: String,
    pub color_depth: u8,
    pub fullscreen: bool,
    pub disable_encryption: bool,
    pub compression: bool,
    pub cache_bitmaps: bool,
    pub sync_numlock: bool,
    pub extra_args: String
}

impl Profile {

    pub fn get_connect_args(&self, settings: &Settings) -> Vec<String> {

        let mut args = Vec::<String>::new();

        // Username
        let username = self.username.trim();
        if !username.is_empty() {
            args.push(format!("-u {}", username));
        }

        // Domain
        let domain = self.domain.trim();
        if !domain.is_empty() {
            args.push(format!("-d {}", domain));
        }

        // Window geometry
        args.push(format!("-g {}x{}", self.width, self.height));

        // Keymap
        args.push(format!("-k {}", settings.keymap_path.join(&self.keymap).display()));

        // Experience
        args.push(format!("-x {}", self.experience));

        // Color depth
        args.push(format!("-a {}", self.color_depth));

        // Fullscreen
        if self.fullscreen {
            args.push("-f".into());
        }

        // Disable encryption
        if self.disable_encryption {
            args.push("-E".into());
        }

        // Compression
        if self.compression {
            args.push("-z".into());
        }

        // Cache bitmaps
        if self.cache_bitmaps {
            args.push("-P".into());
        }

        // Numlock sync
        if self.sync_numlock {
            args.push("-N".into());
        }

        // Extra args
        let extra_args = self.extra_args.trim();
        if !extra_args.is_empty() {
            args.push(extra_args.into());
        }

        // Host & Port
        args.push(format!("{}:{}", self.host, self.port));

        args

    }

}

#[derive(Debug, Clone)]
pub struct Profiles {
    profiles: HashMap<String, Profile>,
    path: PathBuf
}

impl Profiles {

    pub fn load() -> RdgResult<Self> {

        let conf_dir = dirs::config_dir().expect("config dir does not exist").join("rdg");
        let profiles_path = conf_dir.join("profiles.json");

        if !conf_dir.exists() {
            fs::create_dir_all(&conf_dir)?;
        }

        if !profiles_path.exists() {
            fs::write(&profiles_path, "{}\n")?;
        }

        let profiles_data = fs::read_to_string(&profiles_path)?;

        let profiles: HashMap<String, Profile> = serde_json::from_str(&profiles_data)?;

        Ok(Self {
            profiles: profiles,
            path: profiles_path
        })

    }

    pub fn insert(&mut self, profile: Profile) -> RdgResult<()> {

        self.profiles.insert(profile.host.clone(), profile);
        self.save()?;
        Ok(())

    }

    pub fn get<K: Into<String>>(&self, key: K) -> Option<&Profile> {

        self.profiles.get(&key.into())

    }

    pub fn iter(&self) -> std::collections::hash_map::Iter<String, Profile> {

        self.profiles.iter()

    }

    fn save(&self) -> RdgResult<()> {

        let profiles_str = serde_json::to_string_pretty(&self.profiles)?;
        fs::write(&self.path, profiles_str)?;

        Ok(())

    }

}
