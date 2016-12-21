#include<iostream>
#include<cstddef>
#include<string>

// Хешираща функция, подходяща за rolling хеширане:
// за стринг "abcd" изчислява 2^3 * 'a' + 2^2 * 'b' + 2 * 'c' + 'd'
uint64_t rollingHash(const char* bytes, size_t size)
{
    uint64_t result = 0;
    for (size_t i = 0; i < size; i++)
    {
        result <<= 1;
        result += bytes[i];
    }
    return result;
}

// Търсене на низ в низ - искаме да намерим на колко (и евентуално на кои)
// позиции pattern се среща точно в text, използвайки техниката на rolling hash.
// Сложност: О(n+m), където n=|text|; m=|pattern|.
// ВАЖНО: коректността зависи от хеширащата функция!
// Третият аргумент указва дали искаме да ни принтира "информация" за търсенето
// или само да връща бройката намерени съвпадения.
size_t findMatches(const std::string& pattern, const std::string& text, bool fullPrint = true)
{
    const size_t m = pattern.size(), n = text.size();
    // oops
    if (m > n)
        return 0;
    // генерираме хеша на pattern-а, който търсим
    const uint64_t patternHash = rollingHash(pattern.data(), m);
    // инициализираме "търкалящия" се хеш по първите m символа в text
    uint64_t tmpHash = rollingHash(text.data(), m);
    if (fullPrint)
    {
        std::cout << "pattern: " << pattern << "\n";
        std::cout << "text: " << text << "\n";
    }
    size_t matches = 0;
    // може още в началото да сме намерили съвпадение
    if (patternHash == tmpHash)
    {
        ++matches;
        if (fullPrint) std::cout << " pos: 0\n";
    }
    // за всяка следваща позиция актуализираме tmpHash за константно време (!)
    for (size_t i = 1; i <= n - m; i++)
    {
        // за да знаем как да променим tmpHash, трябва да знаем как работи хеширащата функция
        tmpHash -= uint64_t(text[i - 1]) << (m - 1); // <=> text[i-1] * (1 << (m-1))
        tmpHash <<= 1; // <=> tmpHash *= 2;
        tmpHash += text[i + m - 1];
        if (patternHash == tmpHash)
        {
            if (fullPrint) std::cout << " pos: " << i << "\n";
            ++matches;
        }
    }
    if (fullPrint) std::cout << "\n";
    return matches;
}

// Задачка - търси се най-дългият подстринг на str, който се среща в него поне K пъти.
// Идея: тъй като се вълнуваме от дължина, ще генерираме всички подстрингове подред по дължини
// и ако намерим някой с повече от K съвпадения, по-дълъг от досега намерения, записваме го.
// По-нататък можем да развием тази идея като приложим двоично търсене над интервала от дължини,
// за да намерим по-бързо тази най-голяма дължина. Естествено, възможно е и да няма такъв стринг.
void zadachka(const std::string& str, size_t K)
{
    std::string result;
    const size_t n = str.size();
    for (size_t len = 1; len <= n; len++)
        for (size_t idx = 0; idx <= n - len; idx++)
        {
            std::string tmp(str.begin() + idx, str.begin() + idx + len);
            //За да се подсигурим, че генерираме правилно всички подстрингове
            //std::cout << "\"" << tmp << "\"\n";
            size_t matches = findMatches(tmp, str, false);
            if (matches >= K && tmp.size() > result.size())
                result = tmp;
        }
    std::cout << "> str = \"" << str << "\", K = " << K << ": ";
    if (result.empty())
        std::cout << "No such substring!\n";
    else
        std::cout << "\"" << result << "\", length = " << str.size() << "\n";
}

void testZadachka()
{
    std::string str;
    size_t K;
    while (true)
    {
        std::getline(std::cin, str);
        std::cin >> K;
        // Ако след четене на число следва четене на стринг
        // -> .ignore() на потока, от който четем (дали std::cin или някой файл)
        std::cin.ignore();
        if (K == 0)
            break;
        // отново: за да сме сигурни, че сме прочели това, което искаме
        //std::cout << ">  \"" << str << "\", " << K << "\n";
        zadachka(str, K);
        std::cout << "\n";
    }
}

int main()
{
    //findMatches("abc", "abcdefneshtosiabcstupid");
    //findMatches("abc", "abca");
    //zadachka("ababab", 4);
    
    std::cout << "Enter K=0 to break...\n";
    testZadachka();

    // Странен пример, илюстриращ колко ненадеждна е хеширащата ни функция
    std::string strange = "dsighfOGUGHNUOdfKFLDSIJGOIEJHOALKGFNZFKLs";
    findMatches("DSI", strange);
    std::cout << "hash(\"HOA\") = " << rollingHash("HOA", 3) << "\n";
    std::cout << "hash(\"DSI\") = " << rollingHash("DSI", 3) << "\n";
    std::cout << "hash(\"IJG\") = " << rollingHash("IJG", 3) << "\n";
}
