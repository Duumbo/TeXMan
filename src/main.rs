use std::env;
use std::ffi::OsString;
use std::fs;
use std::path::{Path, PathBuf};

const TEMPLATE_DIR: &str = "/home/dumbo/.config/texman/templates/";
const BUFFMAX: usize = 100;
const COLON: u8 = 58;
const LTH: u8 = 60;
const GTH: u8 = 62;

enum SubCommand {
    Spawn,
    Main
}

struct Map {
    key: Vec<u8>,
    val: Vec<u8>
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

fn read_from_template(
    template: &PathBuf
) -> Result<(fs::ReadDir, Vec<OsString>), std::io::Error> {

    let dir_coll = fs::read_dir(template)
        .expect("Template dir incorrect");

    let names = Vec::new();
    for elem in dir_coll {
        names.push(elem?.file_name());
    }

    Ok((dir_coll, names))
}

fn write_to_out() {
    fs::write(out_dir.join("file"), out_buffer)
        .expect("Could not write file.");
}

fn parse_token() {
    let mut is_tag: bool = false;
    let mut found_semi: usize = 0;
    let mut buffer: Vec<u8> = Vec::with_capacity(BUFFMAX);
    let mut out_buffer: Vec<u8> = Vec::new();
    let file_buffer = fs::read(
        file.expect("Could not acces the template directory.")
        .path()
    ).expect("Could not read from file inside template dir.");
    for byte in file_buffer.into_iter() {
        // We are reading a token.
        if is_tag {
            // End of tag
            if byte == GTH {
                is_tag = false;
                let mut val: Vec<u8> = map_token(&buffer[2..], &token_map);
                out_buffer.append(&mut val);
                buffer.clear();
            }
            // omnomnom
            else {
                out_buffer.push(byte);
            }
        // We found a minimum of a semi colon
        } else if found_semi > 0 {
            // We found another one
            if byte == COLON {
                found_semi += 1;
                buffer.push(byte);
            // We found the start of a token.
            } else if found_semi == 2 {
                if byte == LTH {
                is_tag = true;
                buffer.push(byte);
                }
                // This is a fake.
                else {
                    for _ in 0..buffer.len() {
                        // There will always be a char in buffer
                        out_buffer.push(buffer.pop().unwrap());
                    }
                    out_buffer.push(byte);
                }
            // Empty the buffer.
            } else {
                // There will always be a char in buffer here.
                out_buffer.push(buffer.pop().unwrap());
                out_buffer.push(byte);
            }
        // Nothing important.
        } else {
            out_buffer.push(byte);
        }
    }

}

fn spawn(params: &Vec<String>) -> Result<(), std::io::Error>{

    let token_map: Vec<Map> =
        vec![Map{key: vec![108, 111, 108], val: vec![120, 100]}];

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
    let template = Path::new(TEMPLATE_DIR);
    let template = template.join(Path::new(&params[2]));
    let (template_it, file_names) = read_from_template(&template)?;
    for file in template_it {
        parse_token(file, &token_map);
        write_to_out(file);
    }
    Ok(())
}

fn map_token(token: &[u8], map: &[Map]) -> Vec<u8> {
    for n in map {
        if n.key == token {
            return n.val.clone();
        }
    }
    return Vec::new();
}

fn help(kind: SubCommand) {
    match kind {
        SubCommand::Main => {
            println!("Help message. 'main'");
            println!("Usage: texman subcommand args");
        },
        SubCommand::Spawn => {
            println!("Help message. 'spawn'");
            println!("Usage: texman spawn template out_dir");
        }
    }
}
