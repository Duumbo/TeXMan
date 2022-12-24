# TeXMan
### *A LaTeX Template Manager*

## Table of Content
* Goals
* Roadmap

## Goals
This program should manage a library of template for easy use for new project. Motivations for this project was the need to change things on a template too
often. For example, the name on the template can't be always set for the same thing for team homework. These values could be managed by a smart template
with profiles for automatic completion. A template also has skeletons for say a homework question, but you have to copy and paste multiple times for the
accurate number of question in the specific homework. This tool should be able to automatically manage the number of duplicate pages you want in the
template.

## Roadmap
The developpement of this tool requires that I get familiar with handling files in C. I want to use C to get more familiar with this language mostly, but
also because I don't want to paint myself into a corner by using bash if I want to eventually port to Windows. List of thing to implement, in order:
1. User interface.
2. File stream. Being able to copy files from the template folder to anywhere asked, modifing things as asked.
3. Profile interface. Save frequent variables in profiles, json format.
4. Save current folder as template. Configurations saved inside a json file.
