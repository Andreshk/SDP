#ifndef __GRAPH__
#define __GRAPH__

#include <map>
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <vector>
#include <ostream>

using namespace std;

// поради някаква причина при мен липсва, ако ви прави проблем закоментирайте го
std::ostream& operator<<(std::ostream& os, const std::string& str) { return os << str.data(); }

template <class VT, class LT>
class Graph
{
public:

    void addVertex(const VT&);
    void addEdge(const VT&, const VT&, const LT& label = LT());

    vector<pair<VT, LT>> outgoingEdges(const VT&) const;
    vector<pair<VT, LT>> incomingEdges(const VT&) const;

    bool topologicalSort(vector<VT>&) const;

    void toDotty(ostream&) const;
private:

    unordered_map<VT, vector<pair<VT, LT>>> edges;

};

template <class VT, class LT>
void Graph<VT, LT>::addVertex(const VT &newVertex)
{
    edges[newVertex];
}

template <class VT, class LT>
void Graph<VT, LT>::addEdge(const VT &outV, const VT &inV, const LT &lbl)
{
    addVertex(outV);
    addVertex(inV);

    edges[outV].push_back(pair<VT, LT>(inV, lbl));
}

template <class VT, class LT>
void Graph<VT, LT>::toDotty(ostream &out) const
{
    /*
    digraph G
    {
    1->2[label="hello"];
    3->4;
    1;
    }
    */
    out << "digraph G\n{\n";

    //for (const pair<VT, vector<pair<VT, LT>>>& mapEl : edges)
    for (const auto& mapEl : edges)
    {
        out << "\t\"" << mapEl.first << "\";\n";

        for (const pair<VT, LT> &edge : mapEl.second)
        for (const auto& edge : mapEl.second)
        {
            out << "\t\""
                << mapEl.first
                << "\" -> \""
                << edge.first
                << "\"[label=\""
                << edge.second
                << "\"];\n";
        }
    }

    out << "}\n";

}

template <class VT, class LT>
vector<pair<VT, LT>> Graph<VT, LT>::outgoingEdges(const VT &out) const
{
    return edges.at(out);
}

template <class VT, class LT>
vector<pair<VT, LT>> Graph<VT, LT>::incomingEdges(const VT &out) const
{
    vector<pair<VT, LT>> result;
    for (const auto& vEdges : edges) {
        for (const auto& edge : vEdges.second) {
            if (edge.first == out) {
                result.push_back(pair<VT, LT>(vEdges.first, edge.second));
            }
        }
    }
    return result;
}

template <class VT, class LT>
bool Graph<VT, LT>::topologicalSort(vector<VT>& sorted) const {
    // опашка, в която ще слагаме подред всеки връх, който остане без входящи ребра
    queue<VT> freeVertices;

    // map, в който ще държим броя входящи ребра за всеки връх
    // когато този брой стане 0, можем да сложим върха в опашката от "свободни" върхове
    map<VT, int> incomingEdgesCounts;

    // инициализираме incomingEdgesCounts с първоначалните бройки от графа
    for (const auto& vEdges : edges) {
        const VT& vertex = vEdges.first;
        const vector<pair<VT, LT>>& neighbours = vEdges.second;
        // инициализираме incomingEdgesCount[vertex] на 0, ако все още няма стойност
        incomingEdgesCounts[vertex];
        for (const auto& edge : neighbours) {
            incomingEdgesCounts[edge.first] += 1;
        }
    }

    // вкарваме в опашката първоначалните "свободни" върхове от графа,
    // т.е. тези без входящи ребра
    for (const auto& edgeCount : incomingEdgesCounts) {
        if (edgeCount.second == 0) {
            freeVertices.push(edgeCount.first);
        }
    }
    while (!freeVertices.empty()) {
        // за всеки пореден връх в опашката от свободни върхове
        // вкарваме го в резултатната сортирана редица sorted,
        // след което го "махаме" от графа, като "изтриваме" всички ребра, излизащи от него
        // това правим на практика като намалим бройката на входящите ребра на съседите му
        VT currentV = freeVertices.front();
        freeVertices.pop();
        sorted.push_back(currentV);

        for (const auto& edge : edges.at(currentV)) {
            // reduce number of incoming edges
            // nodes can go into the sorted list only after this counter reaches 0
            incomingEdgesCounts[edge.first] -= 1;
            if (incomingEdgesCounts[edge.first] == 0) {
                freeVertices.push(edge.first);
            }
        }
    }

    // топологическото сортиране е успешно, т.е. обхванало е целият граф, ако в края на алгоритъма
    // т.е. когато се изпразни опашката от freeVertices
    // всички ребра са махнати от графа, т.е. всеки връх има 0 входящи ребра
    for (const auto& edgeCount : incomingEdgesCounts) {
        if (edgeCount.second != 0) {
            // има останало ребро, значи е имало цикъл и топологическото сортиране е невъзможно
            return false;
        }
    }
    // всичко е наред, няма останали ребра и в sorted сме подредили в правилния ред върховете на графа
    return true;
}

#endif

