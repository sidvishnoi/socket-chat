#ifndef SRC_DB_DATABASE_H_
#define SRC_DB_DATABASE_H_

#include <string>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

int getNumberOfLines(std::fstream &file, size_t lineLength);

template <class T>
class Database {
 public:
  explicit Database(
    const std::string &dir,
    const std::string &_pk,
    std::string (*bkfn)(const T &) = nullptr)
  : pk(_pk), dbDir(dir), bucketFn(bkfn) {
    if (dbDir.back() != '/') dbDir.append("/");
    createDbDir();
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
  bool remove(T & entry);

 private:
  // primary key
  std::string pk;

  // create a new bucket based on name returned by bucketFn
  std::string (*bucketFn)(const T &entry);

  // where to save database files
  std::string dbDir;

  // get name of bucket for an entry using bucketFn
  std::string getBucketName(const T &entry);

  // create a bucket if not exists
  bool createBucket(const std::string &bucketName);

  // get number of records in a bucket
  int getNumberOfRecords(const std::string &bucketName, const T &entry);

  // create the database directory
  void createDbDir();

  // helper function for add(const T&). uses binary search
  int getInsertPosition(std::fstream &file, const T &entry);
};

template <class T>
void Database<T>::createDbDir() {
  // TODO: Add support for other OS
  std::string cmd("mkdir -p " + dbDir);
  const int st = system(cmd.c_str());
  if (st > 1) {
    throw std::runtime_error("Failed to create database directory.");
  }
}

template <class T>
std::string Database<T>::getBucketName(const T &entry) {
  if (bucketFn != nullptr) return bucketFn(entry);
  return entry.get(pk).substr(0, 2);
}

template <class T>
bool Database<T>::createBucket(const std::string &bucketName) {
  std::ifstream fl(bucketName);
  if (fl) return false;    // not created
  const std::string cmd = "touch " + bucketName;
  return !system(cmd.c_str());
}

template <class T>
int Database<T>::getNumberOfRecords(const std::string &bucketName, const T& entry) {
  std::ifstream file(bucketName);
  file.seekg(0, std::ios::end);
  return file.tellg() / entry.size();
}

#endif  //  SRC_DB_DATABASE_H_
