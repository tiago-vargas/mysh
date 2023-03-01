#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#include "colors.h"

#define INPUT_BUFFER_SIZE 1024
#define ARG_MAX 1023

#define out /* simply indicates an out parameter */

void parse_arguments(out char *arguments[], char **argv, int argc)
{
	int last_index = argc - 1;
	for (int i = 0; i < last_index; i++)
		arguments[i] = argv[i + 1];

	arguments[last_index] = NULL;
}

void strip_new_line_at_the_end(char string[])
{
	/// Replaces ending newline with '\0', if any

	int i;
	for (i = 0; string[i] != '\n' && string[i] != '\0'; i++)
		;

	string[i] = '\0';
}

void tokenize(char string[], out char *tokens[])
{
	tokens[0] = strtok(string, " ");

	for (int i = 1; i < INPUT_BUFFER_SIZE; i++)
		tokens[i] = strtok(NULL, " ");
}

void print_prompt()
{
	printf(BLUE "MyShell $ " RESET);
}

void get_raw_input(out char input_buffer[], int buffer_size)
{
	fgets(input_buffer, buffer_size, stdin);
}

void copy_vector_ending_with_null(out char *destination[], char **source)
{
	int i;
	for (i = 0; source[i] != NULL; i++)
		destination[i] = source[i];

	destination[i] = NULL;
}

int main(const int argc, char **argv)
{
	bool command_has_arguments = (argc > 1);
	bool should_run_interactively = (!command_has_arguments);

	if (!should_run_interactively)
	{
		/// Usage: `mysh command [options]`

		char *executable_path = argv[1];
		char *arguments[argc];
		parse_arguments(out arguments, argv, argc);

		execv(executable_path, arguments);
	}
	else
	{
		/// Usage: `mysh`

		char input_buffer[INPUT_BUFFER_SIZE];

		while (true)
		{
			print_prompt();
			get_raw_input(input_buffer, INPUT_BUFFER_SIZE);

			if (feof(stdin))
				return 0;

			// Begin parsing input //////////////////////
			strip_new_line_at_the_end(input_buffer);

			char *tokens[ARG_MAX];
			tokenize(input_buffer, out tokens);

			char *arguments[ARG_MAX + 1];  // +1 to allocate NULL at the end
			copy_vector_ending_with_null(out arguments, tokens);

			char *command = tokens[0];
			// End parsing input ////////////////////////

			// For when user presses <Enter> with no commands
			if (command == NULL)
				continue;

			// Manages shell built-in commands
			if (strcmp(command, "cd") == 0)
			{
				char *directory = arguments[1];
				chdir(directory);
				continue;
			}

			// Manages other commands
			int pid = fork();

			bool is_child_process = (pid == 0);
			bool is_parent_process = (pid > 0);

			if (is_child_process)
			{
				execvp(command, arguments);
				// If program control got here, then `execv` failed
				printf(RED "Command not executed: errno = %d" "\n" RESET, errno);
				exit(errno);
			}
			else if (is_parent_process)
			{
				waitpid(pid, NULL, 0);
			}
			else
			{
				printf(RED "Error while forking: errno = %d" "\n" RESET, errno);
			}
		}
	}

	return 0;
}
