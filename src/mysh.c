#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define INPUT_BUFFER_SIZE 1024
#define ARG_MAX 1023

void parse_arguments(/*out*/ char *arguments[], char **argv, int argc)
{
	int last_index = argc - 1;
	for (int i = 0; i < last_index; i++)
		arguments[i] = argv[i + 1];

	arguments[last_index] = NULL;
}

void strip_new_line_at_end(char string[])
{
	// Replaces eding newline with '\0'

	int i;
	for (i = 0; string[i] != '\n' && string[i] != '\0'; i++)
		;

	string[i] = '\0';
}

void tokenize(char string[], /*out*/ char *tokens[])
{
	strip_new_line_at_end(string);

	tokens[0] = strtok(string, " ");

	for (int i = 1; i < INPUT_BUFFER_SIZE; i++)
		tokens[i] = strtok(NULL, " ");
}

void print_prompt()
{
	printf("MyShell $ ");
}

int main(const int argc, char **argv)
{
	if (argc > 1)
	{
		/// Usage: `mysh path-to-command [options]`

		char *executable_path = argv[1];
		char *arguments[argc];
		parse_arguments(/*out*/ arguments, argv, argc);

		execv(executable_path, arguments);
	}
	else
	{
		char input_buffer[INPUT_BUFFER_SIZE];

		while (true)
		{
			/// Usage: `mysh`

			print_prompt();
			fgets(input_buffer, INPUT_BUFFER_SIZE, stdin);

			if (feof(stdin))
				return 0;

			char *tokens[ARG_MAX];
			tokenize(input_buffer, /*out*/ tokens);

			char *arguments[ARG_MAX + 1];
			int i;
			for (i = 0; tokens[i] != NULL; i++)
				arguments[i] = tokens[i];

			arguments[i] = NULL;

			char *command_path;
			command_path = tokens[0];

			execv(command_path, arguments);
		}
	}

	return 0;
}
