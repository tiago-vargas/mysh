#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

void parse_arguments(/*out*/ char *arguments[], char **argv, int argc)
{
	int last_index = argc - 1;
	for (int i = 0; i < last_index; i++)
		arguments[i] = argv[i + 1];

	arguments[last_index] = NULL;
}

int main(const int argc, char **argv)
{
	if (argc > 1)
	{
		/// Usage: `mysh command [options]`

		char *executable_path = argv[1];
		char *arguments[argc];
		parse_arguments(/*out*/ arguments, argv, argc);

		execv(executable_path, arguments);
	}

	return 0;
}
