#include <gtest/gtest.h>

#include "../FibonacciHeap.hxx"

TEST(FibonacciHeap, Empty) {
    FibonacciHeap<int> heap;

    EXPECT_TRUE(heap.Empty());

    heap.Push(1);
    EXPECT_FALSE(heap.Empty());
}

TEST(FibonacciHeap, Top) {
    FibonacciHeap<int> heap;

    heap.Push(3);
    EXPECT_EQ(heap.Top(), 3);

    heap.Push(1);
    EXPECT_EQ(heap.Top(), 1);

    heap.Push(2);
    EXPECT_EQ(heap.Top(), 1);
}

TEST(FibonacciHeap, Push) {
    FibonacciHeap<int> heap;

    heap.Push(3);
    heap.Push(1);
    heap.Push(2);
    EXPECT_EQ(heap.Top(), 1);
}

TEST(FibonacciHeap, Pop) {
    FibonacciHeap<int> heap;

    heap.Push(3);
    heap.Push(4);
    heap.Push(1);
    heap.Push(6);
    heap.Push(5);
    heap.Push(2);
    heap.Pop();
    EXPECT_EQ(heap.Top(), 2);
}

TEST(FibonacciHeap, Update) {
    FibonacciHeap<int> heap;

    using NodeIterator = FibonacciHeap<int>::NodeIterator;
    std::vector<NodeIterator> nodes;
    for (std::size_t i = 1; i <= 8; i++) {
        nodes.push_back(heap.Push(i));
    }

    heap.Pop();
    std::cout << reinterpret_cast<void *>(&*nodes[0]) << std::endl;
    heap.Update(nodes[5], 3);
    EXPECT_EQ(heap.Top(), 2);

    heap.Update(nodes[6], 1);
    EXPECT_EQ(heap.Top(), 1);
}
