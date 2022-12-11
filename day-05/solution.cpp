#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <sstream>
#include <stack>

using namespace std;

class Move {
public:
    int amount;
    int from;
    int to;

    Move(int _amount, int _from, int _to) {
        amount = _amount;
        from = _from - 1;
        to = _to - 1;
    }
};

tuple<vector<stack<char>>, vector<Move>> parse() {
    vector<vector<char>> vectors;
    vector<Move> moves;
    ifstream infile("input.txt");
    string line;
    bool parsingMoves = false;

    while (getline(infile, line)) {
        if (line.empty()) {
            parsingMoves = true;
            continue;
        }
        if (!parsingMoves)
            for (size_t i = 1, idx = 0; i < line.length(); i += 4, idx++) {
                if (idx >= vectors.size()) vectors.emplace_back();
                vector<char> &v = vectors[idx];
                v.insert(v.cbegin(), line[i]);
            }
        else {
            istringstream stream(line);
            int amount, from, to;
            string _;
            stream >> _ >> amount >> _ >> from >> _ >> to;
            moves.emplace_back(amount, from, to);
        }
    }
    vector<stack<char>> stacks(vectors.size());
    for (size_t i = 0; i < vectors.size(); i++) {
        for (char c : vectors[i]) if (c != ' ') stacks[i].push(c);
    }

    return { stacks, moves };
}

int main()
{
    auto [stacks, moves] = parse();
    auto s1 = stacks;
    for (auto m : moves) {
        auto &from = s1[m.from];
        for (size_t i = 0; i < m.amount; i++) {
            char c = from.top();
            from.pop();
            s1[m.to].push(c);
        }
    }

    cout << "Part 1: ";
    for (auto s : s1) cout << s.top();
    cout << endl;

    auto s2 = stacks;
    for (Move m : moves) {
        auto &from = s2[m.from];
        stack<char> temp;
        for (size_t i = 0; i < m.amount; i++) {
            char c = from.top();
            from.pop();
            temp.push(c);
        }
        while(!temp.empty()) {
            s2[m.to].push(temp.top());
            temp.pop();
        }
    }

    cout << "Part 2: ";
    for (auto s : s2) cout << s.top();
    cout << endl;

    return 0;
}
