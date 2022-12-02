#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> parse() {
    std::ifstream infile("input.txt");
    std::string line;
    std::vector<int> elves;
    int elf = 0;
    while(std::getline(infile, line)) {
        if (line.empty()) {
            elves.push_back(elf);
            elf = 0;
        } else {
            elf += std::stoi(line);
        }
    }
    return elves;
}

int main() {
    std::vector<int> elves = parse();
    std::sort(elves.begin(), elves.end());

    int top = elves.back();
    int top3 = 0;
    for (int i = 0; i < 3; i++) {
        top3 += elves.back();
        elves.pop_back();
    }

    std::cout << "Top: " << top << std::endl;
    std::cout << "Top 3: " << top3 << std::endl;
    return 0;
}