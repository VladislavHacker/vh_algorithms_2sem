#include <iostream>
#include <limits>
#include <string>
#include <vector>

enum {
    no_edge = 0,
    has_edge = 1
};

size_t minCut (
    std::vector < std::vector <size_t> > &graph,
    std::vector <size_t> &first_side,
    std::vector <size_t> &second_side
) {
    std::vector < std::vector <size_t> > v(graph.size());
    std::vector <size_t> best_cut;
    for (size_t i = 0; i < graph.size(); ++i) {
        v[i].push_back(i);
    }
    long long prev = 0;
    std::vector <char> in_a(graph.size(), 0);
    std::vector <size_t> w(graph.size(), 0);
    std::vector <bool> exists(graph.size(), true);
    size_t best_cost = std::numeric_limits<size_t>::max();
    for (size_t phase = 0; phase < graph.size() - 1; ++phase) {
        in_a.assign(graph.size(), 0);
        w.assign(graph.size(), 0);
        for (size_t i = 0; i < graph.size() - phase; ++i) {
            long long sel = -1;
            for (size_t j = 0; j < graph.size(); ++j) {
                if (exists[j] && in_a[j] == 0 && (sel == -1 || w[j] > w[sel])) {
                    sel = j;
                }
            }
            if (i == graph.size() - phase - 1) {
                if (w[sel] < best_cost) {
                    best_cost = w[sel];
                    best_cut = v[sel];
                }
                v[prev].insert(v[prev].end(), v[sel].begin(), v[sel].end());
                for (int j = 0; j < graph.size(); ++j) {
                    graph[j][prev] += graph[sel][j];
                    graph[prev][j] = graph[j][prev];
                }
                exists[sel] = false;
            } else {
                in_a[sel] = 1;
                for (int j = 0; j < graph.size(); ++j) {
                    w[j] += graph[sel][j];
                }
                prev = sel;
            }
        }
    }
    second_side = best_cut;
    first_side.assign(0, 0);
    for (size_t i = 0; i < graph.size(); ++i) {
        bool res = true;
        for (size_t j = 0; j < best_cut.size(); ++j) {
            if (i == best_cut[j]) {
                res = false;
            }
        }
        if (res) {
            first_side.push_back(i);
        }
    }
}

void getInput (
    std::istream &in,
    std::vector < std::vector <size_t> > &graph
) {
    size_t n;
    in >> n;
    graph.assign(n, std::vector <size_t> (n, no_edge));
    for (size_t i = 0; i < n; ++i) {
        std::string line;
        in >> line;
        for (size_t j = 0; j < n; ++j) {
            if (line[j] == '1') {
                graph[i][j] = has_edge;
            }
        }
    }
}

void setOutput (
    std::ostream &out,
    std::vector <size_t> &first_side,
    std::vector <size_t> &second_side
) {
    for (size_t v : first_side) {
        out << v + 1 << " ";
    }
    out << std::endl;
    for (size_t v : second_side) {
        out << v + 1 << " ";
    }
}
int main() {
    std::vector < std::vector <size_t> > graph;
    getInput(std::cin, graph);

    std::vector <size_t> first_side;
    std::vector <size_t> second_side;

    minCut(graph, first_side, second_side);

    setOutput(std::cout, first_side, second_side);
    return 0;
}
