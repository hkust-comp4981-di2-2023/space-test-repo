## Space Consumption Benchmark for RocksDB and Sherry

Reproduce our benchmarking results by the following steps:

1. Produce two static RocksDB Library: `plr_rocksdb` for Sherry and `default_rocksdb` for the original index. To produce these two binaries, we need to first clone the following branches:
   - [Original Index Block](https://github.com/hkust-comp4981-di2-2023/rocksdb/tree/6.8.fb): Branch `6.8.fb`.
   - [Sherry](https://github.com/hkust-comp4981-di2-2023/rocksdb/tree/fyp-6.8.fb-dev-debug-opt): Branch `fyp-6.8.fb-dev-debug-opt`.

2. In the two branches, use the command `make release -j4` to produce `librocksdb.a` for each version. Save to `space-test-repo` directory and rename it to distinguish between the original index and Sherry.

3. In `Makefile` of this repository, there are several settings parameter. Configure the binary path before running any tasks.

4. Use the command `make generate_data`, it will generate `NUM_ENTRIES` specified in `Makefile`, each entries have `KEY_SIZE` and `VALUE_SIZE` specified in `generate_data.cpp`.

5. Before running any test, open `database_put_plr.cpp` and `database_put_default.cpp` and change `KEY_SIZE` and `VALUE_SIZE` to match the corresponding generated size. Then specify `TYPE` in `Makefile`. Run `make testing` to put all data into the database.
   - If you would like to run variable value size, modify `KEY_SIZE` and `VALUE_SIZE` in `database_put_plr_var.cpp` and `database_put_default_var.cpp` and run `make testing_var`.

6. If all settings are correct, you will see output like `creating XXX entries`. The number of entries output here should be the same as `NUM_ENTRIES`.

7. Run `make report` to get the index size of the database.
