#pragma once

#include <cmath>
#include <list>
#include <vector>

template <typename T>
class FibonacciHeap {
    class Node;

  public:
    typedef typename std::list<Node>::iterator NodeIterator;

  private:
    struct Node {
        T value;
        NodeIterator parent;
        std::list<Node> children {};
        bool marked { false };

        Node(T value, const NodeIterator &parent) : value(value), parent(parent) {
        }
    };

    std::list<Node> roots {};
    NodeIterator min { roots.end() };

  public:
    bool Empty() const {
        return roots.empty();
    }

    T &Top() const {
        return min->value;
    }

    NodeIterator Push(T &&value) {
        roots.emplace_back(std::forward<T>(value), roots.end());
        if (min == roots.end() or value < min->value) {
            min = std::prev(roots.end());
        }

        return std::prev(roots.end());
    }

    void Pop() {
        roots.splice(roots.end(), min->children);
        roots.erase(min);

        std::size_t maxDegree = std::floor(std::log2(roots.size())) + 1;
        std::vector<NodeIterator> degrees(maxDegree, roots.end());
        for (NodeIterator current = roots.begin(); current != roots.end();) {
            NodeIterator next = std::next(current);

            while (degrees[current->children.size()] != roots.end()) {
                NodeIterator other = degrees[current->children.size()];
                if (other->value < current->value) {
                    std::swap(other, current);
                }

                if (other->parent == roots.end()) {
                    current->children.splice(current->children.end(), roots, other);
                } else {
                    current->children.splice(current->children.end(), other->parent->children, other);
                }

                other->parent = current;
                degrees[current->children.size() - 1] = roots.end();
            }

            degrees[current->children.size()] = current;
            current = next;
        }

        min = roots.end();
        for (NodeIterator &node : degrees) {
            if (node != roots.end() and (min == roots.end() or node->value < min->value)) {
                min = node;
            }
        }
    }

    void Update(NodeIterator &node, T &&value) {
        if (value < node->value) {
            NodeIterator current = node;
            while (true) {
                NodeIterator parent = node->parent;
                if (parent == roots.end()) {
                    break;
                }

                roots.splice(roots.end(), node->parent->children, node);
                node->marked = false;
                node->parent = roots.end();

                if (not parent->marked) {
                    parent->marked = true;
                    break;
                }

                current = parent;
            }

            if (value < min->value) {
                min = node;
            }
        }

        node->value = std::forward<T>(value);
    }
};
