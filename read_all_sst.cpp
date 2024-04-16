
#include <rocksdb/sst_file_reader.h>
#include <rocksdb/table_properties.h>
#include <dirent.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <db_path>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string dir_path = argv[1];
    DIR* dir;
    struct dirent* ent;
    uint64_t total_index_size = 0;
    uint64_t total_data_size = 0;

    if ((dir = opendir(dir_path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string file_name = ent->d_name;
            if (file_name.find(".sst") != std::string::npos) {
                std::string sst_file_path = dir_path + "/" + file_name;
                rocksdb::SstFileReader sst_file_reader {rocksdb::Options()};
                rocksdb::Status s = sst_file_reader.Open(sst_file_path);
                std::cout << "Currently opening " << sst_file_path << std::endl;
                if (!s.ok()) {
                    std::cerr << "Error opening SST file: " << s.ToString() << std::endl;
                    continue;
                }
                total_data_size += sst_file_reader.GetTableProperties()->data_size;
                total_index_size += sst_file_reader.GetTableProperties()->index_size;
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Could not open directory" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Total data size: " << total_data_size << std::endl; // NOLINT(cppcoreguidelines-pro-type-vararg)
    std::cout << "Total index size: " << total_index_size << std::endl;
    return EXIT_SUCCESS;
}
