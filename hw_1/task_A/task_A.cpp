/* Троян-Головян Владислав */
/* Д. З. 1, задача "За молоком" */
/* Леон и Матильда собрались пойти в магазин за молоком. Их хочет поймать Стэнсфилд, поэтому нашим товарищам нужно
 * сделать это как можно быстрее. Каково минимальное количество улиц, по которым пройдёт хотя бы один из ребят (либо
 * Матильда, либо Леон, либо оба вместе)?  */

#include <iostream>
#include <vector>
#include <queue>
/* Поиск в ширину */
void BFS(const std::vector <std::vector <int> > &graph, const int v, const int f, std::vector<int> &ways) {
    std::queue<int> q;
    q.push(v);
    ways[v] = 0;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        for (auto it = graph[current].begin(); it != graph[current].end(); ++it) {
            if (ways[*it] == -1) {
                q.push(*it);
                ways[*it] = ways[current] + 1;
            }
        }
    }
}
/* Поиск количества дорог */
unsigned long long find_way_count(std::vector<int> &matilda_ways, std::vector<int> &leon_ways,  std::vector<int> &milk_ways, int size) {
    int min = matilda_ways[0] + milk_ways[0] + leon_ways[0];
    for (int i = 0; i < size; i++) {
       if (matilda_ways[i] + milk_ways[i] + leon_ways[i] < min) {
           min = matilda_ways[i] + milk_ways[i] + leon_ways[i];
       }
    }
    return min;
}

int main() {
    int n, m, matilda, leon, milk;
    std::cin >> n >>  m >> leon >> matilda >> milk;
    std::vector <std::vector <int> > graph(n);
    int a, b;
    for (int i = 0; i < m; ++i) {
        std::cin >> a >> b;
        graph[a - 1].push_back(b - 1);
        graph[b - 1].push_back(a - 1);
    }
    std::vector<int> matilda_ways(graph.size(), -1);
    BFS(graph, matilda - 1, milk - 1, matilda_ways);
    std::vector<int> leon_ways(graph.size(), -1);
    BFS(graph, leon - 1, milk - 1, leon_ways);
    std::vector<int> milk_ways(graph.size(), -1);
    BFS(graph, milk - 1, milk - 1, milk_ways);
    std::cout << find_way_count(matilda_ways, leon_ways, milk_ways, graph.size());
    return 0;
}
