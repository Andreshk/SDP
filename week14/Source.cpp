#include<iostream>
#include<unordered_map>
#include<vector>
#include<stack>
#include<queue>

// за удобство - закоментирайте ако възниква конфликт
std::ostream& operator<<(std::ostream& os, const std::string& s) { return os << '"' << s.data() << '"'; }

template<class V>
class Graph
{
    // върховете в реалния граф ще бъдат числа, и по-точно поредни,
    // започващи от 0,1,... Тогава вместо map от върхове към
    // списъка със съседите можем да използваме просто масив
    // от такива списъци (в случая, пак динамични масиви).
    // В такъв случай няма нужда да се притесняваме да "копираме"
    // върхове наляво-надясно - това е просто копиране на числа (безплатно)
    std::vector<std::vector<size_t>> neighbours;

    // Ако искаме да поддържаме графи с произволни типове по върховете,
    // можем за удобство да правим двупосочно съпоставяне връх<->пореден номер (индекс)
    // и за самите алгоритми да работим с граф с числа при върховете, и само
    // за приемане/връщане на стойности да разботим с тези стойности от произв.тип.
    // В едната посока, тъй като работим с произволни типове, трябва да ползваме
    // std::map или std::unordered_map. В другата посока, тъй като имаме числа от 0,1,...,
    // отново можем да използваме обикновен (макар и динамичен) масив.
    std::unordered_map<V, size_t> indexes;
    std::vector<V> vertices;

public:
    // добавяне на връх - добавя връх със съответния пореден номер (нямаме контрол над номерата)
    void addVertex(const V& v)
    {
        // ако върхът съществува вече
        if (indexes.find(v) != indexes.end())
            return;
        // добавяме го в индекса с поредния му номер
        indexes[v] = vertices.size();
        // съответно запомняме и на кой номер съответства и този връх
        vertices.push_back(v);
        // най-накрая, инициираме и празен списък на съседство за него
        neighbours.push_back({});
    }

    void addEdge(const V& from, const V& to)
    {
        size_t fromIdx = indexes[from], toIdx = indexes[to];
        neighbours[fromIdx].push_back(toIdx);
    }

    void addBidirectionalEdge(const V& from, const V& to)
    {
        // за удобство пробваме първо да добавим върховете <=> жертваме скорост!
        addVertex(from);
        addVertex(to);
        addEdge(from, to);
        addEdge(to, from);
    }

    // аналогични на другото представяне
    std::vector<V> outgoingEdges(const V& v)
    {
        // от индекса на този връх получаваме списък от индекси
        // -> трансформираме го до списък с реални върхове, използвайки vertices
        std::vector<size_t> temp = neighbours.at(indexes[v]);
        std::vector<V> result;
        for (size_t i : temp)
            result.push_back(vertices[i]);
        return result;
    }
    std::vector<V> incomingEdges(const V& v)
    {
        std::vector<V> result;
        size_t vIdx = indexes[v];
        for (size_t u = 0; u < neighbours.size();u++)
            for (const auto& w : neighbours[u])
                if (w == vIdx) // ако v (по-точно индекса на v) присъства в списъка на съседство на някое u -> добавяме върха на u
                    result.push_back(vertices[u]);
        return result;
    }

    size_t vertexCount() const { return neighbours.size(); }

    void BFS(const V& start)
    {
        std::vector<bool> visited(vertexCount(), false);
        std::queue<size_t> fringe;
        size_t startIdx = indexes[start];
        fringe.push(startIdx);
        visited[startIdx] = true;
        std::cout << "Starting BFS from vertex " << start << ":";
        while (!fringe.empty())
        {
            size_t curr = fringe.front();
            fringe.pop();
            std::cout << " " << vertices[curr];
            for(size_t v : neighbours[curr])
                if (!visited[v])
                {
                    visited[v] = true;
                    fringe.push(v);
                }
        }
        std::cout << "\n\n";
    }

    void DFS(const V& start)
    {
        std::vector<bool> visited(vertexCount(), false);
        std::stack<size_t> fringe;
        size_t startIdx = indexes[start];
        fringe.push(startIdx);
        std::cout << "Starting DFS from vertex " << start << ":";
        while (!fringe.empty())
        {
            size_t curr = fringe.top();
            fringe.pop();
            if (!visited[curr])
            {
                std::cout << " " << vertices[curr];
                for (size_t v : neighbours[curr])
                    if (!visited[v])
                    {
                        fringe.push(v);
                    }
            }
            visited[curr] = true;
        }
        std::cout << "\n\n";
    }
};

int main()
{
    Graph<std::string> g;
    // след като добавянето на ребра предварително пробва
    // да добави върховете, нямаме нужда от това
    /*g.addVertex("abc");
    g.addVertex("def");
    g.addVertex("banichka");
    g.addVertex("neshtosi");
    g.addVertex("xyz");*/
    g.addBidirectionalEdge("abc", "def");
    g.addBidirectionalEdge("abc", "banichka");
    g.addBidirectionalEdge("banichka", "neshtosi");
    g.addBidirectionalEdge("banichka", "xyz");
    g.addBidirectionalEdge("def", "xyz");
    g.addBidirectionalEdge("neshtosi", "xyz");

    g.BFS("abc");
    g.DFS("abc");
}
