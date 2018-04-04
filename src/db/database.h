#ifndef SRC_DB_DATABASE_H_
#define SRC_DB_DATABASE_H_

#include <string>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

template <class T>
class Database {
 public:
  explicit Database(
    const std::string &dir,
    const std::string &_pk,
    std::pair<int, int> _bucketHash = {0, 2}
  ) : pk(_pk), dbDir(dir), bucketHash(_bucketHash) {
    if (dbDir.back() != '/') dbDir.append("/");
    createDbDir();
  }

  // append an entry of class T (specializes Entry) to database
  int add(const T &);

  // get an entry where primary key value is value
  // uses linear search
  std::pair<T, int> get(const std::string &value);

  // update an entry in database
  bool update(T &oldEntry, T &newEntry);

  // mark an entry as "deleted"
  bool remove(T & entry);

 private:
  // primary key
  std::string pk;

  // bucket name is substring of pk at these indices
  std::pair<int, int> bucketHash;

  // where to save database files
  std::string dbDir;

  // get name of bucket for a pk value using bucketHash
  std::string getBucketName(const std::string &pkValue) {
    return dbDir + pkValue.substr(bucketHash.first, bucketHash.second) + ".txt";
  }

  // create a bucket if not exists
  bool createBucket(const std::string &bucketName);

  // get number of records in a bucket
  int getNumberOfRecords(std::fstream &bucket, const T &entry);

  // create the database directory
  void createDbDir();

  // helper function for add(const T&). uses binary search
  int getInsertPosition(std::fstream &bucket, const T &entry);
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
bool Database<T>::createBucket(const std::string &bucketName) {
  std::ifstream fl(bucketName);
  if (fl) return false;   // not created
  const std::string cmd = "touch " + bucketName;
  return !system(cmd.c_str());
}

template <class T>
int Database<T>::getNumberOfRecords(std::fstream &bucket, const T& entry) {
  bucket.seekg(0, std::ios::end);
  const int num = bucket.tellg() / entry.size();
  bucket.seekg(0);
  return num;
}

template <class T>
int Database<T>::add(const T &entry) {
  std::pair<T, int> old = get(entry.get(pk));
  if (old.second != -1) return 1;
  const std::string bucketName = getBucketName(entry.get(pk));
  std::ofstream bucket(bucketName, std::ios::app | std::ios::binary);
  bucket << entry.serialize() + "\n";
  return 0;
}

template <class T>
std::pair<T, int> Database<T>::get(const std::string &value) {
  std::string record;
  std::ifstream bucket(getBucketName(value), std::ios::binary);
  while (std::getline(bucket, record) && !record.empty()) {
    T temp(record);
    if (temp.get(pk) == value) {
      return {temp, bucket.tellg()};
    }
  }
  return {T(), -1};
}

template <class T>
bool Database<T>::update(T &oldEntry, T &newEntry) {
  using namespace std;

  pair<T, int> old = get(oldEntry.get(pk));

  // do not update if some entry already exists with newEntry.pk
  pair<T, int> same = get(newEntry.get(pk));
  if (same.second != -1) return false;

  ofstream bucket(
    getBucketName(oldEntry.get(pk)),
    ios::in | ios::out | ios::binary);
  int offset = old.second - oldEntry.size() - 1;
  bucket.seekp(offset);
  bucket << newEntry.serialize() + "\n";
  return true;
}

#endif  //  SRC_DB_DATABASE_H_
