#include <bits/stdc++.h>
#include <gomory_hu.h>
using namespace std;

int main() {
    vector<tuple<int, int, int>> graph;
    auto tree = gomory_hu(graph);
    return 0;
}