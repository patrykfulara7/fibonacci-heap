#pragma once

#include <cmath>
#include <list>
#include <stdexcept>
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
        if (Empty()) {
            throw std::runtime_error("Heap is empty");
        }

        return min->value;
    }

    NodeIterator Push(T &&value) {
        NodeIterator it = roots.emplace(roots.end(), std::forward<T>(value), roots.end());
        if (min == roots.end() or value < min->value) {
            min = it;
        }

        return it;
    }

    void Pop() {
        if (Empty()) {
            throw std::runtime_error("Heap is empty");
        }

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
        NodeIterator it = roots.begin();
        do {
            NodeIterator current = it++;

            std::size_t degree = current->children.size();
            while (nodes[degree] != roots.end()) {
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

                degree++;
            }

            nodes[degree] = current;

            if (current->value < min->value) {
                min = current;
            }
        } while (it != roots.end());
    }

    // TODO: Handle value greater than value being updated
    void Update(NodeIterator &node, T &&value) {
        if (value < node->value) {
            Cut(node);

            if (value < min->value) {
                min = node;
            }
        }

        node->value = std::forward<T>(value);
    }

  private:
    void Cut(NodeIterator &node) {
        NodeIterator parent = node->parent;
        if (parent == roots.end()) {
            return;
        }

        roots.splice(roots.end(), node->parent->children, node);
        node->parent = roots.end();
        node->marked = false;

        if (not parent->marked) {
            parent->marked = true;
        } else {
            Cut(parent);
        }
    }
};
