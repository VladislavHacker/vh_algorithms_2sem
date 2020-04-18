#include <iostream>
#include <vector>
#include <deque>
#include <limits>
#include <algorithm>
#include <queue>

class cmp{
public:
    bool operator() (std::pair<size_t, size_t> & a, std::pair<size_t, size_t> & b) const {
        return a.second > b.second;
    }
};

size_t search(size_t m, size_t a, size_t b, size_t x, size_t y) {
    std::vector <size_t> ways(m, std::numeric_limits<size_t>::max());
    std::vector <bool> visited(m,false);
    std::priority_queue<std::pair<size_t , size_t >, std::vector <std::pair<size_t , size_t >>, cmp> order((cmp()));
    ways[x] = 0;
    order.push(std::pair<size_t ,size_t >(x, 0));
    visited[x] = true;
    while (!order.empty()) {
        if (visited[y]) {
            break;
        }
        size_t current = order.top().first;
        visited[current] = true;
        order.pop();
        size_t first_galaxy = (current + 1) % m;
        size_t second_galaxy = (current * current + 1) % m;
        ways[first_galaxy] = std::min(ways[current] + a, ways[first_galaxy]);
        ways[second_galaxy] = std::min(ways[current] + b, ways[second_galaxy]);
        if (!visited[first_galaxy]) {
            order.push(std::pair<size_t , size_t >(first_galaxy, ways[first_galaxy]));
        }
        if (!visited[second_galaxy]) {
           order.push(std::pair<size_t , size_t >(second_galaxy, ways[second_galaxy]));
        }
    }
    return ways[y];
}

int main() {
    size_t a, b, m, x, y;
    std::cin >> a >> b >> m >> x >> y;
    std::cout << search(m, a, b, x, y);
    return 0;
}
