#ifndef SRC_DB_DATABASE_H_
#define SRC_DB_DATABASE_H_

#include <string>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

int getNumberOfLines(std::fstream &file, size_t lineLength);
bool createFileIfNotExists(const std::string &fileName);

template <class T>
class Database {
 public:
  explicit Database(const std::string &dir, const std::string &_pk)
    : pk(_pk), dbDir(dir) {
    if (dbDir.back() != '/') dbDir.append("/");
    std::string cmd("mkdir -p " + dbDir);
    const int st = system(cmd.c_str());
    if (st > 1) {
      throw std::runtime_error("Failed to create database directory.");
    }
  }

  // add an entry of class T (specializes Entry) to database
  // entry is added in way such that db is sorted by primary key
  int add(const T &);

  // append an entry of class T
  // !!! do not use with add() !!!
  int append(const T &);

  // get an entry where primary key value is value
  // uses binary search
  T get(const std::string &value) const;

  // get an entry from database where key = value
  // uses Linear search
  T get(const std::string &key, const std::string &value) const;

  // TODO:
  // get an entry based on a lambda comparison function
  // uses Linear search


  // update an entry in database
  bool update(T &oldUser, T &newUser);

  // mark an entry as "deleted"
  bool remove(T &);

 private:
  std::string pk;             // primary key
  std::string dbDir;          // where to save database files
  const int hashLength = 2;   // split the db by first `hashLength` chars of pk

  // helper function for add(const T&)
  // uses binary search
  int getInsertPosition(std::fstream &file, const T &);
};

#endif  //  SRC_DB_DATABASE_H_
