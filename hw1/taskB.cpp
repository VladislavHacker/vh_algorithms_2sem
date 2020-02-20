/* Троян-Головян Владислав */
/* Д. З. 1, задача B "Секретные материалы" */
/* Джон Макклейн сообщает по рации новую информацию о террористах в отдел с n полицейскими. Он звонит нескольким сотрудникам и просит распространить информацию по отделу,
 * зная, что у каждого полицейского есть связь с определёнными коллегами. Джон Макклейн хочет, чтобы операция прошла успешно. Но если полицейский позвонит коллеге, от которого
 * (возможно, не напрямую) сам получил информацию, террористы смогут отследить этот звонок и помешать операции. Если же двое сотрудников оповестят одного, ничего плохого не произойдёт.
 * Помогите Джону Макклейну. Выведите NO, если кто-то из полицейских ошибётся, делая звонок. Если всё пройдёт хорошо, выведите YES и порядок, в котором полицейские
 * получат информацию, считая, что полицейские оповещают коллег по возрастанию их номеров, а в начале Джон даёт информацию тем, кому не может позвонить никто из коллег.   */

#include <iostream>
#include <vector>
/* Поиск в глубину */
bool dfs(const std::vector<std::vector<int>> &graph, int v, std::vector<char> &colors,  std::vector<int> &sort_result) {
     colors[v] = 1;
     bool res = true;
     for (int i = 0; i < graph[v].size() && res; ++i) {
         if (colors[graph[v][i]] == 0) {
             res = res && dfs(graph, graph[v][i], colors, sort_result);
         } else if (colors[graph[v][i]] == 1) {
             res = false;
         }
     }
     colors[v] = 2;
     sort_result.push_back(v);
     return res;
}
/* Топологическая сортировка */
bool top_sort(const std::vector<std::vector<int>> &graph, std::vector<int> &sort_result) {
    std::vector<char> colors(graph.size(), 0);
    sort_result.clear();
    bool res = true;
    for (int i = 0; i < graph.size() && res; ++i) {
        if (colors[i] == 0)
            res = res && dfs(graph, i, colors, sort_result);
    }
    return res;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> graph(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        graph[a].push_back(b);
    }
    std::vector<int> sort_result;
    if (top_sort(graph, sort_result)) {
        std::cout << "YES" << std::endl;
        for (int i = sort_result.size() - 1; i >= 0; --i) {
            std::cout << sort_result[i] << " ";
        }
    } else {
        std::cout << "NO";
    }
    return 0;
}
