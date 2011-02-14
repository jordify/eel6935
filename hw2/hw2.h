/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _HW2_H_RPCGEN
#define _HW2_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct open_1_argument {
	char *arg1;
	int arg2;
};
typedef struct open_1_argument open_1_argument;

struct write_1_argument {
	char *arg1;
	int arg2;
	char *arg3;
};
typedef struct write_1_argument write_1_argument;

#define HW2_PROGRAM 0x20000001
#define HW2_VERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define OPEN 1
extern  char ** open_1(char *, int , CLIENT *);
extern  char ** open_1_svc(char *, int , struct svc_req *);
#define FIND 2
extern  char ** find_1(char *, CLIENT *);
extern  char ** find_1_svc(char *, struct svc_req *);
#define REMOVE 3
extern  char ** remove_1(char *, CLIENT *);
extern  char ** remove_1_svc(char *, struct svc_req *);
#define SHOW 4
extern  char ** show_1(char *, CLIENT *);
extern  char ** show_1_svc(char *, struct svc_req *);
#define WRITE 5
extern  char ** write_1(char *, int , char *, CLIENT *);
extern  char ** write_1_svc(char *, int , char *, struct svc_req *);
extern int hw2_program_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define OPEN 1
extern  char ** open_1();
extern  char ** open_1_svc();
#define FIND 2
extern  char ** find_1();
extern  char ** find_1_svc();
#define REMOVE 3
extern  char ** remove_1();
extern  char ** remove_1_svc();
#define SHOW 4
extern  char ** show_1();
extern  char ** show_1_svc();
#define WRITE 5
extern  char ** write_1();
extern  char ** write_1_svc();
extern int hw2_program_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_open_1_argument (XDR *, open_1_argument*);
extern  bool_t xdr_write_1_argument (XDR *, write_1_argument*);

#else /* K&R C */
extern bool_t xdr_open_1_argument ();
extern bool_t xdr_write_1_argument ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_HW2_H_RPCGEN */
