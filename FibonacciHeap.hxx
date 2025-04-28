#pragma once

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
struct Node {
    T value;
    Node<T> *parent { nullptr };
    Node<T> *prev { this };
    Node<T> *next { this };
    Node<T> *children { nullptr };
    std::size_t degree { 0 };
    bool marked { false };

    Node(T value) : value(value) {
    }
};

template <typename T>
class FibonacciHeap {
  private:
    Node<T> *roots { nullptr };
    Node<T> *min = { nullptr };
    std::size_t degree { 0 };

  private:
    void Remove(Node<T> *node) {
        for (; node->degree > 0; node->degree--) {
            Remove(node->children);
            node->children = node->children->next;
        }

        delete min;
    }

  public:
    ~FibonacciHeap() {
        for (; roots->degree > 0; roots->degree--) {
            Remove(roots->children);
            roots->children = roots->children->next;
        }
    }

    bool Empty() const {
        return degree == 0;
    }

    T &Top() const {
        if (degree == 0) {
            throw std::runtime_error("Heap is empty");
        }

        return min->value;
    }

    Node<T> *Push(T &&value) {
        Node<T> *newNode = new Node<T>(std::forward<T>(value));

        if (degree == 0) {
            roots = newNode;
            min = newNode;
        } else {
            newNode->prev = roots->prev;
            newNode->next = roots;

            roots->prev->next = newNode;
            roots->prev = newNode;

            if (value < min->value) {
                min = newNode;
            }
        }

        degree++;

        return newNode;
    }

    void Pop() {
        if (degree == 0) {
            throw std::runtime_error("Heap is empty");
        }

        if (min->degree > 0) {
            for (; min->degree > 0; min->degree--) {
                min->children->parent = nullptr;
                min->children = min->children->next;
            }

            min->children->prev->next = roots;
            roots->prev->next = min->children;
            std::swap(min->children->prev, roots->prev);

            degree += min->degree;
        }

        if (degree == 1) {
            delete min;
            roots = nullptr;
            min = nullptr;
            degree = 0;
        } else {
            roots = min->next;

            min->prev->next = min->next;
            min->next->prev = min->prev;
            delete min;
            degree--;

            std::size_t maxDegree = std::floor(std::log2(degree)) + 1;
            std::vector<Node<T> *> degrees(maxDegree, nullptr);
            for (; degree > 0; degree--) {
                Node<T> *current = roots;
                roots = roots->next;

                for (; degrees[current->degree] != nullptr; current->degree++) {
                    Node<T> *other = degrees[current->degree];
                    if (other->value < current->value) {
                        std::swap(other, current);
                    }

                    if (current->degree == 0) {
                        other->prev = other;
                        other->next = other;

                        current->children = other;
                    } else {
                        other->prev = current->children->prev;
                        other->next = current->children;

                        current->children->prev->next = other;
                        current->children->prev = other;
                    }

                    other->parent = current;
                    degrees[current->degree] = nullptr;
                }

                degrees[current->degree] = current;
            }

            roots = nullptr;
            min = nullptr;
            for (Node<T> *node : degrees) {
                if (node != nullptr) {
                    if (degree == 0) {
                        node->prev = node;
                        roots = node;
                        min = node;
                    } else {
                        node->prev = roots->prev;

                        roots->prev->next = node;
                        roots->prev = node;

                        if (node->value < min->value) {
                            min = node;
                        }
                    }

                    degree++;
                }
            }

            roots->prev->next = roots;
        }
    }

    void Update(Node<T> *node, T &&value) {
        node->value = std::forward<T>(value);

        Node<T> *current = node;
        while (true) {
            if (current->value < min->value) {
                min = current;
            }

            Node<T> *parent = current->parent;
            if (parent == nullptr) {
                break;
            }

            current->parent = nullptr;
            parent->degree--;

            if (not parent->marked) {
                parent->marked = true;
                break;
            }

            current = parent;

            current->prev = roots->prev;
            current->next = roots;

            roots->prev->next = current;
            roots->prev = current;

            current->marked = false;
            degree++;
        }
    }
};
