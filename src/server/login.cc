#include "./server.h"

std::string login(Database<User> &db, const string &uname, const string &pass) {
  auto record = db.get(uname).first;
  auto user = record.get();

  // register user if not exists and log in
  if (user["status"] == "undefined") {
    db.add(User({uname, pass, "1"}));
    cout << "Registered new user: " << uname << endl;
    return "SUCCESS";
  }

  // check credentials and state
  if (user["password"] == pass) {
    if (user["status"] == "1") return "ALREADY LOGGED IN";
    db.update(record, User({uname, pass, "1"}));
    return "SUCCESS";
  }

  return "FAILED";
}
