# TeXMan
### *A LaTeX Template Manager* (wip)

## Table of Content
* Goals
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
