# REQUIRED: The following libraries are compiled via
# make static_lib command, and rename the libaray to the current directory
PLR_LIB=./plr_librocksdb.a
DEFAULT_LIB=./default_librocksdb.a

# REQUIRED: The database path for the PLR and Default RocksDB to test the space
# IT SHOULD BE THE SAME THROUGHOUT THE PROCESS
PLR_DB_PATH=./plr_test_db
DEFAULT_DB_PATH=./default_test_db

# REQUIRED: Generated data path and name
# The file name will append _uniform, _linear, _exponential
# IT SHOULD BE THE SAME THROUGHOUT THE PROCESS
GENERATED_FILE_PATH=./
GENERATED_FILE_NAME=generated_data

# Testing type, could be {uniform, linear, exponential}
TYPE=exponential
# Number of entries to generate
NUM_ENTRIES=10000000

# Usage:
# 1. Config the above variables
# 2. move the required rocksdb libraries to the current directory
# 3. make generate_data to generate the data
# 4. make testing to ingest the data and create DB
# 5. make report to get the total index size of the DB

all:

generate_data:
	echo "Generating data"
	rm -rf *.dat
	rm -f generate_data
	g++ -std=c++11 ./generate_data.cpp -o ./generate_data -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd
	./generate_data $(GENERATED_FILE_PATH)$(GENERATED_FILE_NAME) $(NUM_ENTRIES)
testing:
	echo "Ingesting $(TYPE) data"
	g++ -std=c++11 ./database_put_plr.cpp -o ./database_put_plr $(PLR_LIB) -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd 
	g++ -std=c++11 ./database_put_default.cpp -o ./database_put_default $(DEFAULT_LIB) -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd
	rm -rf $(PLR_DB_PATH)
	rm -rf $(DEFAULT_DB_PATH)
	echo "Running PLR RocksDB"
	./database_put_plr $(PLR_DB_PATH) $(GENERATED_FILE_NAME)_$(TYPE).dat
	echo "Running Default RocksDB"
	./database_put_default $(DEFAULT_DB_PATH) $(GENERATED_FILE_NAME)_$(TYPE).dat

testing_var:
	echo "Ingesting $(TYPE) data"
	g++ -std=c++11 ./database_put_plr_var.cpp -o ./database_put_plr_var $(PLR_LIB) -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd 
	g++ -std=c++11 ./database_put_default_var.cpp -o ./database_put_default_var $(DEFAULT_LIB) -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd
	rm -rf $(PLR_DB_PATH)
	rm -rf $(DEFAULT_DB_PATH)
	echo "Running PLR RocksDB"
	./database_put_plr_var $(PLR_DB_PATH) $(GENERATED_FILE_NAME)_$(TYPE).dat
	echo "Running Default RocksDB"
	./database_put_default_var $(DEFAULT_DB_PATH) $(GENERATED_FILE_NAME)_$(TYPE).dat
report:
	g++ -std=c++11 ./read_all_sst.cpp -o ./read_all_sst $(PLR_LIB) -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd
	echo "Default RocksDB Total Index size"
	./read_all_sst $(DEFAULT_DB_PATH)
	echo "PLR RocksDB Total Index size"
	./read_all_sst $(PLR_DB_PATH)