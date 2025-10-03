#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

const std::string INPUT_FILE = "input_task2.txt";
const std::string OUTPUT_ODD_FILE = "output_odd.txt";
const std::string OUTPUT_EVEN_FILE = "output_even.txt";

void write_line_func(const std::string& line_content, std::ofstream& output_stream) {
    std::cout << "Thread (ID: " << std::this_thread::get_id << ") is writing line to file";
    output_stream << line_content;
}

void create_test_file() {
    std::ofstream test_file(INPUT_FILE);
    if (test_file.is_open()) {
        test_file << "Line 1: odd thread";
        test_file << "Line 2: even thread";
        test_file << "Line 3: odd thread";
        test_file << "Line 4: even thread";
        test_file << "Line 5: odd thread";
        test_file << "Line 6: even thread";
        test_file << "Line 7: odd thread";
        test_file << "Line 8: even thread";
        std::cout << "Test file '" << INPUT_FILE << "' created" << std::endl;
        test_file.close();
    }
    else {
        std::cerr << "Error: could not open or create '" << INPUT_FILE << "' file" << std::endl;
    }
}

int main() {
    create_test_file();
    std::ifstream input_fp(INPUT_FILE);
    std::ofstream output_odd_fp(OUTPUT_ODD_FILE);
    std::ofstream output_even_fp(OUTPUT_EVEN_FILE);

    if (!input_fp.is_open() || !output_odd_fp.is_open() || !output_even_fp.is_open()) {
        std::cerr << "Error: could not open one or more files" << std::endl;
        return 1;
    }

    std::cout << "Files are successfully opened" << std::endl;

    std::string line;
    int line_count=0;
    while (std::getline(input_fp, line)) {
        line_count++;
        line += "\n";

        std::cout << "Line " << line_count << "processing... ";
        std::thread worker_thread;

        if (line_count % 2 != 0) {
            std::cout << "Odd" << std::endl;
            worker_thread = std::thread(write_line_func, std::ref(line), std::ref(output_odd_fp));
        }
        else {
            std::cout << "Even" << std::endl;
            worker_thread = std::thread(write_line_func, std::ref(line), std::ref(output_even_fp));
        }

        if (worker_thread.joinable()) {
            worker_thread.join();
            std::cout << "Thread is done and joined" << std::endl;
        }
    }

    input_fp.close();
    output_odd_fp.close();
    output_even_fp.close();

    std::cout << "Process closed. Look for 'output_odd.txt' and 'output_even.txt' for results" << std::endl;
    return 0;
}