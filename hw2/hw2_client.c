#include "hw2.h"

void
hw2_program_1(char *host)
{
	CLIENT *clnt;
	char * *result_1;
	char *open_1_arg1;
	char * *result_2;
	char *find_1_arg1;
	char * *result_3;
	char *remove_1_arg1;
	char * *result_4;
	char *show_1_arg1;
	char * *result_5;
	char *write_1_arg1;
	int write_1_arg2;
	char *write_1_arg3;

	clnt = clnt_create (host, HW2_PROGRAM, HW2_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

        open_1_arg1 = "open arg\n";
        find_1_arg1 = "find arg\n";
        remove_1_arg1 = "remove arg\n";
        show_1_arg1 = "show arg\n";
        write_1_arg1 = "write arg 1\n";
        write_1_arg2 = 42;
        write_1_arg3 = "write arg 3\n";

	result_1 = open_1(open_1_arg1, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
        printf(*result_1);
	result_2 = find_1(find_1_arg1, clnt);
	if (result_2 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
        printf(*result_2);
	result_3 = remove_1(remove_1_arg1, clnt);
	if (result_3 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
        printf(*result_3);
	result_4 = show_1(show_1_arg1, clnt);
	if (result_4 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
        printf(*result_4);
	result_5 = write_1(write_1_arg1, write_1_arg2, write_1_arg3, clnt);
	if (result_5 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}
        printf(*result_5);
	clnt_destroy (clnt);
}

int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	hw2_program_1 (host);
exit (0);
}
