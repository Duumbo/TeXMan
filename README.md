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

# Installation

### Requirements

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

- [x] 'Classic' template
- [x] 'Homework' template (Overleaf's rebuild)
- [x] 'Cover Letter' template
- [ ] 'RevTeX' template
- [ ] 'Beamer' template
- [x] Install/uninstall scripts
- [x] Version control
- [ ] REAME.md
- [ ] Add all well explained commands and features
  - [ ] Supported colors
  - [ ] Supported Universities (emblems + colors)
- [ ] Setup default pdf viewer based on OS
- [ ] Rebuild 'Homework' template's `quesiton{}{}` command as newenvironnement

# Credits
