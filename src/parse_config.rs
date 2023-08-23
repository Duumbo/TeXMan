use std::env::var;
use toml::Table;

pub fn template_dir(config_file: &Table) -> String {
    let home_dir = var("HOME").unwrap();
    match config_file.get("template_dir") {
        Some(s) => s.as_str().unwrap().into(),
        None => format!("{}/{}", &home_dir, "Templates/"),
    }
}

pub fn profile_dir(config_file: &Table) -> String {
    let home_dir = var("HOME").unwrap();
    match config_file.get("profile_dir") {
        Some(s) => s.as_str().unwrap().into(),
        None => format!("{}/{}", &home_dir, "Profiles/"),
    }
}

pub fn default_profile(config_file: &Table) -> String {
    match config_file.get("default_profile") {
        Some(s) => s.as_str().unwrap(),
        None => "default",
    }
    .into()
}
