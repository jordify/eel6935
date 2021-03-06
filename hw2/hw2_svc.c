/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "hw2.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static char **
_open_1 (open_1_argument *argp, struct svc_req *rqstp)
{
	return (open_1_svc(argp->arg1, argp->arg2, rqstp));
}

static char **
_find_1 (char * *argp, struct svc_req *rqstp)
{
	return (find_1_svc(*argp, rqstp));
}

static char **
_remove_1 (char * *argp, struct svc_req *rqstp)
{
	return (remove_1_svc(*argp, rqstp));
}

static char **
_show_1 (char * *argp, struct svc_req *rqstp)
{
	return (show_1_svc(*argp, rqstp));
}

static char **
_write_1 (write_1_argument *argp, struct svc_req *rqstp)
{
	return (write_1_svc(argp->arg1, argp->arg2, argp->arg3, rqstp));
}

static void
hw2_program_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		open_1_argument open_1_arg;
		char *find_1_arg;
		char *remove_1_arg;
		char *show_1_arg;
		write_1_argument write_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case OPEN:
		_xdr_argument = (xdrproc_t) xdr_open_1_argument;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) _open_1;
		break;

	case FIND:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) _find_1;
		break;

	case REMOVE:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) _remove_1;
		break;

	case SHOW:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) _show_1;
		break;

	case WRITE:
		_xdr_argument = (xdrproc_t) xdr_write_1_argument;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) _write_1;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (HW2_PROGRAM, HW2_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, HW2_PROGRAM, HW2_VERSION, hw2_program_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (HW2_PROGRAM, HW2_VERSION, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, HW2_PROGRAM, HW2_VERSION, hw2_program_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (HW2_PROGRAM, HW2_VERSION, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
