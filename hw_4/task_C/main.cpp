#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <limits>

class SegmentTree {
public:

    explicit SegmentTree (const std::vector <size_t> &tickets, size_t size) {
        size_t n = 2 * (1 << (size_t)ceil(log2((double)size)));
        tree_.assign(n - 1, std::make_pair(std::numeric_limits <size_t >::max(), -1));
        size_ = n / 2;
        for (size_t i = 0; i < tickets.size(); ++i) {
            tree_[tree_.size() / 2 + i].first = tickets[i];
        }

        for (size_t i = tree_.size() / 2; i >= 1; --i) {
            tree_[i - 1].first = std::min(
                tree_[2 * (i - 1) + 1].first,
                tree_[2 * (i - 1) + 2].first
            );
        }
    }

    size_t size () {
        return size_;
    }
    size_t getMin (size_t node, size_t tl, size_t tr, size_t l, size_t r) {
        if (tl > tr || l > r) {
            return std::numeric_limits <size_t >::max();
        }
        if (l == tl && r == tr) {
            return tree_[node].first;
        }
        
        push(node);
        
        size_t p = tl + (tr - tl) / 2;
        size_t result = std::min (
                getMin(2 * node + 1, tl, p, l, std::min(r, p)),
                getMin(2 * node + 2, p + 1, tr, std::max(l, p + 1), r)
        );
        return result;
    }

    void update (size_t node, size_t tl, size_t tr, size_t l, size_t r, size_t color) {
        if (tl > tr) {
            return;
        }
        if (l > r) {
            return;
        }
        if (l == tl && tr == r) {
            tree_[node].second = color;
            tree_[node].first = color;
        } else {

            push(node);

            size_t p = tl + (tr - tl) / 2;
            update(2 * node + 1, tl, p, l, std::min(r, p), color);
            update(2 * node + 2, p + 1, tr, std::max(l, p + 1), r, color);
            
            tree_[node].first = std::min (
                    tree_[node * 2 + 1].first,
                    tree_[node * 2 + 2].first
            );
        }
    }

private:

    void push (size_t node) {
        if (tree_[node].second != -1) {
            tree_[node * 2 + 1].second = tree_[node].second;
            tree_[node * 2 + 2].second = tree_[node].second;
            tree_[node * 2 + 1].first = tree_[node].second;
            tree_[node * 2 + 2].first = tree_[node].second;
            tree_[node].second = -1;
        }
    }

    std::vector < std::pair<size_t, long long> > tree_;
    size_t size_;

};
void getInput (
    std::istream &in,
    std::vector <size_t> &line
) {
    size_t n;
    in >> n;
    line.assign(n, 0);
    size_t r, g, b;
    for (size_t i = 0; i < n; ++i) {
        in >> r >> g >> b;
        line[i] = r + g + b;
    }
}

void setOutput (
    std::ostream &out,
    std::vector <size_t> &result
) {
    for (size_t x : result) {
        out << x << " ";
    }
}

void makeQueries (
    std::istream &query_stream,
    std::vector <size_t> &line,
    std::vector <size_t> &result
) {
    SegmentTree tree(line, line.size());
    size_t m;
    query_stream >> m;
    size_t fill_from, fill_to, r, g, b, photo_from, photo_to;
    for (size_t i = 0; i < m; ++i) {
        query_stream >> fill_from >> fill_to >> r >> g >> b >> photo_from >> photo_to;
        tree.update(0, 0, tree.size() - 1, fill_from, fill_to, r + g + b);
        result.push_back(tree.getMin(0, 0, tree.size() - 1, photo_from, photo_to));
    }
}

int main() {
    std::vector <size_t> line;
    getInput(std::cin, line);
    std::vector <size_t> result;
    makeQueries(std::cin, line, result);
    setOutput(std::cout, result);
    return 0;
}
