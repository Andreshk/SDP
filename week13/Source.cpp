#include<fstream>
#include<iostream>
#include"Graph.h"

using namespace std;

Graph<int, string> buildIntGraph() {
    Graph<int, string> g;
    g.addEdge(142, 143);
    g.addEdge(143, 370);
    g.addEdge(143, 378);
    g.addEdge(378, 401);
    g.addEdge(143, 341);
    g.addEdge(143, 321);
    g.addEdge(341, 401);
    g.addEdge(321, 326);
    g.addEdge(326, 401);
    g.addEdge(326, 421);
    g.addEdge(321, 322);
    g.addEdge(322, 401);
    g.addEdge(322, 421);
    // uncomment to add a cycle to the graph
    // g.addEdge(326, 142);
    //ofstream intsDotty("intNodesNoCycle.dotty");
    //g.toDotty(intsDotty);
    return g;
}

Graph<string, string> buildClothesGraph() {
    Graph<string, string> clothes;
    // имаме ребро, ако първата дреха трябва да се облече преди втората
    clothes.addEdge("underwear", "leggings");
    clothes.addEdge("leggings", "socks");
    clothes.addEdge("leggings", "jeans");
    clothes.addEdge("T-shirt", "jeans");
    clothes.addEdge("underwear", "T-shirt");
    clothes.addEdge("T-shirt", "blouse");
    clothes.addEdge("blouse", "sweatshirt");
    clothes.addEdge("sweatshirt", "coat");
    clothes.addEdge("sweatshirt", "scarf");
    clothes.addEdge("scarf", "coat");
    clothes.addEdge("coat", "gloves");
    clothes.addVertex("hat");
    clothes.addEdge("jeans", "boots");
    clothes.addEdge("boots", "coat");
    //ofstream clothesDotty("clothes.dotty");
    //clothes.toDotty(clothesDotty);
    return clothes;

}

int main()
{
    Graph<int, string> g = buildIntGraph();
    vector<pair<int, string>> inEdges = g.incomingEdges(401);
    cout << "Incoming edges to vertex 401: ";
    for (const auto& edge : inEdges) {
        cout << edge.first << " ";
    }
    cout << endl;
    vector<int> sorted;
    if (g.topologicalSort(sorted)) {
        cout << "Topological sort: ";
        for (const auto& v : sorted) {
            cout << v << " ";
        }
        cout << endl;
    }
    else {
        cout << "No topological sort, has cycles :(" << endl;
    }

    cout << "------------------" << endl << "Clothes graph" << endl;
    Graph<string, string> clothes = buildClothesGraph();
    vector<string> clothesTopologicalSort;
    if (clothes.topologicalSort(clothesTopologicalSort)) {
        cout << "Topological sort: ";
        for (const auto& v : clothesTopologicalSort) {
            cout << v << " ";
        }
        cout << endl;
    }
    else {
        cout << "No topological sort, has cycles :(" << endl;
    }

    return 0;

/* lol, do not do this at home... or especially at work (!)
#define for_each(...) for (const auto& __VA_ARGS__)
    for_each(x : {1, 2, 3})
        std::cout << x << " ";
    std::cout << "\n";
*/
}
