#pragma once

#include <cmath>
#include <list>
#include <numbers>
#include <stdexcept>
#include <vector>

template <typename T>
class FibonacciHeap {
    class Node;

  public:
    typedef typename std::list<Node>::iterator NodeIterator;

  private:
    struct Node {
        NodeIterator parent;
        T value;
        std::list<Node> children {};
        bool marked { false };

        Node(NodeIterator parent, T value) : parent(parent), value(value) {
        }

        template <typename... Args>
        Node(NodeIterator parent, Args... args) : parent(parent), value(std::forward<Args>(args)...) {
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
        NodeIterator it = roots.emplace(roots.end(), roots.end(), std::forward<T>(value));
        if (min == roots.end() or value < min->value) {
            min = it;
        }

        return it;
    }

    template <typename... Args>
    NodeIterator Emplace(Args &&...args) {
        NodeIterator it = roots.emplace(roots.end(), roots.end(), std::forward<Args>(args)...);
        if (min == roots.end() or it->value < min->value) {
            min = it;
        }

        return it;
    }

    void Pop() {
        if (Empty()) {
            throw std::runtime_error("Heap is empty");
        }

        AddChildrenToRoot(min);
        roots.erase(min);

        if (Empty()) {
            min = roots.end();
        } else {
            Consolidate();
        }
    }

    // TODO: Handle value greater than value being updated
    void Update(NodeIterator node, T &&value) {
        if (value < node->value) {
            Decrease(node, std::forward<T>(value));
        } else if (value > node->value) {
            throw std::runtime_error("Value can't be inscreased");
        }
    }

  private:
    void AddChildrenToRoot(NodeIterator node) {
        for (Node &child : node->children) {
            child.parent = roots.end();
        }

        roots.splice(roots.end(), node->children);
    }

    void Consolidate() {
        std::size_t maxDegree = std::floor(std::log(roots.size()) / std::log(std::numbers::phi)) + 1;
        std::vector<NodeIterator> nodes(maxDegree + 1, roots.end());

        min = roots.begin();
        NodeIterator it = roots.begin();
        do {
            NodeIterator current = it++;

            std::size_t degree = current->children.size();
            while (nodes[degree] != roots.end()) {
                NodeIterator other = nodes[degree];
                if (other->value < current->value) {
                    std::swap(other, current);
                }

                current->children.splice(current->children.end(), roots, other);
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

    void Decrease(NodeIterator node, T &&value) {
        if (value < node->value) {
            CascadingCut(node);

            if (value < min->value) {
                min = node;
            }
        }

        node->value = std::forward<T>(value);
    }

    void CascadingCut(NodeIterator node) {
        NodeIterator parent = node->parent;
        if (parent != roots.end()) {
            if (not parent->marked) {
                parent->marked = true;
            } else {
                Cut(node);
                CascadingCut(parent);
            }
        }
    }

    void Cut(NodeIterator node) {
        roots.splice(roots.end(), node->parent->children, node);
        node->parent = roots.end();
        node->marked = false;
    }
};
