#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void help() {
	/* Function help
	 * Displays the help message.
	 * */
	printf("Called the help function.\n");
	return;
}

int copy_file(FILE *source, FILE *target) {
	/* Function copy_file.
	Copies file from source to target and closes the file.
	FILE *source: file pointer to the source file.
	FILE *target: file pointer to the target file.

	CLOSES the file before returning.
	*/

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
	return 0;

}

void spawn(int argc, char **argv) {
	/* Function spawn.
	This function spawns the correct template from the template directory to the current directory.
	argc: Number of argument passed.
	argv: Argument vector passed from the command line.
	*/
	
	// Get the current working directory
	char buf[PATH_MAX];
	char *cwd = getcwd(buf, sizeof(buf));

	printf("Called '%s' with %d arguments\n", argv[0], argc);

	// File pointers for the source and the target
	FILE *source, *target;

	// Open the files. Here the name of the file will eventually change as the function will loop over the existing file in the templates directory.
	target = fopen(strcat(cwd, "/somefile.txt"), "w");
	source = fopen("../templates/somefile.txt", "r");

	copy_file(source, target);

	return;
}

void save(int argc, char **argv) {
	/* Function save.
	This function saves the current directory as a template.
	argc: Number of argument passed.
	argv: Argument vector passed from the command line.
	*/

	printf("Called '%s' with %d arguments\n", argv[0], argc);
	return;
}

int main(int argc, char **argv) {
	/* Function main.
	 * Provides basic UI in the command line.
	 * */

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
