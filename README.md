# TeXMan
### *A LaTeX Template Manager* (wip)

## Table of Content
* Installation
* Usage
* Configuration
* Roadmap

## Installation
No install script exists for the time being. If you want to use it, you can
build it using cargo.

## Usage
To spawn a template, use the `spawn` subcommand.
```
spawn <template> <out_dir>
```

## Configuration
The configuration file is placed in `$XDG_CONFIG_HOME/texman/texman.toml`. Here
is a sample configuration file.
```
template_dir = "$HOME/Documents/Templates"
```

## Roadmap
Currently, the project can read a template directory and outputs it in the
correct directory. Profiles are needed to be created. They need to pack the
configurations:
* Tokens and their values.
* Override the regex to use. This is needed because not all languages can support
the same token syntax.
