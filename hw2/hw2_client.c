#include "hw2.h"
#include "hw2_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <ctype.h>

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

        char cmd[21];
        char filename[21];
        char strings[21];
	int len;

	clnt = clnt_create (host, HW2_PROGRAM, HW2_VERSION, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

        open_1_arg1 = "open arg 1\n";
        find_1_arg1 = "find arg\n";
        remove_1_arg1 = "remove arg\n";
        show_1_arg1 = "show arg\n";
        write_1_arg1 = "write arg 1\n";
        write_1_arg2 = 42;
        write_1_arg3 = "write arg 3\n";

        while(1){
          printf("$");
          *strings = '\0';
          len = nextin(cmd, filename, strings);
          if (len<0){
            clnt_destroy(clnt);
            printf("Exiting...\n");
            exit(0);
          }

          printf("%d\n", len);

          if (strcmp(cmd,"open") == 0){

            result_1 = open_1(filename, len, clnt);
            if (result_1 == (char **) NULL) {
                    clnt_perror (clnt, "call failed");
            }
            printf(*result_1);
          }
          else if(strcmp(cmd,"exit") == 0){
            clnt_destroy(clnt);
            printf("Exiting...\n");
            exit(0);
          }

        }
        /*
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
        */
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

int nextin (char *cmd, char *name, char *str) {
	int i, ch;

	ch = getc(stdin);
	while (isspace(ch)) {
		ch = getc(stdin);
	}
	if (ch == EOF) {
		return -1;
	}
	i = 0;
	while (!isspace(ch)) {
		if (++i > MAXCOMMANDLEN) {
			printf("Error: Command is too long.\n");
			exit (1);
		}
		*cmd++ = ch;
		ch = getc(stdin);
	}
	*cmd = '\0';

	while (isspace(ch)) {
		if (ch == '\n') {
			*name = '\0';
			return 0;
		}
		ch = getc(stdin);
	}
	i = 0;
	while (!isspace(ch)) {
		if (++i > MAXFILENAME) {
			printf("Error: File name is too long.\n");
			exit (1);
		}
		*name++ = ch;
		ch = getc(stdin);
	}
	*name = '\0';

        while (isspace(ch)) {
                if (ch == '\n') {
                        *str = '\0';
                        return 0;
                }
                ch = getc(stdin);
        }
        i = 0;
        while (ch != '\n') {
                if (++i > MAXSTRLEN) {
                        printf("Error: String is too long.\n");
                        exit (1);
                }
                *str++ = ch;
                ch = getc(stdin);
        }
        *str = '\0';
	fflush(stdin);
	return i;
}
