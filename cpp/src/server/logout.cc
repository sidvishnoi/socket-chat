#include "./server.h"

bool logout(const Database<User> &db, const string &uname) {
  auto user = db.get(uname).first;
  return db.update(user, User({uname, user.get("password"), "0"}));
}
