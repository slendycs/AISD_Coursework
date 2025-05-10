#include "../linked_list.hpp"
#include "../build/_deps/googletest-src/googletest/include/gtest/gtest.h"
#include <sstream>

TEST(ListTests, test_insert_item)
{
    LinkedList<int> list;
    list.pushBack(1);
    list.pushBack(3);
    list.pushBack(2);

    std::stringstream ss;
    list.print(ss);
    EXPECT_EQ(ss.str(), "1 3 2 ");
}

TEST(ListTests, test_search_item)
{
    LinkedList<int> list;
    list.pushBack(1);
    list.pushBack(3);
    list.pushBack(5);
    list.pushBack(4);

    EXPECT_TRUE(list.searchItem(5));
}

TEST(ListTests, test_clear)
{
    LinkedList<int> list;
    list.pushBack(1);
    list.pushBack(3);
    list.pushBack(5);
    list.pushBack(4);
    list.clear();

    EXPECT_TRUE(list.isEmpty());
}

TEST(ListTests, test_delete_item)
{
    LinkedList<int> list;
    list.pushBack(1);
    list.pushBack(3);
    list.pushBack(5);
    list.pushBack(4);

    list.deleteItem(3);
    EXPECT_FALSE(list.searchItem(3));
}

TEST(ListTests, test_index_access)
{
    LinkedList<int> list;
    list.pushBack(1);
    list.pushBack(3);
    list.pushBack(5);
    list.pushBack(4);

    EXPECT_EQ(list[0], 1);
    EXPECT_EQ(list[2], 5);
    EXPECT_EQ(list[3], 4);
    EXPECT_THROW(list[4], std::logic_error);
}

TEST(ListTests, test_getSize)
{
    LinkedList<int> list;
    list.pushBack(1);
    list.pushBack(3);
    list.pushBack(5);
    list.pushBack(4);

    EXPECT_EQ(list.getSize(), 4);
}