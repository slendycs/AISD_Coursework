#include "../queue.hpp"
#include "../build/_deps/googletest-src/googletest/include/gtest/gtest.h"

TEST(queueTests, enQueue_test)
{
    QueueVector<int> queue;
    queue.enQueue(1);
    queue.enQueue(2);
    queue.enQueue(3);

    EXPECT_EQ(queue[0], 1);
    EXPECT_EQ(queue[1], 2);
    EXPECT_EQ(queue[2], 3);
}

TEST(queueTests, deQueue_test)
{
    QueueVector<int> queue;
    queue.enQueue(1);
    queue.enQueue(2);
    queue.enQueue(3);

    EXPECT_EQ(queue.deQueue(), 1);
    EXPECT_EQ(queue.deQueue(), 2);
    EXPECT_EQ(queue.deQueue(), 3);
}

TEST(queueTests, isEmpty_test)
{
    QueueVector<int> queue;
    EXPECT_TRUE(queue.isEmpty());
    queue.enQueue(1);
    EXPECT_FALSE(queue.isEmpty());
    queue.deQueue();
    EXPECT_TRUE(queue.isEmpty());
}

TEST(queueTests, resize_test)
{
    // Создайм очередь с максимальным кол-вом элементов 2
    QueueVector<int> queue(2);

    // Добавляем элементы
    queue.enQueue(1);
    queue.enQueue(2);
    queue.enQueue(3); // Происходит ресайз

    EXPECT_EQ(queue[0], 1);
    EXPECT_EQ(queue[1], 2);
    EXPECT_EQ(queue[2], 3);
}

TEST(queueTests, underflow_test)
{
    QueueVector<int> queue;
    queue.enQueue(1);
    queue.deQueue();
    EXPECT_THROW(queue.deQueue(), std::underflow_error);
}