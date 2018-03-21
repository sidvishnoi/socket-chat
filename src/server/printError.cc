#include "server.h"

void printError() {
  cout << "[ERROR]: ";
  switch (errno) {
    case EADDRINUSE:
      cout << "EADDRINUSE" << endl;
      break;
    case EBADF:
      cout << "EBADF" << endl;
      break;
    case EACCES:
      cout << "EACCES" << endl;
      break;
    default:
      cout << "Some unknown error";
  }
}
