#include <iostream>
#include <vector>
#include <limits>
#include <tuple>

void getInput (
    std::istream &in,
    std::vector < std::tuple <size_t, size_t, double, double> > &graph,
    size_t &n,
    size_t &m,
    size_t &s,
    double &money
) {
    in >> n >> m >> s >> money;
    --s;
    int a, b;
    double rab, cab, rba, cba;

    for (size_t i = 0; i < m; ++i) {
        std::cin >> a >> b >> rab >> cab >> rba >> cba;
        --a;
        --b;
       graph.emplace_back(a, b, rab, cab);
       graph.emplace_back(b, a, rba, cba);
    }
}

bool fordBellman (
    std::vector < std::tuple <size_t, size_t, double, double> > &graph,
    size_t n,
    size_t first_currency,
    double money
) {
    std::vector<double> d(n,0);
    d[first_currency] = money;
    long long k = -1;
    for (size_t i = 0; i <= n; ++i) {
        k = -1;
        for (size_t j = 0; j < graph.size(); ++j) {
            if (d[std::get<1>(graph[j])] <
                    (d[std::get<0>(graph[j])]  - std::get<3>(graph[j])) * std::get<2>(graph[j])) {
                d[std::get<1>(graph[j])] =
                        (d[std::get<0>(graph[j])]  - std::get<3>(graph[j])) * std::get<2>(graph[j]);
                k = std::get<0>(graph[j]);
            }
        }
    }
    return k != -1;
}

void setOutput (
    std::ostream &out,
    bool result
) {
    out << (result ? "YES" : "NO");
}

int main() {
    size_t n, m, s;
    double money;
    std::vector < std::tuple <size_t, size_t, double, double> > graph;
    getInput(std::cin, graph, n, m, s, money);
    setOutput(std::cout, fordBellman(graph, n, s, money));
    return 0;
}