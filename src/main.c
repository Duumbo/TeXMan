#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

#define ATSIGN 64  // ASCII Value for @
#define NEWLINE 10 // ASCII value for \n
#define COLON 58  // ASCII Value for :
#define PROFILESPATH "/home/dumbo/Documents/Projets/TeXMan/profiles/"
#define TEMPLATEPATH "/home/dumbo/Documents/Projets/TeXMan/templates/"
#define PROFILELEN 46


void help() {
	/* Function help
	 * Displays the help message.
	 * */
	printf("Called the help function.\n");
	return;
}


int get_profile(char *profile, char** key, char** val, int** token_offset, int** val_offset) {
    // Returns tokens and values from profile file.
    // Output: key, val, token_offset, val_offset, n_of_keys
	// Open the desired profile.
	FILE *profile_file;
	char *extension = ".profile";
	strcat(profile, extension);
    int prof_file_len = strlen(profile);
	char *profile_path = malloc((PROFILELEN + prof_file_len) * sizeof(char));  // The profile path is determined by the constant for no
	strncpy(profile_path, PROFILESPATH, PROFILELEN);
	strcat(profile_path, profile);
	printf("Accessing %s\n", profile_path);
	profile_file = fopen(profile_path, "r");

	// Check if file exist. If it does not, return.
	if (profile_file == NULL) {
		printf("Invalid profile, %s is not defined.\n", profile);
		return 0;
	}

	// Read until newline or EOF
    int n_of_keys = 0;
    int len_of_key = 0;
    int len_of_val = 0;
	bool flag = false;
	int ch;
	int i;
	while ((ch = fgetc(profile_file)) != EOF) {
		if (ch != NEWLINE) {
			// Check if we need to switch for the value.
			if (ch == COLON) {
				flag = true;
				i = 0;
				continue;
			}

			// This branch is when we a filling up val
			if (flag) {
				// Point to the correct place in memory for value.
				char *curr_val = *val + i * sizeof(char);
				*curr_val = ch;
				i++;
                len_of_val++;
			}
			else{  // This branch is when we are filling up key

				// Convert ch from byte to char
				char curr_char = ch;
				strncat(*key, &curr_char, 1);
                len_of_key++;
			}

		}
		else {  // Here we have finished filling up our key and val, let's process them.
			// Set the flag back to false so we start by filling key up.
			flag = false;
            // Reallocate val and key to match true length
            *val = realloc(*val, (len_of_val + 100) * sizeof(char));
            *key = realloc(*key, (len_of_key + 100) * sizeof(char));
            int *curr_offset = *token_offset + n_of_keys * sizeof(int);
            int *curr_offset_val = *val_offset + n_of_keys * sizeof(int);
            n_of_keys++;
            *curr_offset = len_of_key;
            *curr_offset_val = len_of_val;

		}
	}

	// Let's process one last time our key and val.


	// Close the file.
	fclose(profile_file);
	return n_of_keys;
}


int copy_file(
        FILE *source,
        FILE *target,
        char *key,
        char *val,
        int *token_offset,
        int *val_offset,
        int n_of_keys
) {
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
                int len_of_buf = strlen(buf);
                bool f = false;
                bool found = false;
                int offset_match = -1;
                for (int i = 0; i < n_of_keys; ++i) {
                    if (len_of_buf == *(token_offset + i*sizeof(int))) {
                        found = true;
                        for (int k = 0; k < *(token_offset + i*sizeof(int)); ++k) {
                            found = found && (*(buf + k*sizeof(char)) == *(key + k*sizeof(char)));
                        }
                    }
                    if (found) {
                        printf("Token match at token_offset: %d\n", i);
                        offset_match = i;
                        break;
                    }
                }
                if (f) {
                    printf("Token '%s' not found in profile file. Replacing buf.\n", buf);
                    for (int k =0; k < len_of_buf; ++k) {
                        fputc(*(buf + k*sizeof(char)), target);
                    }
                }
                else if (found) {
                    printf("Replacing token by value.\n");
                    for (int k = 0; k < *(val_offset + offset_match*sizeof(int)); ++k) {
                        fputc(*(val + k*sizeof(char)), target);
                    }
                }

				// If ch is EOF, it means there was a syntax error in the input file, panic.
				if (ch == EOF) {
					printf("Exiting abnormally, tokeniser failed.\n");
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
    // Profile memory allocation
	char *key = malloc(100 * sizeof(char));
	char *val = malloc(100 * sizeof(char));
    int *token_offset = malloc(sizeof(int));
    *token_offset = 0;
    int *val_offset = malloc(sizeof(int));
    *val_offset = 0;
    int n_of_keys = 0;
	if (argc == 1) {
		printf("Spawning with the default profile, all token will be replaced by nothing.\n");
	}
	if (argc == 2) {
		char *to_profile = argv[1];
		printf("Using profile: %s\n", argv[1]);
		n_of_keys = get_profile(to_profile, &key, &val, &token_offset, &val_offset);
	}
	if (argc > 2) {
		printf("Too many arguments.\n");
		return;
	}

	// Get the template directory
	char template_dir[PATH_MAX];
	strcpy(template_dir, TEMPLATEPATH);
	//strcat(template_dir, TEMPLATEPATH);

	// File pointers for the source and the target
	FILE *source, *target;

	// Loop over all the files in the template directory and spawns them in the current directory.

	// Create a namelist for the directory scan.
	struct dirent **namelist;
	// Get the number of subdir in the template dir. Alphanumerically sorted and inside namelist
	int n = scandir(template_dir, &namelist, NULL, alphasort);
	if (n < 0) {
		printf("Error in scanning the template directory.\n");
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
		copy_file(source, target, key, val, token_offset, val_offset, n_of_keys);

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
		char *to_spawn[argc - 1];
		// Move the argv for the sub-call.
		int i;
		for (i = 0; i < argc; i++) to_spawn[i] = argv[i + 1];
		spawn(argc - 1, to_spawn);
	}
	else if (strcmp(argv[1], "save") == 0) {
		char *to_save[argc - 1];
		// Move the argv for the sub-call.
		int i;
		for (i = 0; i < argc; i++) to_save[i] = argv[i + 1];
		save(argc - 1, to_save);
	}
	else {
		help();
	}

	return 0;
}
