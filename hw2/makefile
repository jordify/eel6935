
# This is a template Makefile generated by rpcgen

# Parameters

CLIENT = client
SERVER = server

SOURCES_CLNT.c = 
SOURCES_CLNT.h = 
SOURCES_SVC.c = 
SOURCES_SVC.h = 
SOURCES.x = hw2.x

TARGETS_SVC.c = hw2_svc.c hw2_server.c hw2_xdr.c 
TARGETS_CLNT.c = hw2_clnt.c hw2_client.c hw2_xdr.c 
TARGETS = hw2.h hw2_xdr.c hw2_clnt.c hw2_svc.c hw2_client.c hw2_server.c

OBJECTS_CLNT = $(SOURCES_CLNT.c:%.c=%.o) $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_SVC = $(SOURCES_SVC.c:%.c=%.o) $(TARGETS_SVC.c:%.c=%.o)
# Compiler flags 

CFLAGS += -g -Wall
LDLIBS += -lnsl
RPCGENFLAGS = -N

# Targets 

all : $(CLIENT) $(SERVER)

$(TARGETS) : $(SOURCES.x) 
	rpcgen $(RPCGENFLAGS) $(SOURCES.x)

$(OBJECTS_CLNT) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(TARGETS_CLNT.c) 

$(OBJECTS_SVC) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(TARGETS_SVC.c) 

$(CLIENT) : $(OBJECTS_CLNT) 
	$(LINK.c) -o $(CLIENT) $(OBJECTS_CLNT) $(LDLIBS) 

$(SERVER) : $(OBJECTS_SVC) 
	$(LINK.c) -o $(SERVER) $(OBJECTS_SVC) $(LDLIBS)

clean:
	 $(RM) *.o $(CLIENT) $(SERVER)
send:
	scp -P2222 $(CLIENT) jordi@faraday.acis.ufl.edu:~/testSpace/
	scp -P2222 $(SERVER) jordi@faraday.acis.ufl.edu:~/testSpace/
