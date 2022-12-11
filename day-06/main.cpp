#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <sstream>
#include <set>

using namespace std;

string parse() {
    ifstream infile("input.txt");
    string line;
    getline(infile, line);
    return line;
}

set<int> getFirstNElements(string array, int i, int n) {
    set<int> result;
    for (int j = i; j < i + n; j++) {
        result.insert(array[j]);
    }
    return result;
}


int findMarker(int markerLength) {
    string str = parse();
    for (int i = 0; i < str.length(); i++) {
        if (getFirstNElements(str, i, markerLength).size() == markerLength)
            return i + markerLength;
    }
    return 0;
}

int main()
{
    cout << "First Packet Marker: " << findMarker(4) << endl;
    cout << "First Message Marker: " << findMarker(14) << endl;
}
