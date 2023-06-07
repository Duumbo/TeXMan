use crate::SubCommand;

pub fn help(kind: SubCommand) {
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
