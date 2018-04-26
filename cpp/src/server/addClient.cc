#include "./server.h"

bool addClient(
  const int currentClientFd,
  fd_set *master,
  const std::string &credentials,
  const Database<User> &db,
  FdToName &names
  ) {
  vector<string> tokens = split(credentials, DELIM, 2);
  string &username(tokens[0]), &password(tokens[1]);

  auto loginStatus = login(db, username, password);

  auto type = (loginStatus != "SUCCESS") ? "ERROR" : "INFO";
  string loginMsg = type + DELIM + "AUTH" + DELIM + loginStatus;
  send(currentClientFd, loginMsg.c_str(), loginMsg.size(), 0);

  if (loginStatus == "SUCCESS") {
    cout << color::green << "[CLIENT:LOGIN] " << color::reset
      << "<" << username << "> logged in from <"
      << getPeerName(currentClientFd) << ">" << endl;
    names[currentClientFd] = username;
    return true;
  }

  cout << color::magenta << "[CLIENT:AUTH_FAIL] " << color::reset
    << "Invalid login attempt by <" << username << "> from <"
    << getPeerName(currentClientFd) << ">" << endl;
  names[currentClientFd] = "";
  FD_CLR(currentClientFd, master);
  close(currentClientFd);
  return false;
}
