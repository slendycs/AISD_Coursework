#include <iostream>
#include "directed_graph.h"

int main()
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.insertNode(2);
    
    graph.addVertex(0, 3.0, 1);
    graph.addVertex(1, 4.0, 2);
    graph.addVertex(2, -8.0, 0); // Создаём цикл с отрицательным весом
    
    auto result = graph.bellmanFord(0);
    result.displayAllItems(std::cout);
}