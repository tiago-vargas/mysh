#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT_BUFFER_SIZE 1000
#define MAX_NUMBER_OF_PARAMETERS 32
#define out
// #define mod

void print_prompt()
{
	printf("MyShell $ ");
}

void tokenize(char string[], out char *tokens[])
{
	tokens[0] = strtok(string, " ");

	for (int i = 1; i < INPUT_BUFFER_SIZE; i++)
		tokens[i] = strtok(NULL, " ");
}

void parse_input(char input[], out char *command, out char **parameters)
{
	fgets(input, INPUT_BUFFER_SIZE, stdin);

	char *tokens[1 + MAX_NUMBER_OF_PARAMETERS];
	tokenize(input, out tokens);

	command = tokens[0];
	// parameters = &(tokens[1]);
}

void main(int argc, char *argv)
{
	while (true)
	{
		char input_buffer[INPUT_BUFFER_SIZE];  // will be modified by strtok
		char *command;
		char **parameters;
		int wstatus;

		print_prompt();
		parse_input(input_buffer, out command, out parameters);

		// read_command(command, parameters);  /* read input from terminal */
		if (fork() > 0)                     /* fork off child process */
		{
			/* Parent code. */
			wait(out &wstatus);       /* wait for (any) child to exit */
		}
		else
		{
			/* Child code. */
			execve(command, parameters, 0); /* execute command */
		}
	}
}
