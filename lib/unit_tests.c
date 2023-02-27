#include <stdio.h>
#include <stdbool.h>

#define RED         "\e[31m"
#define GREEN       "\e[32m"
#define RESET_COLOR "\e[0m"

void run_test(bool (*test_case)(), char *description)
{
	if (test_case())
		printf(GREEN "PASSED" RESET_COLOR ": %s" "\n", description);
	else
		printf(RED   "FAILED" RESET_COLOR ": %s" "\n", description);
}
