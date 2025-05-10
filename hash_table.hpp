#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "linked_list.hpp"
#include <iostream>
#include <vector>
#include <ostream>

template <typename V>
class ChainedHashTable 
{
public:
    // Конструктор
    ChainedHashTable(size_t size) : size_(size) 
    {
        for (size_t i = 0; i < size_; i++) 
        {
            table_.push_back(new LinkedList<std::pair<size_t, V>>);
        }
    }

    // Конструктор перемещения
    ChainedHashTable(ChainedHashTable&& other) noexcept: 
        size_(other.size_),
        table_(std::move(other.table_)) 
    {
        other.size_ = 0;
        other.table_.clear();
    }

    // Оператор перемещающего присваивания
    ChainedHashTable& operator=(ChainedHashTable&& other) noexcept 
    {
        if (this != &other) 
        {
            for (auto* bucket : table_) delete bucket;
            table_.clear();
            
            size_ = other.size_;
            table_ = std::move(other.table_);
            
            other.size_ = 0;
            other.table_.clear();
        }
        return *this;
    }

    // Деструктор
    ~ChainedHashTable() 
    {
        for (auto* bucket : table_) 
        {
            delete bucket;
        }
        table_.clear();
    }

    // Запрет копирования и перемещения
    ChainedHashTable(const ChainedHashTable&) = delete;
    ChainedHashTable& operator=(const ChainedHashTable&) = delete;

    // Методы
    void insert(size_t key, V value);
    void deleteItem(size_t key);
    void displaySameHashItems(std::ostream& out, size_t hash) const;
    void displayAllItems(std::ostream& out);

    // Оператор [] для доступа по ключу
    V& operator[](size_t key) 
    {
        size_t hash = getHash(key);
        auto* bucket = table_.at(hash);
        
        for (size_t i = 0; i < bucket->getSize(); ++i) 
        {
            auto* pair = bucket->at(i);
            if (pair->first == key) 
            {
                return pair->second;
            }
        }
        
        // Вставка нового элемента
        insert(key, V());
        return bucket->at(bucket->getSize() - 1)->second;
    }

private:
    size_t size_; // Кол-во хешей в таблице
    std::vector<LinkedList<std::pair<size_t, V>>*> table_; // Таблица с бакетами
    size_t getHash(size_t key) const; // Хеш функция
};

// Реализации методов
template <typename V>
void ChainedHashTable<V>::insert(size_t key, V value) 
{
    size_t hash = getHash(key);
    auto* bucket = table_.at(hash);
    
    for (size_t i = 0; i < bucket->getSize(); ++i) 
    {
        auto* pair = bucket->at(i);
        if (pair->first == key) 
        {
            pair->second = value;
            return;
        }
    }
    
    bucket->pushBack(std::make_pair(key, value));
}

template <typename V>
void ChainedHashTable<V>::deleteItem(size_t key) 
{
    size_t hash = getHash(key);
    auto* bucket = table_.at(hash);
    
    for (size_t i = 0; i < bucket->getSize(); ++i) 
    {
        auto* pair = bucket->at(i);
        if (pair->first == key) 
        {
            bucket->deleteItem(*pair);
            return;
        }
    }
    
    throw std::logic_error("Key not found");
}

template <typename V>
void ChainedHashTable<V>::displaySameHashItems(std::ostream& out, size_t hash) const 
{
    if (hash >= size_) throw std::range_error("Invalid hash");
    
    auto* bucket = table_.at(hash);
    for (size_t i = 0; i < bucket->getSize(); ++i) 
    {
        auto* pair = bucket->at(i);
        out << "(" << pair->first << ": " << pair->second << ") ";
    }
    if (bucket->getSize() != 0) out << "\n";
}

template <typename V>
void ChainedHashTable<V>::displayAllItems(std::ostream& out) 
{
    for (size_t hash = 0; hash < size_; ++hash) 
    {
        displaySameHashItems(out, hash);
    }
}

template <typename V>
size_t ChainedHashTable<V>::getHash(size_t key) const
{
    return key % size_; // Хеширование делением
}

#endif