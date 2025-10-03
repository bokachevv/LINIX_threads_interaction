#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

const std::string INPUT_FILE = "input_task2.txt";
const std::string OUTPUT_ODD_FILE = "output_odd.txt";
const std::string OUTPUT_EVEN_FILE = "output_even.txt";

void write_line_func(const std::string& line_content, std::ofstream& output_stream) {
    std::cout << std::endl << "Thread (ID: " << std::this_thread::get_id() << ") is writing line to file";
    output_stream << line_content;
    output_stream.flush();
}

void create_test_file() {
    std::ofstream test_file(INPUT_FILE);
    if (test_file.is_open()) {
        test_file << "Line 1: odd thread\n";
        test_file << "Line 2: even thread\n";
        test_file << "Line 3: odd thread\n";
        test_file << "Line 4: even thread\n";
        test_file << "Line 5: odd thread\n";
        test_file << "Line 6: even thread\n";
        test_file << "Line 7: odd thread\n";
        test_file << "Line 8: even thread\n";
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
    std::ofstream output_odd_fp(OUTPUT_ODD_FILE, std::ios::trunc);
    std::ofstream output_even_fp(OUTPUT_EVEN_FILE, std::ios::trunc);

    if (!input_fp.is_open() || !output_odd_fp.is_open() || !output_even_fp.is_open()) {
        std::cerr << "Error: could not open one or more files" << std::endl;
        return 1;
    }

    std::cout << "Files are successfully opened" << std::endl;

    std::string odd_line, even_line;
    int line_count=0;
    while (true) {

        if (!std::getline(input_fp, odd_line)) {
            break;
        }
        line_count++;
        odd_line += "\n";
        std::cout << "\nLine " << line_count << " processing... Odd" << std::endl;

        if (!std::getline(input_fp, even_line)) {
            std::thread odd_thread(write_line_func, std::ref(odd_line), std::ref(output_odd_fp));
            if (odd_thread.joinable()) {
                odd_thread.join();
                std::cout << "Thread for line " << line_count << " is done and joined." << std::endl;
            }
            break;
        }

        line_count++;
        even_line += "\n";
        std::cout << "Line " << line_count << " processing... Even" << std::endl;

        std::thread odd_thread(write_line_func, std::ref(odd_line), std::ref(output_odd_fp));
        std::thread even_thread(write_line_func, std::ref(even_line), std::ref(output_even_fp));

        if (odd_thread.joinable()) {
            odd_thread.join();
            std::cout << std::endl << "Thread for odd line is done and joined." << std::endl;
        }

        if (even_thread.joinable()) {
            even_thread.join();
            std::cout << "Thread for even line is done and joined." << std::endl;
        }

    }

    input_fp.close();
    output_odd_fp.close();
    output_even_fp.close();

    std::cout << "Process closed. Look for 'output_odd.txt' and 'output_even.txt' for results" << std::endl;
    return 0;
}