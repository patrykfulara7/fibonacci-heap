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
    Node<T> *previous { nullptr };
    Node<T> *next { nullptr };
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

  public:
    bool Empty() const {
        return degree == 0;
    }

    T &FindMin() const {
        if (degree == 0) {
            throw std::runtime_error("Heap is empty");
        }

        return min->value;
    }

    Node<T> *Insert(T &&value) {
        Node<T> *newNode = new Node<T>(std::forward<T>(value));

        if (degree == 0) {
            newNode->previous = newNode;
            newNode->next = newNode;

            roots = newNode;
            min = newNode;
        } else {
            newNode->previous = roots->previous;
            newNode->next = roots;

            roots->previous->next = newNode;
            roots->previous = newNode;

            if (value < min->value) {
                min = newNode;
            }
        }

        degree++;

        return newNode;
    }

    void DeleteMin() {
        if (degree == 0) {
            throw std::runtime_error("Heap is empty");
        }

        if (min->degree > 0) {
            for (; min->degree > 0; min->degree--) {
                min->children->parent = nullptr;
                min->children = min->children->next;
            }

            min->children->previous->next = roots;
            roots->previous->next = min->children;
            std::swap(min->children->previous, roots->previous);

            degree += min->degree;
        }

        if (degree == 1) {
            delete min;

            roots = nullptr;
            min = nullptr;
            degree = 0;
        } else {
            if (roots == min) {
                roots = roots->next;
            }

            min->previous->next = min->next;
            min->next->previous = min->previous;
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
                        other->previous = other;
                        other->next = other;

                        current->children = other;
                    } else {
                        other->previous = current->children->previous;
                        other->next = current->children;

                        current->children->previous->next = other;
                        current->children->previous = other;
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
                        node->previous = node;
                        node->next = node;

                        roots = node;
                        min = node;
                    } else {
                        node->previous = roots->previous;
                        node->next = roots;

                        roots->previous->next = node;
                        roots->previous = node;

                        if (node->value < min->value) {
                            min = node;
                        }
                    }

                    degree++;
                }
            }
        }
    }

    void DecreaseKey(Node<T> *node, T &&value) {
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

            current->previous = roots->previous;
            current->next = roots;

            roots->previous->next = current;
            roots->previous = current;

            current->marked = false;
            degree++;
        }
    }
};
