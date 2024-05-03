#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"
#include "rocksdb/status.h"
#include "rocksdb/table.h"
using namespace std;
using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::PinnableSlice;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteBatch;
using ROCKSDB_NAMESPACE::WriteOptions;
using rocksdb::BlockBasedTableOptions;
void write_to_db(rocksdb::DB* db, string key, string value);


const int KEY_SIZE = 8;
const int VALUE_SIZE = 400;
const int BLOCK_SIZE = 4;
template<typename T>
std::string to_string(T value) {
    const size_t value_size = sizeof(value) / sizeof(char);
    union {
        char buffer[value_size];
        T value;
    } obj;
    obj.value = value;
    return std::string{std::begin(obj.buffer), std::end(obj.buffer)};
}

// A utility function for decoding any value from string
// If we have a string "a", it will convert to 97 (reversed order)
// Encoding scheme will be memcpy()
template<typename T>
T to_type(std::string str) {
    const size_t value_size = sizeof(T) / sizeof(char);
    union {
        char buffer[value_size];
        T value{};
    } obj;
    size_t count = 0;
    for (auto i = std::begin(str); i != std::end(str); i++) {
        obj.buffer[count++] = *(i);
    }
//    std::cout << obj.buffer[7] << std::endl;
    return obj.value;
}

// Read the file and print the key-value pairs
void read_key_value_pairs(std::string filename, rocksdb::DB* db) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    int a = 0;
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    file.seekg(0, ios::beg);
    uint64_t count = 0;
    while (count < fileSize / (KEY_SIZE+VALUE_SIZE)) {
        std::string key_str(KEY_SIZE, 0);
        file.read(&key_str[0], KEY_SIZE);
        // keys.push_back(key_str);
        // uint64_t key = to_type<uint64_t>(key_str);

        std::string value_str(VALUE_SIZE, 0);
        file.read(&value_str[0], VALUE_SIZE);
        // values.push_back(value_str);
        write_to_db(db, key_str, value_str);
        count ++;
        // std::cout << "Key: " << key_str << " Value: " << value_str << std::endl;
    }
    std::cout << "Read " << count << " key-value pairs" << std::endl;
    file.close();
    // cout << a << endl;
}

// teat by using main function
// 1. Read the file and store the key-value pairs
// 2. Open a DB and put all the key-value pairs
// 3. Flush at last
// 4. Close the DB
// using rocksdb



void write_to_db(rocksdb::DB* db, string key, string value) {
    rocksdb::Status status = db->Put(rocksdb::WriteOptions(), key, value);
    if (!status.ok()) {
        std::cerr << "Unable to put key-value pair" << std::endl;
        return;
    }
}
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <db_name> <ingest_file>" << std::endl;
        return -1;
    }
    // TODO: CHANGE HERE
    std::string dbname = argv[1];
    std::string filename = argv[2];
    // generate_random_key_value_pairs_uniform(filename, 10000000);
    // read the file and store the key-value pairs
    std::vector<std::string> keys;
    std::vector<std::string> values;
    

    // open a DB and put all the key-value pairs
    rocksdb::DB* db;
    rocksdb::Options options;
    options.compression = rocksdb::kNoCompression;
    // set sst file size 
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    options.create_if_missing = true;
    BlockBasedTableOptions table_options;
    table_options.enable_index_compression = false;
    table_options.block_size = BLOCK_SIZE * 1024;

    // TODO: Change the index type IF Default
    // table_options.index_type = BlockBasedTableOptions::kLearnedIndexWithPLR;
    options.table_factory.reset(NewBlockBasedTableFactory(table_options));
    // Change the index type in rocksdb
    // std::cout << "Ingesting " << keys.size() << " key-value pairs" << std::endl;
    rocksdb::Status status = rocksdb::DB::Open(options, dbname, &db);
    if (!status.ok()) {
        std::cerr << "Unable to open/create testdb" << std::endl;
        return -1;
    }
    read_key_value_pairs(filename, db);
    // for (int i = 0; i < keys.size(); i++) {
    //     // std::cout << "Key: " << keys[i] << " Value: " << values[i] << std::endl;
    //     status = db->Put(rocksdb::WriteOptions(), keys[i], values[i]);
    //     if (!status.ok()) {
    //         std::cerr << "Unable to put key-value pair" << std::endl;
    //         return -1;
    //     }
    // }

    // flush at last
    db->Flush(ROCKSDB_NAMESPACE::FlushOptions());
    // close the DB
    // db->Close();

    delete db;

    return 0;
}