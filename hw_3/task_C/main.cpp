#include <algorithm>
#include <iostream>
#include <vector>

class DST {
public:

    DST(size_t n) {
        parent_.assign(n, 0);
    }

    void makeSet (size_t v) {
       parent_[v] = v;
    }

    size_t findSet (size_t v) {
        if (parent_[v] == v) {
            return v;
        } else {
            parent_[v] = findSet(parent_[v]);
            return parent_[v];
        }
    }

    void unionSet (size_t first, size_t second) {
        first = findSet(first);
        second = findSet(second);
        if (first != second) {
            parent_[second] = first;
        }
    }

private:
    std::vector < size_t > parent_;
};

void getInput (
    std::istream &in,
    std::vector < std::pair < size_t, std::pair <size_t, size_t> > > &graph,
    size_t &n,
    size_t &m
) {
    in >> n >> m;
    std::vector <size_t> a(n, 0);
    for (size_t i = 0; i < n; ++i) {
        in >> a[i];
    }
    for (size_t i = 0; i < m; ++i) {
        size_t from, to, weight;
        in >> from >> to >> weight;
        --from;
        --to;
        if (weight < a[from] + a[to]) {
            graph.emplace_back(weight, std::pair<size_t, size_t>(from, to));
        }
    }
    size_t min = 0;
    for (size_t i = 0; i < n; ++i) {
        if (a[min] > a[i]) {
            min = i;
        }
    }
    for (size_t i = 0; i < n; ++i) {
        graph.emplace_back(a[i] + a[min], std::pair <size_t, size_t> (i, min));
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
    DST trees(n);
    for (size_t i = 0; i < n; ++i) {
        trees.makeSet(i);
    }
    for (size_t i = 0; i < graph.size(); ++i) {
        size_t weight = graph[i].first;
        size_t from = graph[i].second.first;
        size_t to = graph[i].second.second;
        if (trees.findSet(from) != trees.findSet(to)) {
            trees.unionSet(from, to);
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
