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
        NodeIterator it = roots.emplace_back(std::forward<T>(value), roots.end());
        if (min == roots.end() or value < min->value) {
            min = it;
        }

        return it;
    }

    void Pop() {
        for (Node &child : min->children) {
            child.parent = roots.end();
        }

        roots.splice(roots.end(), min->children);
        roots.erase(min);

        if (Empty()) {
            min = roots.end();
            return;
        }

        min = roots.begin();
        std::size_t maxDegree = std::floor(std::log2(roots.size())) + 1;
        std::vector<NodeIterator> nodes(maxDegree, roots.end());
        for (NodeIterator it = roots.begin(); it != roots.end();) {
            NodeIterator current = it++;

            std::size_t degree = current->children.size();
            for (; nodes[degree] != roots.end(); degree++) {
                NodeIterator other = nodes[degree];
                if (other->value < current->value) {
                    std::swap(other, current);
                }

                if (other->parent == roots.end()) {
                    current->children.splice(current->children.end(), roots, other);
                } else {
                    current->children.splice(current->children.end(), other->parent->children, other);
                }

                other->parent = current;
                nodes[degree] = roots.end();
            }

            nodes[degree] = current;

            if (current->value < min->value) {
                min = current;
            }
        }
    }

    // TODO: Handle value greater than value being updated
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
