#include <algorithm>
#include <iostream>
#include <vector>

void getInput (
    std::istream &in,
    std::vector < std::pair < size_t, std::pair <size_t, size_t> > > &graph,
    size_t &n,
    size_t &m
) {
    in >> n >> m;
    for (size_t i = 0; i < m; ++i) {
        size_t from, to, weight;
        in >> from >> to >> weight;
        --from;
        --to;
        graph.emplace_back(weight, std::pair <size_t, size_t> (from, to));
        graph.emplace_back(weight, std::pair <size_t, size_t> (to, from));
    }
}

void setOutput (
    std::ostream &out,
    size_t size
) {
    out << size;
}

size_t kraskal(
    std::vector < std::pair < size_t, std::pair <size_t, size_t> > > &graph,
    size_t n
) {
    std::sort(graph.begin(), graph.end());
    std::vector < size_t > result;
    std::vector <size_t> tree;
    for (size_t i = 0; i < n; ++i) {
        tree.push_back(i);
    }
    for (size_t i = 0; i < graph.size(); ++i) {
        size_t weight = graph[i].first;
        size_t from = graph[i].second.first;
        size_t to = graph[i].second.second;
        if (tree[from] != tree[to]) {
            size_t old_to = tree[to];
            for (size_t j = 0; j < n; ++j) {
                if (tree[j] == old_to) {
                    tree[j] = tree[from];
                }
            }
            result.push_back(weight);
        }
    }
    size_t cost = 0;
    for (size_t i = 0; i < result.size(); ++i) {
        cost += result[i];
    }
    return cost;
}

int main() {
    std::vector < std::pair < size_t, std::pair <size_t, size_t> > > graph;
    size_t n, m;
    getInput(std::cin, graph, n, m);
    setOutput(std::cout, kraskal(graph, n));
    return 0;
}
