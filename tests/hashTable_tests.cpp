#include "../hash_table.hpp"
#include "../build/_deps/googletest-src/googletest/include/gtest/gtest.h"

TEST(ChainedHashTableTest, InsertAndRetrieve) 
{
    ChainedHashTable<std::string> table(10);
    
    // Вставка и проверка
    table.insert(42, "Answer");
    ASSERT_EQ(table[42], "Answer");
    
    // Обновление значения
    table.insert(42, "Updated");
    ASSERT_EQ(table[42], "Updated");
}

TEST(ChainedHashTableTest, OperatorAccess) 
{
    ChainedHashTable<int> table(5);
    
    // Проверка создания нового элемента
    table[100] = 500;
    ASSERT_EQ(table[100], 500);
    
    // Проверка возврата ссылки
    table[200] = table[100] + 100;
    ASSERT_EQ(table[200], 600);
}

TEST(ChainedHashTableTest, DeleteItem) 
{
    ChainedHashTable<double> table(3);
    table.insert(1, 3.14);
    
    // Удаление существующего элемента
    table.deleteItem(1);
    std::stringstream ss;
    table.displayAllItems(ss);
    EXPECT_EQ(ss.str(), "");
    
    // Попытка удалить несуществующий ключ
    EXPECT_THROW(table.deleteItem(999), std::logic_error);
}

TEST(ChainedHashTableTest, HashCollision) 
{
    ChainedHashTable<char> table(2); // Все ключи с четными/нечетными хешами
    
    // Два ключа с одинаковым хешем
    table.insert(2, 'A');
    table.insert(4, 'B');
    
    ASSERT_EQ(table[2], 'A');
    ASSERT_EQ(table[4], 'B');
}

TEST(ChainedHashTableTest, DisplayMethods) 
{
    ChainedHashTable<int> table(3);
    table.insert(0, 10);
    table.insert(1, 20);
    table.insert(3, 30); // Хеш = 3 % 3 = 0
    
    std::stringstream ss;
    
    // Проверка вывода бакета с коллизией
    table.displaySameHashItems(ss, 0);
    ASSERT_EQ(ss.str(), "(0: 10) (3: 30) \n");
    
    ss.str("");
    table.displayAllItems(ss);
    std::string expected = "(0: 10) (3: 30) \n(1: 20) \n";
    ASSERT_EQ(ss.str(), expected);
}

TEST(ChainedHashTableTest, EdgeCases) 
{
    ChainedHashTable<std::string> table(1); // Все элементы в одном бакете
    
    table.insert(0, "Zero");
    table.insert(1, "One"); // 1 % 1 = 0
    ASSERT_EQ(table[0], "Zero");
    ASSERT_EQ(table[1], "One");
    
    table.deleteItem(0);
    EXPECT_THROW(table.deleteItem(0), std::logic_error);
}

TEST(ChainedHashTableTest, ExceptionSafety) 
{
    ChainedHashTable<int> table(5);
    
    // Попытка доступа к невалидному хешу
    EXPECT_THROW(table.displaySameHashItems(std::cout, 10), std::range_error);
}