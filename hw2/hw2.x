/* hw2.x Distributed File System RPC protocol */

program HW2_PROGRAM{
  version HW2_VERSION{
    string OPEN(string, int) = 1;
    string FIND(string) = 2;
    string REMOVE(string) = 3;
    string SHOW(string) = 4;
    string WRITE(string, int, string) = 5;
  } = 1;
} = 0x20000001;
