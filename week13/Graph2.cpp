#include <vector>
#include <cstddef> // size_t

class Graph2
{
    // върховете в този граф ще бъдат числа, и по-точно поредни,
    // започващи от 0,1,... Тогава вместо map от върхове към
    // списъка със съседите можем да използваме просто масив
    // от такива списъци (в случая, пак динамични масиви).
    // В такъв случай няма нужда да се притесняваме да "копираме"
    // върхове наляво-надясно - това е просто копиране на числа (безплатно)
    std::vector<std::vector<size_t>> neighbours;

public:
    // конструкторът създава count на брой върха, засега с никакви съседи помежду си
    Graph2(size_t vertexCount = 1) : neighbours(vertexCount, {}) {}

    // добавяне на връх - добавя връх със съответния пореден номер (нямаме контрол над номерата)
    void addAnotherVertex() { neighbours.push_back({}); }

    // просто
    void addEdge(size_t from, size_t to) { neighbours[from].push_back(to); }

    // аналогични на другото представяне
    std::vector<size_t> outgoingEdges(size_t v) { return neighbours.at(v); }
    std::vector<size_t> incomingEdges(size_t v)
    {
        std::vector<size_t> result;
        for (size_t u = 0; u < neighbours.size();u++)
            for (const auto& w : neighbours[u])
                if (w == v) // ако v присъства в списъка на съседство на някое u -> добавяме го
                    result.push_back(u);
        return result;
    }
};
