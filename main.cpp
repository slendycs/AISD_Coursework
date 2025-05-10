#include <iostream>
#include "directed_graph.h"

int main()
{
    DirectedGraph graph;
    graph.insertNode(0);
    graph.insertNode(1);
    graph.addVertex(0, 5.0, 1);
    graph.insertNode(2);
    graph.addVertex(0, 2, 2);
    graph.addVertex(2, 1, 1);
    
    auto shortestPaths = graph.dijkstra(0);
    shortestPaths.displayAllItems(std::cout);
}