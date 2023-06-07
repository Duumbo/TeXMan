mod help;
use help::help;

mod spawn;
use spawn::spawn;

use std::env;

const TEMPLATE_DIR: &str = "/home/dumbo/.config/texman/templates/";

pub enum SubCommand {
    Spawn,
    Main
}

fn main() {

    // Get command line arguments.
    let args: Vec<String> = env::args().collect();
    let n_args = args.len();

    // Call the main help function.
    if n_args < 2 {
        return help(SubCommand::Main)
    }

    // Parse the subcommand.
    let subcommand = &args[1];
    match subcommand.as_str() {
        "spawn" => spawn(&args).unwrap(),
        _ => help(SubCommand::Main),
    }
}

