#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>

using namespace std;

typedef vector<int> Compartment;
typedef tuple<Compartment, Compartment> Bag;

vector<Bag> parse()
{
    vector<Bag> bags = {};
    ifstream infile("input.txt");
    string line;
    while (getline(infile, line))
    {
        Compartment first = {};
        Compartment second = {};
        size_t len = line.length();
        for (size_t i = 0; i < len; i++)
        {
            Compartment &active = i < len / 2 ? first : second;
            char c = line[i];
            if (c >= 'A' && c <= 'Z')
                active.push_back(c - 'A' + 27);
            if (c >= 'a' && c <= 'z')
                active.push_back(c - 'a' + 1);
        }
        bags.emplace_back(first, second);
    }
    return bags;
}

bool in(const vector<int> &v, const int &x)
{
    return (find(v.begin(), v.end(), x) != v.end());
}

int find(const Bag &bag)
{
    auto &[first, second] = bag;
    for (int i : first)
    {
        if (in(second, i))
            return i;
    }
    return -1;
}

vector<int> merge(const Bag &b)
{
    auto [f, s] = b;
    f.insert(f.end(), s.begin(), s.end());
    return f;
}

int find_badge(const Bag &b1, const Bag &b2, const Bag &b3)
{
    auto first = merge(b1);
    auto second = merge(b2);
    auto third = merge(b3);

    for (int i : first)
    {
        if (in(second, i) && in(third, i))
            return i;
    }
    return -1;
}

int main()
{
    vector<Bag> bags = parse();
    vector<int> found;
    transform(bags.begin(), bags.end(), back_inserter(found),
              [](const Bag &b)
              { return find(b); });
    vector<int> badges;
    for (size_t i = 0; i < bags.size(); i += 3)
    {
        badges.push_back(find_badge(bags[i], bags[i + 1], bags[i + 2]));
    }

    cout << "Sum 1: " << reduce(found.begin(), found.end()) << endl;
    cout << "Badges Sum: " << reduce(badges.begin(), badges.end()) << endl;
    return 0;
}