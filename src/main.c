#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help() {
	printf("Called the help function.\n");
	return;
}

void spawn(int argc, char **argv) {
	// Function spawn.
	// This function spawns the correct template from the template directory to the current directory.
	char cwd[PATH_MAX];

	printf("Called '%s' with %d arguments\n", argv[0], argc);

	// File pointers for the source and the target
	FILE *source, *target;

	// Open the files. Here the name of the file will eventually change as the function will loop over the existing file in the templates directory.
	target = fopen(strcat(getcwd(cwd, sizeof(cwd)), "/somefile.txt"), "w");
	source = fopen("../templates/somefile.txt", "r");

	// Check if target open correctly.
	if (target == NULL) {
		printf("Error opening output file\n Exiting...\n");
		exit(EXIT_FAILURE);
	}
	// Check if source open correctly.
	if (source == NULL) {
		printf("Error opening input file\n Exiting...\n");
		exit(EXIT_FAILURE);
	}

	// Copying the file to the output.
	int ch;
	while ((ch = fgetc(source)) != EOF) {
		fputc(ch, target);
	}

	// Closing input and output.
	fclose(source);
	fclose(target);
	return;
}

void save(int argc, char **argv) {
	// Function save.
	// This function saves the current directory as a template.

	printf("Called '%s' with %d arguments\n", argv[0], argc);
	return;
}

int main(int argc, char **argv) {

	printf("Number of arguments: %d\n", argc);
	printf("Program name is: %s\n", argv[0]);

	// Call the help function if no arguments are passed to the program.
	if (argc == 1) {
		help();
		return 0;
	}
	printf("Argument passed: %s\n", argv[1]);

	// Check for correct keywords passed as arguments.
	if (strcmp(argv[1], "spawn") == 0) {
		char *to_spawn[] = {"spawn"};
		spawn(argc - 1, to_spawn);
	}
	else if (strcmp(argv[1], "save") == 0) {
		char *to_save[] = {"save"};
		save(argc - 1, to_save);
	}
	else {
		help();
	}
	
	return 0;
}
