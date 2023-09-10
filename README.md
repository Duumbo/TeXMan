<div align="center">

# TeXMan

*TeXMan* is a [LaTeX](https://www.latex-project.org/) template manager. Its goal
is to a customizable template manager to quickly create documents.

![LaTeX](https://img.shields.io/badge/latex-%23008080.svg?style=for-the-badge&logo=latex&logoColor=white) ![Rust](https://img.shields.io/badge/rust-%23000000.svg?style=for-the-badge&logo=rust&logoColor=white)

![LICENSE](https://img.shields.io/github/license/Duumbo/TeXMan?color=blue&style=for-the-badge) ![release](https://img.shields.io/github/v/tag/Duumbo/TeXMan?color=%23FF7F50&style=for-the-badge)

</div>

# Table of contents

[Configuration](#configuration)

[Creating a profile](#creating-a-profile)

<details>
  <summary><a href="#installation">Installation</a></summary>
  <OL, TYPE="square">
    <li><a href="#requirements">Requirements</a></li>
    <li><a href="#arch-linux">Arch Linux</a></li>
    <li><a href="#other-unix-systems">Other Unix Systems</a></li>
  </OL>
</details>

[Quick start](#quick-start)

[Features](#features)

[TODO](#todo)

[Credits](#credits)

## Configuration
The configuration file is placed in `$XDG_CONFIG_HOME/texman/texman.toml`. Here
is a sample configuration file.
```
template_dir = "$HOME/Documents/Templates"
```
There will be a default in `/etc/texman.conf` soon.

# Creating a profile
Profiles are the way you can reuse templates that are very frequent. For example,
I have a profile set for different classes with different but static teammates.
I is convienient to simply call *TeXMan* and have my homework ready to go.

Profile files use the [TOML](https://toml.io/en/) syntax. The only required key
is the [Regex](https://en.wikipedia.org/wiki/Regular_expression) to parse token
with. Ex.
```toml
regex = "<<(.*?)>>"
```
will match `<<TOKEN>>`. You can then add all the tokens you want as variable inside
the profile file. If *TeXMan* matches a `<<TOKEN>>` but with no defined variable
inside the profile file, it will simply output `TOKEN` in its place.

#### Example Profile
```toml
regex = "<<(.*?)>>"
NAME1 = "Dimitri Bonanni-Surprenant"
NAME2 = "Teammate"
CLASS = "Relativité Générale"
CODE = "PHQ615"
PROF = "The Teacher"
UNIVERSITY = "The university"
EMBLEM = "The Emplem File to use"
FACULTY = "Science"
DEPARTEMENT = "Physics"
```
Variable names could be lower case.

# Installation

### Requirements
*TeXMan* is written in [Rust](https://www.rust-lang.org/), a Rust compiler is
needed. [Cargo](https://doc.rust-lang.org/cargo/) is the recommended way to
compile this project, as there are many dependencies inside the `Cargo.toml`.

### Arch Linux
For Arch users, there is a `PKGBUILD` available in the repository. For instruction
on how to use such file, please refer to [the arch-linux wiki](https://wiki.archlinux.org/title/Arch_User_Repository).
The file
available on the main branch main be outdated with the wrong version number and
md5sums, but there are tested ones in each release version. If you wish to use
the developpement branch, simply update version in the `Cargo.toml` and `publish.sh`,
then run the `publish.sh` script. This will build the relevant archive to make
the package.

### Other Unix Systems
To install *TeXMan* on a Unix based system, simply
```bash
make
sudo make DESTDIR="/" install
```
To understand where this installs the program, you can refer to the Makefile.
You can specify another place to install *TeXMan* by specifying another `DESTDIR`.
This package will not be followed by your package manager when installed this way.

# Quick start

Once [installation](#installation) properly done, verify that `texman` is in your
path. Then, you can use *TeXMan* with templates present in your specified template
directory.

Assuming you have the template `homework` inside your template directory, with
profiles `default.profile` and `quantum.profile`;

- You can apply the template to a directory with the default profile
```bash
texman spawn homework <OUT_DIR>
```

- You can apply the profile `quantum`
```bash
texman spawn homework <OUT_DIR> -p quantum
```

# Features

- You can list the available templates inside your template directory with
```bash
texman template -l
```
```
Available Templates:
        "homework"
```

- You can list the available profiles inside the `homework` template with
```bash
texman template -p homework
```
```
Available Profiles:
        "default.profile"
        "quantum.profile"
```

# TODO

- [x] Have a customizable regex to replace token.
- [] Have sensible defaults in `/etc/texman.conf`
- [] Support parameter tokens.
- [] Support variable number of file. (Ex. the are 3 questions in a homework, spawn 3 files.)

# Credits
