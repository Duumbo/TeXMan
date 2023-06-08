use crate::SubCommand;
use crate::help;
use std::collections::HashMap;
use std::fs;
use std::iter::zip;
use std::path::{Path, PathBuf};
use regex::Regex;
use toml::Table;

pub fn spawn(params: &Vec<String>, configs: Table) -> Result<(), std::io::Error>{

    let mut token_map: HashMap<String, String> =
        HashMap::new();
    token_map.insert("lol".to_string(), "xd".to_string());

    // Parse the template directory
    let template_dir = configs["template_dir"].as_str()
        .expect("Error in parsing template_dir.");

    // If number of param is incorrect, return help message.
    if params.len() < 4 {
        help(SubCommand::Spawn);
        return Ok(());
    }

    // Create out_dir (param n3).
    let out_dir = Path::new(&params[3]);
    fs::create_dir_all(&out_dir)
        .expect("Could not create out_dir");

    // Read and write the template_dir one file at the time.
    let template = Path::new(template_dir);
    let template = template.join(Path::new(&params[2]));
    let (template_it, fnames) = read_from_template(&template)?;
    for (file, file_names) in zip(template_it, fnames) {
        let fb: Vec<u8> = fs::read(file)?;
        let fb = match std::str::from_utf8(&fb){
            Ok(s) => s,
            Err(_) => panic!("Error in encoding file buffer."),
        };
        let out_buffer = parse_token(&fb, &token_map);
        write_to_out(&out_dir.join(&file_names), &out_buffer);
    }
    Ok(())
}

fn read_from_template(
    template: &PathBuf
) -> Result<(Vec<PathBuf>, Vec<PathBuf>), std::io::Error> {

    let dir_coll = fs::read_dir(template)
        .expect("Template dir incorrect");

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
    fs::write(out_path, out_buffer)
        .expect("Could not write file.");
}

fn parse_token(file_buffer: &str, map: &HashMap<String, String>) -> String {
    // Pattern Definition
    let pattern = Regex::new("::<(.*?)>").unwrap();

    // Copy of file_buffer
    let mut out_buffer = file_buffer.clone().to_owned();

    // Get replace matchs.
    for token in pattern.captures_iter(file_buffer) {
        match map.get(&token[1]) {
            // Replace the token by its value if defined
            Some(value) => out_buffer = pattern.replace(&out_buffer, value).to_string(),
            // Replace the token by its name if undefined
            None => out_buffer = pattern.replace(&out_buffer, &token[1]).to_string(),
        };
    }
    out_buffer
}

