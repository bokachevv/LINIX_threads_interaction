# LINUX threads interaction
## BRIEF
This repository clearly demonstrates how threads interact with each other in Linux.
## KEY POINTS
- The program opens an input file and two output files. It should then read the input file line by line in a loop and spawn two threads.
- One thread should receive the odd line, and the other the even line. Both threads should run in parallel. Each thread writes the received line to its output file and terminates.
- The program should wait for each thread to complete and repeat the cycle of spawning threads and reading lines from the input file until it reads the last line, after which it should close all files.
## LAUNCH
1. Locate root and install GNU C++ compiler
```bash
cd
sudo apt install g++
```
2. Locate parent folder
```bash
cd PARENT_ROOT
```
3. Git clone project to desired directory
```bash   
git clone https://github.com/bokachevv/LINUX_threads_interaction DESIRED_DIR
```
4. Locate desired directory and compile program
```bash
cd DESIRED_ROOT
g++ -o main main.cpp
```
5. Launch with desired arguments
```bash
./main result.txt 1 5 3
```
6. Open .txt to see results
```bash
cat result.txt
```