# TeXMan
### *A LaTeX Template Manager* (wip)

## Table of Content
* Installation
* Usage
* Configuration
* Roadmap

## Installation
Installation requires `cargo`. To install, use the `Makefile`
```bash
make
sudo make DESTDIR="/" install
```

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
Next step is to create sensible defaults in `/etc/texman.conf`.
Options in `$XDG_CONFIG_HOME/texman/texman.conf` should always override those.
