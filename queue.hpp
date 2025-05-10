#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

// Базовый класс очереди
template <class T> 
class Queue
{ 
public: 
	virtual ~Queue () {} 
	virtual void enQueue(const T& e) = 0; // Добавление элемента в очередь
	virtual T deQueue() = 0; // Удаление элемента из очереди 
	virtual bool isEmpty() const = 0; // Проверка очереди на пустоту 
};



// Класс очереди на базе кольцевого массива
template <class T>
class QueueVector : public Queue<T>
{
public:
    // Конструктор по умолчанию
    QueueVector():
        _maxLen(5),
        _tail(0),
        _head(0)
    {
        _data = new T[_maxLen];
    }

    // Конструктор с параметром
    QueueVector(size_t maxSize):
        _maxLen(maxSize),
        _tail(0),
        _head(0)
    {
        _data = new T[_maxLen];
    }

    // Конструктор копирования
    QueueVector(const QueueVector<T>& _obj):
        _maxLen(_obj._maxLen),
        _tail(_obj._tail),
        _head(_obj._head)
    {
        _data = new T[_maxLen];

        for (size_t i = _tail; i <= _head; i++)
        {
            _data[i] = _obj._data[i];
        }
    }

    // Конструктор перемещения
    QueueVector(QueueVector<T>& _obj):
        _maxLen(_obj._maxLen),
        _tail(_obj._tail),
        _head(_obj._head),
        _data(_obj._data)
    {
        _obj._data = nullptr;
        _obj._head = 0;
        _obj._maxLen = 0;
        _obj._tail = 0;
    }

    // Деструктор
    ~QueueVector() override
    {
        delete[] _data;
        _tail = 0;
        _head = 0;
        _maxLen = 0;
    }

    // Перегруженные операторы
    T operator[] (size_t index); // Обращение к элементу по индексу
    QueueVector<T>& operator=(const QueueVector<T>& copy); // Оператор копирующего присваивания
    QueueVector<T>& operator=(QueueVector<T>&& moved); // Оператор перемещающего присваивания

    // Методы
    void enQueue(const T& e) override; // Добавление элемента в очередь
    T deQueue() override; // Удаление элемента из очереди
    bool isEmpty() const override; // Проверка очереди на пустоту
    void display() const; // Вывод всех элементов очереди
    
private:
    // Поля
    T *_data;
    size_t _maxLen;
    size_t _tail;
    size_t _head;

    // Методы
    void resize(); // Увеличение размера списка
};


// Описания методов

template <class T>
T QueueVector<T>::operator[](size_t index)
{
    if (index < _head || index >= _tail) throw std::logic_error("Cannot access non-existent elements");
    return _data[index];
}

template <class T>
QueueVector<T> &QueueVector<T>::operator=(const QueueVector<T> &copy)
{
    if (this != copy)
    {
        delete[] _data;
        _maxLen = copy._maxLen;
        _head = copy._head;
        _tail = copy._tail;
        _data = new T[_maxLen];

        for (size_t i = _head; i <= _tail; i++)
        {
            _data[i] = copy._data[i];
        }
    }
    return *this;
}

template <class T>
QueueVector<T> &QueueVector<T>::operator=(QueueVector<T> &&moved)
{
    if (this != moved)
    {
        delete[] _data;
        _maxLen = moved._maxLen;
        _head = moved._head;
        _tail = moved._tail;
        _data = moved._data;

        moved._data = nullptr;
        moved._head = 0;
        moved._maxLen = 0;
        moved._tail = 0;
    }
    return *this;
}

template <class T>
void QueueVector<T>::enQueue(const T &e)
{   
  if (_tail == _maxLen) resize();
  _data[_tail++] = e;
}

template <class T>
T QueueVector<T>::deQueue()
{
    if (isEmpty()) throw std::underflow_error("Queue Underflow");
    return _data[_head++];
}

template <class T>
bool QueueVector<T>::isEmpty() const
{
    return _head == _tail;
}

template <class T>
void QueueVector<T>::display() const
{
    if (isEmpty()) throw std::runtime_error("Queue is empty!");

    std::cout << "[ ";
    for (size_t i = _head; i < _tail; i++)
    {
        std::cout << _data[i] << " ";
    }
    std::cout << "]\n";
}

template <class T>
void QueueVector<T>::resize()
{   
    if (_tail != _maxLen) throw std::logic_error("No resize needed");

    size_t size = _tail - _head; // Получаем реальное кол-во элементов в очереди
    _maxLen = size * 2; // Увеличиваем максимальное кол-во элементов в 2 раза
    T *_buffer = new T[_maxLen]; // Создаём буффер
    
    // Переносим элементы из очереди в буффер
    for (size_t i = 0; i < size; i++)
    {
        _buffer[i] = deQueue();
    }

    // Устанавливаем новые _head и _tail
    _tail = size;
    _head = 0;
    
    // Освобождаем исходную очередь и переносим в неё элементы из новой
    delete[] _data;
    _data = _buffer;
    _buffer = nullptr;
}

#endif