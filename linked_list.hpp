#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <ostream>

template<class T>
class LinkedList
{
public:
    // Конструктор по умолчанию
    LinkedList():
        head(nullptr),
        tail(nullptr)
    {}

    // Конструктор копирования
    LinkedList(const LinkedList<T> &obj):
        head(nullptr),
        tail(nullptr)
    {
        Node* current = obj.head;
        while(current)
        {
            Node* nextNode = current->next;
            this->pushBack(current->val);
            current = nextNode;
        }
    }
    
    // Конструктор перемещения
    LinkedList(LinkedList<T> &&moved):
        head(moved.head),
        tail(moved.tail)
    {
        moved.tail = nullptr;
        moved.head = nullptr;
    }

    // Деструктор
    ~LinkedList()
    {
        Node* current = head;

        while (current)
        {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // Оператор перемещающего присваивания
    LinkedList<T>& operator=(LinkedList<T>&& moved)
    {
        if (this != &moved)
        {
            // Освобождаем текущие ресурсы
            clear();

            // Переносим ресурсы из moved
            head = moved.head;
            tail = moved.tail;

            // Обнуляем move
            moved.head = nullptr;
            moved.tail = nullptr;
        }
        return *this;
    }

    // Методы
    bool isEmpty() const; // Проверяет наличие узлов в списке 
    bool searchItem(const T &_val) const; // Прверяет наличие элемента в списке
    void pushBack(const T & _val); // Добавление узла в конец списка
    void print(std::ostream& out) const; // Вывод элементов списка в поток
    void clear(); // Очистка списка
    void deleteItem(const T&_val); // Удаление элемента из словаря
    size_t getSize(); // Получение количества элементов в списке
    T* at(size_t index); // Получение указателя на значение

    // Операторы
    T operator[] (size_t index) // Обращение к элементу по индексу
    {   
        size_t currentIndex = 0;
        Node* currentNode = head;

        while (currentNode != nullptr)
        {
            if (currentIndex == index)
            {
                return currentNode->val;
            }

            currentIndex++;
            currentNode = currentNode->next;
        }
        throw std::logic_error("There is no element with this index");
    }


private:
    struct Node
    {
        T val; // Поле данных
        Node *next; // Указатель на следующий узел
        Node *prev; // Указатель на предыдущий узел

        // Конструктор
        Node(T _val):
            val(_val),
            next(nullptr),
            prev(nullptr)
        {}
    };

    Node* head; // Указывает на первый узел списка
    Node* tail; // Указывает на последний узел списка
};




// Реализация методов

template <class T>
bool LinkedList<T>::isEmpty() const
{
    return head == nullptr;
}

template <class T>
bool LinkedList<T>::searchItem(const T &_val) const
{
    Node* current = head;
    while (current)
    {
        if (current->val == _val)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <class T>
void LinkedList<T>::pushBack(const T &_val)
{
    Node* newNode = new Node(_val);
    newNode->prev = tail;
    if (tail != nullptr)
    {
        tail->next = newNode;
    }
    if (head == nullptr)
    {
        head = newNode;
    }
    tail = newNode;
}


template <class T>
void LinkedList<T>::print(std::ostream &out) const
{
    if (isEmpty() == true) throw std::logic_error("Linked List is empty!");

    Node* current = head;

    while (current)
    {
        out << current->val << " ";
        current = current->next;
    }
}

template <class T>
void LinkedList<T>::clear()
{   
    if (isEmpty() == true) throw std::logic_error("Linked List is already empty!");

    Node* current = head;

    while (current)
    {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
}

template <class T>
void LinkedList<T>::deleteItem(const T &_val)
{
    if (searchItem(_val) == false) throw std::logic_error("This element is not in the Linked List");

    Node* current = head;
    while (current) {
        if (current->val == _val) 
        {
            Node* prevNode = current->prev;
            Node* nextNode = current->next;
            
            if (prevNode) 
            {
                prevNode->next = nextNode;
            } 
            else 
            {
                head = nextNode;
            }

            if (nextNode) 
            {
                nextNode->prev = prevNode;
            } 
            else 
            {
                tail = prevNode;
            }

            delete current;
            return;
        }
        current = current->next;
    }
}

template <class T>
size_t LinkedList<T>::getSize()
{
    if (isEmpty()) return 0;
    
    size_t count = 0;
    Node* currentNode = head;

    while (currentNode != nullptr)
    {
        count++;
        currentNode = currentNode->next;
    }

    return count;
}

template <class T>
T* LinkedList<T>::at(size_t index)
{
    size_t currentIndex = 0;
    Node* currentNode = head;

    while (currentNode != nullptr)
    {
        if (currentIndex == index)
        {
            return &(currentNode->val);
        }
        currentIndex++;
        currentNode = currentNode->next;
    }
    throw std::out_of_range("Index out of bounds");
}

#endif