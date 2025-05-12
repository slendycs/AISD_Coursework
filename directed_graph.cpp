#include "directed_graph.h"
#include "queue.hpp"
#include <limits>

bool DirectedGraph::isEmpty() const
{
    if (realSize_ == 0) return true;
    return false;
}

bool DirectedGraph::searchNode(size_t key) const
{
    try
    {
        if (adjacencyList_.at(key) != nullptr)
        {
            return true;
        }
        else return false;
    }
    catch(const std::out_of_range)
    {
        return false;
    }
}

bool DirectedGraph::hasVertex(size_t origin, size_t destination) const
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения


    // Получаем указатель на список ребер
    LinkedList<Vertex>* originVertexes = adjacencyList_.at(origin);

    // Ищем ребро в списке
    for (size_t i = 0; i < originVertexes->getSize(); i++)
    {
        Vertex* temp = originVertexes->at(i);
        if (temp->destination_ == destination) return true;
    }

    return false;
}

double DirectedGraph::removeVertex(size_t origin, size_t destination)
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения

    // Получаем указатель на список ребер
    LinkedList<Vertex>* originVertexes = adjacencyList_.at(origin);

    // Ищем ребро
    Vertex* temp = searchVertex(origin, destination);
    if (temp == nullptr) throw std::logic_error("Such a vertex does not exist");
    double weight = temp->weight_;

    // Удаляем ребро
    originVertexes->deleteItem(*temp);
    return weight;
}

ChainedHashTable<double> DirectedGraph::dijkstra(size_t origin) const
{   
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node does not exist"); // Проверка на существование исходного узла
    if (isOnlyPositiveVertexes() == false) throw std::logic_error("This graph contains vertexes with negative weights, which prevents Dijkstra's algorithm from running"); // Проверка что все рёюра положительные

    // Инициализация расстояний
    ChainedHashTable<double> distances(size_); // таблица узлов и расстояний
    QueueVector<size_t> processingQueue; // Очередь обхода узлов

    // Установка начальных значений
    for (size_t currentNode = 0; currentNode < size_; ++currentNode) 
    {
        if (searchNode(currentNode)) {
            distances.insert(currentNode, std::numeric_limits<double>::infinity());
        }
    }
    distances.insert(origin, 0.0);
    processingQueue.enQueue(origin);

    // Основной цикл обработки узлов
    while (!processingQueue.isEmpty()) 
    {
        size_t currentNode = processingQueue.deQueue();

        // Получение списка рёбер текущего узла
        LinkedList<Vertex>* adjacentVertex = adjacencyList_.at(currentNode);
        if (!adjacentVertex) continue;

        // Обход всех смежных узлов
        size_t edgesCount = adjacentVertex->getSize();
        for (size_t edgeIndex = 0; edgeIndex < edgesCount; ++edgeIndex) 
        {
            Vertex* currentEdge = adjacentVertex->at(edgeIndex);
            size_t neighborNode = currentEdge->destination_;
            double vertexWeight = currentEdge->weight_;

            // Вычисление нового расстояния
            double newDistance = distances[currentNode] + vertexWeight;

            // Обновление расстояния, если найден более короткий путь
            if (newDistance < distances[neighborNode]) {
                distances[neighborNode] = newDistance;
                processingQueue.enQueue(neighborNode);
            }
        }
    }
    distances.deleteItem(origin);
    return std::move(distances);
}

ChainedHashTable<double> DirectedGraph::bellmanFord(size_t origin) const
{   
    // Проверка на существование исходного узла
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node does not exist"); 

    // Сбор всех рёбер графа
    std::vector<std::tuple<size_t, size_t, double>> edges;
    for (size_t u = 0; u < adjacencyList_.size(); ++u) 
    {
        if (adjacencyList_[u] != nullptr) 
        {
            for (size_t i = 0; i < adjacencyList_[u]->getSize(); ++i) 
            {
                Vertex* vertex = adjacencyList_[u]->at(i);
                edges.emplace_back(u, vertex->destination_, vertex->weight_);
            }
        }
    }

    // Инициализация расстояний
    ChainedHashTable<double> distances(size_);
    for (size_t node = 0; node < size_; ++node) 
    {
        if (searchNode(node)) 
        {
            distances.insert(node, std::numeric_limits<double>::infinity());
        }
    }
    distances.insert(origin, 0.0);

    // Релаксация рёбер (n-1 итераций)
    for (size_t i = 1; i < realSize_; ++i) 
    {
        for (const auto& edge : edges) 
        {
            size_t u = std::get<0>(edge);
            size_t v = std::get<1>(edge);
            double weight = std::get<2>(edge);

            if (distances[u] != std::numeric_limits<double>::infinity() && distances[u] + weight < distances[v]) 
            {
                distances[v] = distances[u] + weight;
            }
        }
    }

    // Проверка на отрицательные циклы
    for (const auto& edge : edges) 
    {
        size_t u = std::get<0>(edge);
        size_t v = std::get<1>(edge);
        double weight = std::get<2>(edge);
        if (distances[u] != std::numeric_limits<double>::infinity() && distances[u] + weight < distances[v]) 
        {
            throw std::logic_error("Graph contains a negative-weight cycle");
        }
    }

    distances.deleteItem(origin);
    return distances;
}

double DirectedGraph::wave(size_t origin, size_t destination) const
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения

    if (origin == destination) return 0.0;

    QueueVector<size_t> queue;
    ChainedHashTable<size_t> parents(size_);
    ChainedHashTable<double> edgeWeights(size_);
    ChainedHashTable<bool> visited(size_);

    queue.enQueue(origin);
    parents.insert(origin, std::numeric_limits<size_t>::max());
    edgeWeights.insert(origin, 0.0);
    visited.insert(origin, true);

    bool found = false;

    while (!queue.isEmpty()) 
    {
        size_t current = queue.deQueue();

        if (current == destination) 
        {
            found = true;
            break;
        }

        LinkedList<Vertex>* adjacent = adjacencyList_.at(current);
        if (!adjacent) continue;

        for (size_t i = 0; i < adjacent->getSize(); ++i) 
        {
            Vertex* edge = adjacent->at(i);
            size_t neighbor = edge->destination_;

            if (!visited[neighbor]) 
            {
                visited.insert(neighbor, true);
                parents.insert(neighbor, current);
                edgeWeights.insert(neighbor, edge->weight_);
                queue.enQueue(neighbor);

                if (neighbor == destination) 
                {
                    found = true;
                }
            }
        }
    }

    if (!found) 
    {
        throw std::logic_error("No path exists between the nodes");
    }

    double totalWeight = 0.0;
    size_t current = destination;

    while (current != origin) 
    {
        size_t parent = parents[current];
        totalWeight += edgeWeights[current];
        current = parent;

        if (parent == std::numeric_limits<size_t>::max()) 
        {
            throw std::logic_error("Invalid path detected");
        }
    }

    return totalWeight;
}

void DirectedGraph::insertNode(size_t key)
{
    // Проверяем наличие узла в графе
    if (searchNode(key) == false)
    {
        // Проверяем, можно ли добавить новый узел
        if (key >= size_)
        {
            size_ = key + 1;
            adjacencyList_.resize(size_, nullptr);
        }

        // Добавляем узел
        adjacencyList_.at(key) = new LinkedList<Vertex>;
        realSize_++;
    }
    else throw std::runtime_error("This node already exists in the graph");
}

void DirectedGraph::removeNode(size_t key)
{
    // Проверяем наличие узла
    if (searchNode(key) == false) throw std::invalid_argument("This node is not in the graph");

    // Обходим весь список с узлами
    for (size_t i = 0; i < size_; i++)
    {   
        // Получаем список рёбер
        LinkedList<Vertex>* nodeVertexes = adjacencyList_.at(i);

        // Удаляем узел
        if (i == key)
        {
            if (nodeVertexes->getSize() != 0) nodeVertexes->clear();
            adjacencyList_.at(i) = nullptr;
            realSize_--;
        }
        
        // Удаляем ребро от i-того узла до key
        if ((adjacencyList_.at(i) != nullptr) && hasVertex(i, key))
        {
            removeVertex(i, key);
        }
    }
}

DirectedGraph::Vertex *DirectedGraph::searchVertex(size_t origin, size_t destination) const
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения


    // Получаем указатель на список ребер
    LinkedList<Vertex>* originVertexes = adjacencyList_.at(origin);

    // Ищем ребро в списке
    for (size_t i = 0; i < originVertexes->getSize(); i++)
    {
        Vertex* temp = originVertexes->at(i);
        if (temp->destination_ == destination) return temp;
    }

    return nullptr;
}

bool DirectedGraph::isOnlyPositiveVertexes() const
{
    for (const auto& node: adjacencyList_)
    {
        if (node == nullptr) continue;
        for (size_t i = 0; i < node->getSize(); i++)
        {
            Vertex* temp = node->at(i);
            if (temp->weight_ <= 0) return false;
        }
    }
    return true;
}

void DirectedGraph::addVertex(size_t origin, double weight, size_t destination)
{
    if (searchNode(origin) == false) throw std::invalid_argument("Origin node is not in the graph"); // Проверяем наличие узла источника
    if (searchNode(destination) == false) throw std::invalid_argument("Destination node is not in the graph"); // Проверяем наличие узла назначения


    // Проверяем не является ли новое ребро обратным
    if (hasVertex(destination, origin) == true) throw std::logic_error("There is already a vertex between these nodes");

    // Если между двумя нодами уже есть ребро, обновляем вес
    Vertex* temp = searchVertex(origin, destination);
    if (temp != nullptr)
    {
        temp->weight_ = weight;
        return;
    }

    // Если ребро ещё не встречалось, то добавляем его в список рёбер
    LinkedList<Vertex>* originVertexes = adjacencyList_.at(origin);
    originVertexes->pushBack(Vertex{weight, destination});
}