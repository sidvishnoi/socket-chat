#ifndef SRC_SERVER_DB_DB_H_
#define SRC_SERVER_DB_DB_H_

#include <string>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include "./entry/entry.h"

enum statusCode {
  SUCCESS,
  WARNING,
  FAILED
};

class Database {
 public:
  explicit Database(const std::string &dir) {
    dbDir = dir;
    if (dbDir.back() != '/') dbDir.append("/");
    std::string cmd("mkdir -p " + dbDir);
    const int st = system(cmd.c_str());
    if (st > 1) {
      throw std::runtime_error("Failed to create database directory.");
    }
  }

  statusCode add(Entry &);
  Entry get(const std::string &key, const std::string &value) const;
  statusCode update(Entry &oldUser, Entry &newUser);
  statusCode remove(Entry &);

 private:
  std::string dbDir;
  const int hashLength = 2;

  int getEntryPosition(std::fstream &file, Entry &, const std::string key);
};

#endif  //  SRC_SERVER_DB_DB_H_
