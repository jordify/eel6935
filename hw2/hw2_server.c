#include "hw2.h"

char **
open_1_svc(char *arg1,  struct svc_req *rqstp)
{
	static char * result;

        printf(arg1);
        result = "Open\n";

	return &result;
}

char **
find_1_svc(char *arg1,  struct svc_req *rqstp)
{
	static char * result;

        printf(arg1);
        result = "Find\n";

	return &result;
}

char **
remove_1_svc(char *arg1,  struct svc_req *rqstp)
{
	static char * result;

        printf(arg1);
        result = "Remove\n";

	return &result;
}

char **
show_1_svc(char *arg1,  struct svc_req *rqstp)
{
	static char * result;

        printf(arg1);
        result = "Show\n";

	return &result;
}

char **
write_1_svc(char *arg1, int arg2, char *arg3,  struct svc_req *rqstp)
{
	static char * result;

        printf(arg1);
        printf("%d\n",arg2);
        printf(arg3);
        result = "Write\n";

	return &result;
}
