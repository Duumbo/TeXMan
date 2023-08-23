pub mod help;
mod parse_config;
pub mod spawn;

// Update whenever a new subcommand is added, to be used with the help message.
pub enum SubCommand {
    Spawn,
    Main,
}
