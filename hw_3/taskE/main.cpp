#include <limits>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <vector>

enum {
    unvisited = 0,
    visited = 1
};

void getInput (
    std::istream &in,
    std::vector <std::vector < std::tuple <size_t, long long, long long> > > &graph,
    size_t &n,
    size_t &m,
    size_t &s,
    size_t &t
) {
    in >> n >> m >> s >> t;
    --s;
    --t;
    graph.assign(n, std::vector < std::tuple <size_t, long long, long long> > ());
    size_t from, to;
    for (size_t i = 0; i < m; ++i) {
        in >> from >> to;

        bool st = false;

        if (from - 1 != to - 1) {
            graph[from - 1].emplace_back(to - 1, 1, 0);
            graph[to - 1].emplace_back(from - 1, 0, 0);
        }

    }

}

void changeBackEdge (
        std::vector <std::vector < std::tuple <size_t, long long, long long> > > &graph,
        size_t v,
        size_t u,
        long long delta
) {
    for (std::tuple <size_t, long long, long long> &edge : graph[v]) {
        if (std::get<0> (edge) == u) {
            std::get<2> (edge) -= delta;
            break;
        }
    }
}

size_t fordFalkerson (
        std::vector <std::vector < std::tuple <size_t, long long, long long> > > &graph,
        std::vector <char> &is_visited,
        std::vector <size_t> &way,
        size_t start,
        size_t finish,
        size_t current,
        bool &status
) {
    if (status) {
        return 0;
    }
    if (start == finish) {
        status = true;
        way.push_back(start);
        return current;
    }
    is_visited[start] = visited;
    size_t delta = 0;
    for (std::tuple <size_t, long long, long long> &edge : graph[start]) {
        size_t v = std::get<0>(edge);
        long long c = std::get<1>(edge);
        long long f = std::get<2>(edge);
        if (is_visited[v] == unvisited && f < c) {
            delta = fordFalkerson(graph, is_visited, way, v, finish, current < c - f ? current : c - f, status);
            if (delta > 0) {
                std::get<2>(edge) += delta;
                changeBackEdge(graph, v, start, delta);
                if (status) {
                    way.push_back(v);
                }
                return delta;
            }
        }
    }
    return delta;
}

void getWay (
        std::vector <std::vector < std::pair <size_t, long long> > > &graph,
        std::vector <char> &used,
        std::vector <size_t> &way,
        size_t v,
        size_t finish,
        bool &status
) {
    if (status) {
        return;
    }
    if (v == finish) {
        status = true;
        way.push_back(v);
        return;
    }
    used[v] = visited;
    for (std::pair <size_t, long long> &u : graph[v]) {
        if (u.second == 1 && used[u.first] == unvisited) {
            getWay(graph, used, way, u.first, finish, status);
            if (status) {
                way.push_back(v);
                break;
            }
        }
    }
}

void removeEdges (
        std::vector <std::vector < std::pair <size_t, long long> > > &graph,
        std::vector <size_t> &way
) {
    for (ssize_t i = way.size() - 1; i > 0; --i)  {
        for (std::pair <size_t, long long> &u : graph[way[i]]) {
            if (u.first == way[i - 1]) {
                u.second = 0;
                break;
            }
        }
    }
}

void fixWays (
    std::vector <size_t> &shrek_way,
    std::vector <size_t> &donkey_way,
    size_t n
) {
    std::vector <std::vector<std::pair <size_t, long long>>> graph(n, std::vector<std::pair <size_t, long long>>());
    std::vector <ssize_t> a(n, -1);
    std::vector <ssize_t> b(n, -1);
    for (size_t i = 1; i < donkey_way.size(); ++i) {
        a[donkey_way[i - 1]] = donkey_way[i];
    }
    for (size_t i = 1; i < shrek_way.size(); ++i) {
        if (a[shrek_way[i]] != shrek_way[i - 1]) {
            graph[shrek_way[i - 1]].emplace_back(shrek_way[i], 1);
            graph[shrek_way[i]].emplace_back(shrek_way[i - 1], 1);
        }
    }
    for (size_t i = 1; i < shrek_way.size(); ++i) {
        b[shrek_way[i - 1]] = shrek_way[i];
    }
    for (size_t i = 1; i < donkey_way.size(); ++i) {
        if (b[donkey_way[i]] != donkey_way[i - 1]) {
            graph[donkey_way[i - 1]].emplace_back(donkey_way[i], 1);
            graph[donkey_way[i]].emplace_back(donkey_way[i - 1], 1);
        }
    }
    std::vector <size_t> way1, way2;
    std::vector <char> used1(n, unvisited);
    bool status1 = false;
    getWay(graph, used1, way1, shrek_way[0], *shrek_way.end(), status1);
    removeEdges(graph, way1);
    bool status2 = false;
    std::vector <char> used2(n, unvisited);
    getWay(graph, used2, way2, shrek_way[0], *shrek_way.end(), status2);
    shrek_way = way1;
    donkey_way = way2;
}

int main () {
    size_t n, m , s, t;
    std::vector <std::vector < std::tuple <size_t, long long, long long> > > graph;
    getInput(std::cin, graph, n, m, s, t);

    std::vector <size_t> shrek_way, donkey_way;
    std::vector <char> is_visited(n, unvisited);
    bool status1 = false;
    size_t result;

    result = fordFalkerson(graph, is_visited, shrek_way, s, t, std::numeric_limits<size_t>::max(), status1);

    is_visited.assign(n, unvisited);
    bool status2 = false;
    result = fordFalkerson(graph, is_visited, donkey_way, s, t, std::numeric_limits<size_t>::max(), status2);

    shrek_way.push_back(s);
    donkey_way.push_back(s);
    std::reverse(shrek_way.begin(), shrek_way.end());
    std::reverse(donkey_way.begin(), donkey_way.end());
    shrek_way.pop_back();
    donkey_way.pop_back();
    fixWays(shrek_way, donkey_way, n);

    if (status1 && status2) {
        std::cout << "YES" << std::endl;
        for (ssize_t i = shrek_way.size() - 1; i >= 0; --i) {
            std::cout << shrek_way[i] + 1 << " ";
        }

        std::cout << std::endl;
        for (ssize_t i = donkey_way.size() - 1; i >= 0; --i) {
            std::cout << donkey_way[i] + 1 << " ";
        }
    } else {
        std::cout << "NO";
    }

    return 0;
}
