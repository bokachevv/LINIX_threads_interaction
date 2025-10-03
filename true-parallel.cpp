#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

const std::string INPUT_FILE = "input_task2.txt";
const std::string OUTPUT_ODD_FILE = "output_odd.txt";
const std::string OUTPUT_EVEN_FILE = "output_even.txt";

struct TaskQueue {
    std::queue<std::string> q;
    std::mutex m;
    std::condition_variable cv;
    bool finished = false;
};

void worker_thread_func(TaskQueue& queue, std::ofstream& output_stream) {
    while (true) {
        std::string line_to_write;
        {
            std::unique_lock<std::mutex> lock(queue.m);
            queue.cv.wait(lock, [&]{ return !queue.q.empty() || queue.finished; });

            if (queue.finished && queue.q.empty()) {
                break;
            }

            line_to_write = queue.q.front();
            queue.q.pop();
        }

        std::cout << "  Thread (ID: " << std::this_thread::get_id() << "): Writing line to file" << std::endl;
        output_stream << line_to_write;
        output_stream.flush();
    }
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

    TaskQueue odd_queue;
    TaskQueue even_queue;

    std::thread odd_worker(worker_thread_func, std::ref(odd_queue), std::ref(output_odd_fp));
    std::thread even_worker(worker_thread_func, std::ref(even_queue), std::ref(output_even_fp));

    std::string line;
    int line_count = 0;
    while (std::getline(input_fp, line)) {
        line_count++;
        line += "\n";

        if (line_count % 2 != 0) {
            {
                std::lock_guard<std::mutex> lock(odd_queue.m);
                odd_queue.q.push(line);
                std::cout << "\nLine " << line_count << " processing... Odd (Queue size: " << odd_queue.q.size() << ")" << std::endl;
            }
            odd_queue.cv.notify_one();
        } else {
            {
                std::lock_guard<std::mutex> lock(even_queue.m);
                even_queue.q.push(line);
                std::cout << "Line " << line_count << " processing... Even (Queue size: " << even_queue.q.size() << ")" << std::endl;
            }
            even_queue.cv.notify_one();
        }
    }

    {
        std::lock_guard<std::mutex> odd_lock(odd_queue.m);
        odd_queue.finished = true;
    }
    odd_queue.cv.notify_all();

    {
        std::lock_guard<std::mutex> even_lock(even_queue.m);
        even_queue.finished = true;
    }
    even_queue.cv.notify_all();

    if (odd_worker.joinable()) odd_worker.join();
    if (even_worker.joinable()) even_worker.join();

    input_fp.close();
    output_odd_fp.close();
    output_even_fp.close();

    std::cout << "\nProcess closed. Look for 'output_odd.txt' and 'output_even.txt' for results" << std::endl;
    return 0;
}
