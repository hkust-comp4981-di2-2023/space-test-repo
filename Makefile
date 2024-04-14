PLR_LIB=./plr_librocksdb.a
DEFAULT_LIB=./default_librocksdb.a
PLR_DB_PATH=./plr_test_db
DEFAULT_DB_PATH=./default_test_db
GENERATED_FILE_PATH=./
GENERATED_FILE_NAME=generated_data
TYPE=exponential
NUM_ENTRIES=1000000

generate_data:
	g++ -std=c++11 ./generate_data.cpp -o ./generate_data -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd
	./generate_data $(GENERATED_FILE_PATH)$(GENERATED_FILE_NAME) $(NUM_ENTRIES)
testing:
	echo "Ingesting $(TYPE) data"
	g++ -std=c++11 ./database_put_plr.cpp -o ./database_put_plr $(PLR_LIB) -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd 
	g++ -std=c++11 ./database_put_default.cpp -o ./database_put_default $(DEFAULT_LIB) -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd
	rm -rf $(PLR_DB_PATH)
	rm -rf $(DEFAULT_DB_PATH)
	./database_put_plr $(PLR_DB_PATH) $(GENERATED_FILE_NAME)_$(TYPE).dat
	./database_put_default $(DEFAULT_DB_PATH) $(GENERATED_FILE_NAME)_$(TYPE).dat
report:
	g++ -std=c++11 ./read_all_sst.cpp -o ./read_all_sst $(PLR_LIB) -I ./ -lz -lpthread -lsnappy -lbz2 -llz4 -lzstd
	echo "Default RocksDB Total Index size"
	./read_all_sst $(DEFAULT_DB_PATH)
	echo "PLR RocksDB Total Index size"
	./read_all_sst $(PLR_DB_PATH)