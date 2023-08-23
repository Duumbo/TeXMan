use crate::parse_config;
use crate::SubCommand;

use colored::Colorize;
use toml::Table;

use std::fs::read_dir;

fn general_flags() {
    item("-q", "Forces the output to be quiet.");
    item(
        "-v",
        "Forces the output to be verbose. Only one level of verbose, maximum verbosity.",
    );
}

fn title(s: &str) {
    println!("");
    println!("{}", s.bold());
    println!("");
}

fn item(s: &str, desc: &str) {
    println!("\t{}, {}", s.cyan(), desc);
}

fn new_line() {
    println!("");
}

fn program_description() {
    println!("TeXMan is a template manager for LaTeX or other types of files.");
    println!("Its goal is to be a highly customizable productivity tool.");
}

pub fn help(kind: SubCommand, config_file: Table) {
    let template_dir = parse_config::template_dir(&config_file);
    let profile_dir = parse_config::profile_dir(&config_file);
    let default_profile = parse_config::default_profile(&config_file);
    match kind {
        SubCommand::Main => {
            program_description();
            println!("Help message. 'main'");

            title("Usage");
            println!(
                "\t{} {} {} {}",
                "texman".cyan(),
                "subcommand".cyan(),
                "-[flags]".cyan(),
                "args".italic().cyan()
            );

            title("Available Subcommands".into());
            item(
                "spawn",
                "Spawns a template in a desired directory using a profile.",
            );

            title("General Flags".into());
            general_flags();
        }
        SubCommand::Spawn => {
            program_description();
            println!("Help message. 'spawn'");

            title("Usage");
            println!(
                "\t{} {} {} {} {}",
                "texman",
                "spawn",
                "-[flags]".cyan(),
                "templates".italic().cyan(),
                "out_dir".italic().cyan()
            );
            new_line();
            println!("The template directory is: {}", template_dir);
            println!("The profile directory is: {}", profile_dir);
            println!("The default profile is: {}", default_profile);
            let available_templates =
                read_dir(template_dir).expect("Template Directory does not exists");
            title("Available Templates");
            for existing_template in available_templates {
                println!(
                    "\t{:?}",
                    existing_template
                        .expect("Problem parsing file name")
                        .file_name()
                );
            }

            title("Flags");
            general_flags();
            item("-p", "Overrides default profile.");
        }
    }
}
