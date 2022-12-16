#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <set>
#include <tuple>
#include <map>

std::map<size_t, long> parse() {
    std::ifstream file("input.txt");

    size_t cycle_counter = 0;
    std::map<size_t, long> registry = {{0, 1}};
    for (std::string line; std::getline(file, line);) {
        if (line != "noop") {
            std::stringstream stream(line);
            std::string _;
            long x;
            stream >> _ >> x;
            registry[cycle_counter + 1] = registry[cycle_counter];
            registry[cycle_counter + 2] = x + registry[cycle_counter];
            cycle_counter += 2;
        } else {
            registry[cycle_counter + 1] = registry[cycle_counter];
            cycle_counter++;
        }
    }
    return registry;
}

size_t part_one(std::map<size_t, long> &registry) {
    long sum = 0;
    for (size_t i = 20; i <= 220; i += 40) {
        sum += registry[i - 1] * i;
    }

    return sum;
}

auto part_two(std::map<size_t, long> &registry) {
    for (size_t i = 1; i <= 240; i++) {
        long value_during_cycle = registry[i - 1];
        long pixel_index = (i - 1) % 40;
        bool visible = value_during_cycle >= pixel_index - 1 && value_during_cycle <= pixel_index + 1;
        std::cout << (visible ? "#" : ".");
        if (i % 40 == 0) std::cout << std::endl;
    }
}

int main()
{
    std::map<size_t, long> registry = parse();
    std::cout << "Part One: " << part_one(registry) << std::endl;
    std::cout << "Part Two: " << std::endl;
    part_two(registry);
    return 0;
}
