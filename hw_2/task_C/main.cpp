#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include <functional>
#include <iomanip>

template <typename Type>
class comp {
public:
    bool operator () (
        std::pair <size_t, Type> &a,
        std::pair <size_t, Type> &b
    ) {
        return a.second < b.second;
    }
};

template <typename Type>
Type deikstra (
    const std::vector < std::vector < std::pair <size_t, Type> > > &graph,
    size_t s,
    size_t f,
    const std::function< Type (Type , Type) > &way_expression = [] (ssize_t old, ssize_t weight) -> ssize_t {
        return old + weight;
    }
) {
    std::priority_queue <
        std::pair <size_t, Type>,
        std::vector < std::pair <size_t, Type> >,
        comp <Type>
    > order( (comp<Type>()) );
    std::vector <Type> ways (graph.size(), std::numeric_limits<Type>::max());
    ways[s] = static_cast<Type> (0);
    order.emplace(s, ways[s]);
    for (size_t i = 0; i < graph.size(); ++i) {
        size_t v = order.top().first;
        Type dist = -order.top().second;
        order.pop();
        if (dist != ways[v]) {
            continue;
        }
        if (dist == std::numeric_limits<Type>::max()) {
            break;
        }
        for (size_t j = 0; j < graph[v].size(); ++j) {
            Type way = way_expression(ways[v], graph[v][j].second);
            if (ways[graph[v][j].first] > way) {
                ways[graph[v][j].first] =  way;
                order.emplace(graph[v][j].first, -way);
            }
        }
    }
    return ways[f] != std::numeric_limits<Type>::max() ? ways[f] : static_cast<Type> (-1);
}

template <typename Type>
void readGraph (
    std::istream &in,
    std::vector < std::vector < std::pair <size_t, Type> > > &graph,
    size_t n,
    size_t m
) {
    size_t from, to;
    Type weight;
    graph.assign(n, std::vector <std::pair <size_t, Type> > () );
    for (size_t i = 0; i < m; ++i) {
        in >> from >> to >> weight;
        graph[from - 1].emplace_back(to - 1, weight / 100.0);
        graph[to - 1].emplace_back(from - 1, weight / 100.0);
    }
}

template <typename Type>
void getInput (
    std::istream &in,
    std::vector < std::vector < std::pair <size_t, Type> > > &graph,
    size_t &n,
    size_t &m,
    size_t &s,
    size_t &f
) {
    in >> n >> m >> s >> f;
    --s;
    --f;
    readGraph<Type>(in, graph, n, m);
}

void setOutput (
    std::ostream &out,
    double probability
) {
    out << std::setprecision(6) << probability;
}

int main() {
    std::vector < std::vector < std::pair <size_t, double> > > graph;
    size_t n, m, s, f;
    getInput <double> (std::cin, graph, n, m, s, f);
    setOutput(
       std::cout,
       deikstra <double> (graph, s, f, [] (double a, double b) -> double {
           return (1 - (1 - a) * (1 - b));
       }
    ));
    return 0;
}
