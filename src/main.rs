use texman::help::help;
use texman::spawn::spawn;
use texman::SubCommand;

use toml::Table;

use std::env;
use std::fs;
use std::path::{Path, PathBuf};

fn main() {
    // Get command line arguments.
    let args: Vec<String> = env::args().collect();
    let n_args = args.len();

    // Parse the config file
    let texman_config = config();

    // Call the main help function.
    if n_args < 2 {
        return help(SubCommand::Main, texman_config);
    }

    // Parse the subcommand.
    let subcommand = &args[1];
    match subcommand.as_str() {
        "spawn" => spawn(&args, texman_config).unwrap(),
        _ => help(SubCommand::Main, texman_config),
    }
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
        .expect("Texman config not found.");
    // texmanrc should be toml
    let parsed_config = String::from_utf8(config_buffer)
        .unwrap()
        .parse::<Table>()
        .unwrap();
    parsed_config
}
