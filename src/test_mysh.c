#include <stdlib.h>
#include <string.h>

#include "../lib/unit_tests.c"

#define BUFFER_SIZE 256

bool execute_ls_on_current_directory()
{
	char actual_output[BUFFER_SIZE];
	FILE *mysh = popen("out/mysh -c ls", "r");

	char expected_output[BUFFER_SIZE];
	FILE *sh = popen("sh -c 'ls -C'", "r");

	fgets(actual_output, BUFFER_SIZE, mysh);
	fgets(expected_output, BUFFER_SIZE, sh);

	pclose(mysh);
	pclose(sh);

	return strcmp(actual_output, expected_output) == 0;
}

void run_all_tests()
{
	run_test(execute_ls_on_current_directory,
	         "`mysh -c ls` should return `lib  out  src\\n`");
}

int main()
{
	run_all_tests();

	return 0;
}
