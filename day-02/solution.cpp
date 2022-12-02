#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

vector<tuple<int,int>> parse() {
    ifstream infile("input.txt");
    vector<tuple<int,int>> v = {};
    char a, b;
    while (infile >> a >> b) {
        v.emplace_back(a - 'A', b - 'X');
    }
    return v;
}

int main() {
    auto pairs = parse();
    int first_score = 0;
    int second_score = 0;
    for (auto p : pairs) {
        auto [enemy, me] = p;

        first_score += me + 1;
        if ((enemy + 1) % 3 == me) first_score += 6;
        if (enemy == me) first_score += 3;

        second_score += me * 3;
        if (me == 2) second_score += (enemy + 1) % 3 + 1;
        if (me == 1) second_score += enemy + 1;
        if (me == 0) second_score += (enemy == 0) ? 3 : enemy ;
    }
    cout << "First Score: " << first_score << endl;
    cout << "Second Score: " << second_score << endl;

    return 0;
}