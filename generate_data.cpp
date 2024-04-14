#include <fstream>
#include <random>
#include <iostream>
// #include <climit>
#include <string>

using namespace std;

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

// Generate random key-value pairs

// First generate random keys-value pairs in uniform distribution
// KEY: 8 bytes
// VALUE: 20 bytes
// Use above template functions
// Write the result an array of bytes to a file
void generate_random_key_value_pairs_uniform(std::string filename, int num_pairs) {
    std::ofstream file(filename);
    // Generate random key-value pairs byte by byte using uint8_t
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0, 255);

    for (int i = 0; i < num_pairs; i++) {
        std::string key {};
        for (int j = 0; j < 8; j++) {
            key.push_back(static_cast<char>(dis(gen)));
            file.write(to_string(dis(gen)).c_str(), 1);
        }
        // std::string key_str = to_string(key);

        std::string value_str {};
        for (int j = 0; j < 400; j++) {
            value_str.push_back(static_cast<char>(dis(gen)));
            file.write(to_string(dis(gen)).c_str(), 1);
        }
        // cout original key and value
        // std::cout << "Key: " << key << " Value: " << value_str << std::endl;
        
    }
    file.close();
}

void generate_random_key_value_pairs_exponential(std::string filename, int num_pairs) {
    std::ofstream file(filename);
    // Generate random key-value pairs byte by byte using uint8_t
    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<double> dis(0.5);

    for (int i = 0; i < num_pairs; i++) {
        uint64_t key = dis(gen) * __UINT64_MAX__;
        std::string key_str = to_string<uint64_t>(key);
        file.write(key_str.c_str(), 8);
        std::string value_str {};
        for (int j = 0; j < 400; j++) {
            value_str.push_back(static_cast<char>(dis(gen)));
            file.write(to_string(dis(gen)).c_str(), 1);
        }
        // cout original key and value
        // std::cout << "Key: " << key << " Value: " << value_str << std::endl;
    }
    file.close();
}

void generate_random_key_value_pairs_linear(std::string filename, int num_pairs) {
    std::ofstream file(filename);
    // Generate random key-value pairs byte by byte using uint8_t
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> x {0,1};
    std::vector<double> y {0,1};
    std::piecewise_linear_distribution<double> dis(x.begin(), x.end(), y.begin());

    for (int i = 0; i < num_pairs; i++) {
        uint64_t key = dis(gen) * __UINT64_MAX__;
        std::string key_str = to_string<uint64_t>(key);
        file.write(key_str.c_str(), 8);
        std::string value_str {};
        for (int j = 0; j < 400; j++) {
            value_str.push_back(static_cast<char>(dis(gen)));
            file.write(to_string(dis(gen)).c_str(), 1);
        }
        // cout original key and value
        // std::cout << "Key: " << key << " Value: " << value_str << std::endl;
    }
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <num_pairs>" << std::endl;
        return -1;
    }
    std::string filename = argv[1];
    int num_pairs = std::stoi(argv[2]);
    generate_random_key_value_pairs_uniform(filename+"_uniform.dat", num_pairs);
    generate_random_key_value_pairs_exponential(filename+"_exponential.dat", num_pairs);
    generate_random_key_value_pairs_linear(filename+"_linear.dat", num_pairs);
    return 0;
}