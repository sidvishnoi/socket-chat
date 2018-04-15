#include "./server.h"

bool addClient(
  const int currentClientFd,
  fd_set *master,
  const std::string &credentials,
  Database<User> &db,
  FdToName &names
  ) {
  vector<string> tokens = split(credentials, DELIM, 2);
  string &username(tokens[0]), &password(tokens[1]);

  auto loginStatus = login(db, username, password);

  auto type = (loginStatus != "SUCCESS") ? "ERROR" : "INFO";
  string loginMsg = type + DELIM + "AUTH" + DELIM + loginStatus;
  send(currentClientFd, loginMsg.c_str(), loginMsg.size(), 0);

  if (loginStatus == "SUCCESS") {
    cout << username << " logged in from <"
      << getPeerName(currentClientFd) << ">" << endl;
    names[currentClientFd] = username;
    return true;
  }

  cout << "Invalid login attempt by: " << username << endl;
  names[currentClientFd] = "";
  FD_CLR(currentClientFd, master);
  close(currentClientFd);
  return false;
}
