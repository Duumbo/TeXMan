#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

#define ATSIGN 64  // ASCII Value for @


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

	// New Buffer for the tokens.
	char *buf = malloc(8);

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
	// This flag is to check if we have encountered a @ before
	bool flag = false;
	while ((ch = fgetc(source)) != EOF) {
		if (ch == ATSIGN) {  // If we encounter a @, it does not mean we have a token yet.
			if (flag) {  // If we encounter 2 @, it means we have a token.
				while ((ch = fgetc(source)) != ATSIGN && ch != EOF) {  // Let's get every char from the token.
					char curr_char = ch;
					// Add this char to the token.
					strncat(buf, &curr_char, 1);
				}

				// We have the completed Token, we can now replace it with what we want.
				printf("TOKEN FOUND: %s\n", buf);

				// If ch is EOF, it means there was a syntax error in the input file, panic.
				if (ch == EOF) {
					printf("Exiting abnormally, tokeniser failed.");
					break;
				}

				// Start the process up again
				flag = false;
				continue;
			}
			else {  // This was the first @ we encountered, let's loop once more to find another one.
				flag = true;
				continue;
			}
		}
		else {  // Current char is not a @
			if (flag) {  // Let's fix our mistake by putting a @ back.
				fputc(ATSIGN, target);
			}
			// flag should be false, because it is not a suitable token.
			flag = false;
		}
		// Put char without modifying it.
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
	strcat(cwd, "/");

	// Get the template directory
	char template_dir[PATH_MAX];
	strcpy(template_dir, cwd);
	strcat(template_dir, "../templates/");

	// File pointers for the source and the target
	FILE *source, *target;

	// Loop over all the files in the template directory and spawns them in the current directory.

	// Create a namelist for the directory scan.
	struct dirent **namelist;
	// Get the number of subdir in the template dir. Alphanumerically sorted and inside namelist
	int n = scandir(template_dir, &namelist, NULL, alphasort);
	if (n < 0) {
		printf("Error in scanning the template directory.");
		exit(EXIT_FAILURE);
	}

	// Buffer for the source and target
	char source_dir[PATH_MAX];
	char target_dir[PATH_MAX];

	while (n > 2) {
		// Copy the current working dir and the template dir into the buffers.
		strcpy(source_dir, cwd);
		strcpy(target_dir, template_dir);

		// Open the files. namelist is in alphanumerical order, so this loops from z->a->123.
		// This is why this loop is while n > 2, so we don't try to copy over . and ..
		target = fopen(strcat(source_dir, namelist[n - 1]->d_name), "w");
		source = fopen(strcat(target_dir, namelist[n - 1]->d_name), "r");

		// Actually copy the files from source to target
		copy_file(source, target);

		// The dirent struct is on the heap, must free it.
		free(namelist[n - 1]);
		n--;
	}
	free(namelist);

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
