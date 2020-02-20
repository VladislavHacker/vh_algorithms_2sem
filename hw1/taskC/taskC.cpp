/* Троян-Головян Владислав */
/* Д. З. 1, задача C "Районы, кварталы, жилые массивы" */
/* Фрэнку опять прилетел новый заказ. Однако в этот раз город
 * играет по очень странным правилам: все дороги в городе односторонние и
 * связывают только офисы нанимателей перевозчика. Множество офисов, между любыми двумя из
 * которых существует путь, образуют квартал, если нельзя добавить никакие другие, чтобы условие выполнялось.
 * Фрэнку интересно, каково минимальное количество односторонних дорог нужно ещё построить, чтобы весь город
 * стал квраталом.  */
#include <iostream>
#include <vector>
#include <utility>
/* DFS для получения порядка обхода */
void dfs(std::vector<std::vector<int>> &graph, int v, std::vector<char> &colors, std::vector<int> &order) {
    colors[v] = 1;
    for (int i = 0; i < graph[v].size(); ++i) {
        if (colors[graph[v][i]] == 0)
            dfs(graph, graph[v][i], colors, order);
    }
    colors[v] = 2;
    order.push_back(v);
}
/* DFS для поиска компонент связности */
void component_dfs(std::vector<std::vector<int>> &graph, int v, std::vector<char> &colors, std::vector<int> &component, std::vector<int> &v_comp, int comp_num) {
    component.push_back(v);
    colors[v] = 1;
    v_comp[v] = comp_num;
    for (int i = 0; i < graph[v].size(); ++i) {
        if (colors[graph[v][i]] == 0)
            component_dfs(graph, graph[v][i], colors, component, v_comp, comp_num);
    }
    colors[v] = 2;
}
/* Конденсация на основании данных полученым с помощью алгоритма Косарайю */
void condense_graph(std::vector<std::vector<int>> &graph, std::vector<std::vector<int>> &condensed_graph, std::vector<int> v_comp) {
    for (int i = 0; i < graph.size(); ++i)
        for (int j = 0; j < graph[i].size(); ++j)
            if (v_comp[i] != v_comp[graph[i][j]])
                condensed_graph[v_comp[i]].push_back(v_comp[graph[i][j]]);
}
/* Функция для получения конденсированного орграфа из исходного */
void make_condensed_graph(std::vector<std::vector<int>> &graph, std::vector<std::vector<int>> &trans_graph, std::vector<std::vector<int>> &condensed_graph) {
    std::vector<int> order;
    std::vector<char> colors(graph.size(), 0);

    for (int i = 0; i < graph.size(); ++i) {
        if (colors[i] == 0) {
            dfs(graph, i, colors, order);
        }
    }

    colors.assign(graph.size(), 0);
    std::vector<std::vector<int>> components;
    std::vector<int> v_comp(graph.size(), 0);

    for (int i = graph.size() - 1; i >= 0; --i) {
        if (colors[order[i]] == 0) {
            std::vector<int> component;
            component_dfs(trans_graph, order[i], colors, component, v_comp, components.size());
            components.push_back(component);
        }
    }

    condensed_graph.clear();
    condensed_graph.assign(components.size(), std::vector<int> ());
    condense_graph(graph, condensed_graph, v_comp);
}
/* Поиск min количества дорог, необходимых чтобы дополнить ациклический орграф до сильной связности */
int count_roads(std::vector<std::vector<int>> &graph,  std::vector<std::pair<int, int>> &in_out) {
    if (graph.size() == 1)
        return 0;
    for (int i = 0; i < graph.size(); ++i) {
        in_out[i].first += graph[i].size();
        for (int j = 0; j < graph[i].size(); ++j) {
            in_out[graph[i][j]].second++;
        }
    }
    int count1 = 0;
    int count2 = 0;
    for (int i = 0; i < in_out.size(); ++i) {
        if (in_out[i].first == 0)
            count1++;
        if (in_out[i].second == 0)
            count2++;
    }
    return count1 < count2 ? count2 : count1;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> graph(n);
    std::vector<std::vector<int>> trans_graph(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        a--;
        b--;
        graph[a].push_back(b);
        trans_graph[b].push_back(a);
    }
    std::vector<std::vector<int>> condensed_graph;
    make_condensed_graph(graph, trans_graph, condensed_graph);
    std::vector<std::pair<int, int>> in_out_counts(condensed_graph.size(), std::pair<int, int>(0, 0));
    std::cout << count_roads(condensed_graph, in_out_counts);
    return 0;
}