use clap::{Parser, Subcommand};
use colored::Colorize;
use toml::Table;
use regex::Regex;

use std::env;
use std::fs;
use std::fs::File;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::iter::zip;

#[derive(Parser)]
#[command(author, version, about, long_about = None)]
pub struct Cli {
    /// For debugging purposes. Maximum verbosity.
    #[arg(short, long)]
    verbose: bool,

    /// Supresses all output.
    #[arg(short, long)]
    quiet: bool,

    #[command(subcommand)]
    command: Option<Commands>,
}

#[derive(Subcommand)]
enum Commands {
    /// Spawns a template and replaces according to the profile.
    Spawn {
        /// The Template to use.
        template: String,

        /// The output directory.
        out_dir: String,

        /// The profile to use.
        #[arg(short, long, default_value_t = String::from("default"))]
        profile: String,
    },

    /// Returns info on the existing templates
    Template {
        /// Lists all available templates.
        #[arg(short, long)]
        list: bool,

        /// Get all available profiles for the template.
        #[arg(short, long, value_name = "template", default_value_t = String::from(""))]
        profiles: String,

    }
}

fn main() -> Result<(), Box<dyn std::error::Error>>{
    let cli = Cli::parse();

    // Parse the config file
    let texman_config = config();

    // Parse the subcommand.
    if let Some(sub_command) = cli.command {
        match sub_command {
            Commands::Spawn {template, out_dir, profile} => spawn(&vec![template, out_dir, profile], texman_config)?,
            Commands::Template { list, profiles } => temp_info(list, profiles, texman_config)?,
        };
    };

    Ok(())
}

fn spawn(params: &Vec<String>, configs: Table) -> Result<(), std::io::Error> {
    // Parse the template directory
    let template_dir = configs["template_dir"]
        .as_str()
        .expect("Error in parsing template_dir.");
    let template = PathBuf::from(template_dir);
    let template = template.join(Path::new(&params[0]));

    // Read the profile directory
    let profile_file = template.clone();
    let profile_file = profile_file.join(PathBuf::from(format!("{}.profile", params[2])));
    let token_buffer = fs::read(profile_file.clone())
        .expect(&format!("Profile does not exist. Check for {:?}", profile_file));
    // texmanrc should be toml
    let token_map = String::from_utf8(token_buffer)
        .unwrap()
        .parse::<Table>()
        .unwrap();

    // Create out_dir (param n3).
    let out_dir = Path::new(&params[1]);
    fs::create_dir_all(&out_dir).expect("Could not create out_dir");

    // Read and write the template_dir one file at the time.
    let (template_it, fnames) = read_from_template(&template)?;
    for (file, file_names) in zip(template_it, fnames) {
        if let Some(ext) = file_names.extension() {
            if ext == "profile" {continue;}
        }
        let fb: Vec<u8> = fs::read(file)?;
        let fb = match std::str::from_utf8(&fb) {
            Ok(s) => s,
            Err(_) => {
                println!("Warning: File encoding is not UTF-8 for {:?}. Skipping patern matching.", &file_names);
                let mut out_file = File::create(&out_dir.join(&file_names))?;
                out_file.write_all(&fb)?;
                continue;
            },
        };
        let out_buffer = parse_token(&fb, &token_map);
        write_to_out(&out_dir.join(&file_names), &out_buffer);
    }
    Ok(())
}

fn read_from_template(template: &PathBuf) -> Result<(Vec<PathBuf>, Vec<PathBuf>), std::io::Error> {
    let dir_coll = fs::read_dir(template).expect("Template dir incorrect");

    let mut names = Vec::new();
    let mut dir = Vec::new();
    for elem in dir_coll {
        let e = elem?.path();
        dir.push(e.clone());
        names.push(PathBuf::from(e.file_name().unwrap()));
    }

    Ok((dir, names))
}

fn write_to_out(out_path: &PathBuf, out_buffer: &str) {
    fs::write(out_path, out_buffer).expect("Could not write file.");
}

fn parse_token(file_buffer: &str, map: &Table) -> String {
    // Pattern Definition
    let pattern = Regex::new(
        match map.get("regex") {
            Some(value) => value.as_str().unwrap(),
            None => panic!("The key `regex` is required in the profile file"),
        }
    ).expect("Recieved an illegal regular expression");

    // Copy of file_buffer
    let mut out_buffer = file_buffer.clone().to_owned();

    // Get replace matchs.
    for token in pattern.captures_iter(file_buffer) {
        match map.get(&token[1]) {
            // Replace the token by its value if defined
            Some(value) => out_buffer = pattern.replace(&out_buffer, value.as_str().unwrap()).to_string(),
            // Replace the token by its name if undefined
            None => out_buffer = pattern.replace(&out_buffer, &token[1]).to_string(),
        };
    }
    out_buffer
}

fn temp_info(list: bool, template: String, conf: Table) -> Result<(), Box<dyn std::error::Error>> {
    if list {
        // Parse the template directory
        let template_dir = conf["template_dir"]
            .as_str()
            .expect("Error in parsing template_dir.");

        // Print all Templates
        let available_templates =
            fs::read_dir(template_dir).expect("Template Directory does not exists");
        println!("{}", "Available Templates:".bold());
        for existing_template in available_templates {
            println!(
                "\t{:?}",
                existing_template
                    .expect("Problem parsing file name")
                    .file_name()
            );
        }

        // Return now, jobs done
        return Ok(());
    }

    if template == "" {
        println!("Missing template to read profiles from.");
        return Ok(());
    }
    // Parse the template directory
    let template_dir = conf["template_dir"]
        .as_str()
        .expect("Error in parsing template_dir.");
    let template_dir = PathBuf::from(template_dir);
    let template_path = template_dir.join(PathBuf::from(template));
    let available_profiles =
        fs::read_dir(template_path).expect("Template provided does not exists.");
    println!("{}", "Available Profiles:".bold());

    for existing_file in available_profiles {
        let fp = existing_file?.path();
        if let Some(extension) = fp.extension() {
            if extension == "profile" {
                println!("\t{:?}",
                         fp.file_name().unwrap())
            };
        }
    }

    Ok(())
}

fn config() -> Table {
    // Parse the config file. Default path is $XDG_CONFIG_HOME/texman/texman.toml
    let var = env::var("XDG_CONFIG_HOME");
    let xdg_config_home: PathBuf = match var {
        Ok(val) => PathBuf::from(&val),
        Err(_) => {
            // If XDG_CONFIG_HOME is not set, then assume the default value.
            let var = env::var("HOME").expect("Home environnement variable not set.");
            Path::new(&var).join(Path::new(".config"))
        }
    };
    let config_buffer = fs::read(xdg_config_home.join(Path::new("texman/texman.toml")))
        .expect("TeXMan config not found.");
    // texmanrc should be toml
    let parsed_config = String::from_utf8(config_buffer)
        .unwrap()
        .parse::<Table>()
        .unwrap();
    parsed_config
}
