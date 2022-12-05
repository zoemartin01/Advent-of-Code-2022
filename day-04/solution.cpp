#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <sstream>

using namespace std;

class Section
{
public:
    int start;
    int end;

    Section(int _start, int _end)
    {
        start = _start;
        end = _end;
    }
};

vector<tuple<Section, Section>> parse()
{
    vector<tuple<Section, Section>> sections;
    ifstream infile("input.txt");
    string line;
    while (getline(infile, line))
    {
        replace(line.begin(), line.end(), ',', ' ');
        replace(line.begin(), line.end(), '-', ' ');

        istringstream s(line);
        int a, b, x, y;
        s >> a >> b >> x >> y;

        sections.emplace_back(Section(a, b), Section(x, y));
    }
    return sections;
}

bool contain(const Section &s1, const Section &s2)
{
    return (s1.start <= s2.start && s1.end >= s2.end) || (s2.start <= s1.start && s2.end >= s1.end);
}

bool in(const Section &s, const int &i)
{
    return (s.start <= i && s.end >= i);
}

bool overlap(const Section &s1, const Section &s2)
{
    return (in(s1, s2.start) || in(s1, s2.end) || in(s2, s1.start) || in(s2, s1.end));
}

int main()
{
    auto parsed = parse();
    vector<tuple<Section, Section>> fully_contained;
    vector<tuple<Section, Section>> overlapping;

    copy_if(parsed.begin(), parsed.end(), back_inserter(fully_contained),
            [](const auto &t)
            {
                auto &[s1, s2] = t;
                return contain(s1, s2);
            });

    copy_if(parsed.begin(), parsed.end(), back_inserter(overlapping),
            [](const auto &t)
            {
                auto &[s1, s2] = t;
                return overlap(s1, s2);
            });

    cout << "Fully contained pairs: " << fully_contained.size() << endl;
    cout << "Overlapping pairs: " << overlapping.size() << endl;
    return 0;
}
