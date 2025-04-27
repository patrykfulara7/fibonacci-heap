#include <gtest/gtest.h>
#include <stdexcept>

#include "../FibonacciHeap.hxx"

TEST(FibonacciHeap, Empty) {
    FibonacciHeap<int> heap;

    EXPECT_TRUE(heap.Empty());

    heap.Insert(1);
    EXPECT_FALSE(heap.Empty());
}

TEST(FibonacciHeap, FindMin) {
    FibonacciHeap<int> heap;

    EXPECT_THROW(heap.FindMin(), std::runtime_error);

    heap.Insert(3);
    EXPECT_EQ(heap.FindMin(), 3);

    heap.Insert(1);
    EXPECT_EQ(heap.FindMin(), 1);

    heap.Insert(2);
    EXPECT_EQ(heap.FindMin(), 1);
}

TEST(FibonacciHeap, Insert) {
    FibonacciHeap<int> heap;

    heap.Insert(3);
    heap.Insert(1);
    heap.Insert(2);
    EXPECT_EQ(heap.FindMin(), 1);
}

TEST(FibonacciHeap, DeleteMin) {
    FibonacciHeap<int> heap;

    EXPECT_THROW(heap.DeleteMin(), std::runtime_error);

    heap.Insert(3);
    heap.Insert(4);
    heap.Insert(1);
    heap.Insert(6);
    heap.Insert(5);
    heap.Insert(2);
    heap.DeleteMin();
    EXPECT_EQ(2, heap.FindMin());
}

TEST(FibonacciHeap, DecreaseKey) {
    FibonacciHeap<int> heap;

    std::vector<Node<int> *> nodes;
    for (std::size_t i = 1; i <= 8; i++) {
        nodes.push_back(heap.Insert(i));
    }

    heap.DeleteMin();
    heap.DecreaseKey(nodes[5], 3);
    EXPECT_EQ(heap.FindMin(), 2);

    heap.DecreaseKey(nodes[6], 1);
    EXPECT_EQ(heap.FindMin(), 1);
}
