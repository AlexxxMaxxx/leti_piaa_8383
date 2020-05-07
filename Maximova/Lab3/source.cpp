#include <algorithm>
#include <iostream>
#include <iomanip> 
#include <fstream>
#include <vector>
#define SIZE 26

std::ifstream infile("test.txt");

class Check {                               //для проверки корректности введенных данных
protected:
    int checkInt() {                        //ожидается положительное число
        int a = 0;

        while ((!(std::cin >> a)) || a <= 0) {
            std::cout << "Ошибка. Данные некорректны! Ожидается натуральное число.\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            fflush(stdin);
        }
        std::cin.get();
        return a;
    }

    bool checkChar(char a, char b) {        //если исток равен стоку, то запуск алгоритма не имеет смысла
        if (a == b) {
            std::cout << "Значения истока и стока совпадают.\n";
            return false;
        }
        return true;
    }

};

class Edge {
private:
    char vertSt;
    char vertFn;
    int capacity;
    int flow;

public:
    Edge(char st, char fn, int cap) : vertSt(st), vertFn(fn), capacity(cap), flow(0) {}
    void setFlow(int flow, int flag) {
        if (flag) this->flow += flow;
        else this->flow -= flow;
    }
    char getVertSt() {
        return vertSt;
    }
    char getVertFn() {
        return vertFn;
    }
    int getCapacity() {
        return capacity;
    }
    int getFlow() {
        return flow;
    }
};

class Ford_Falkerson : protected Check {
private:
    int numberEdges;                                       //количество ориентированных рёбер графа
    int maxFlow;                                           //максимальная величина потока
    int char_;                                             //для записи вершин в матрицу 

    int depth;
    int depth_;

    char source;
    char sink;

    std::vector <int> used;                                //просмотренные вершины                     
    std::vector <int> path;                                //построенный путь
    std::vector <Edge> listEdge;                           //для хранения ребер графа, введенных пользователем
    std::vector <std::vector <int> > matrix;               //матрица смежности взвешенного графа


    int findVertex(int vertex) {                           //для поиска смежных вершин
        for (int i = 0; i < SIZE; i++) {
            if (!used[i] && matrix[vertex][i] != 0) {      //не посещенные, ребра между которыми не закрыты
                return i;
            }
        }
        return -1;
    }

    void clear() {                                          //все вершины снова непросмотренные
        depth_ = depth;
        for (int i = 0; i < used.size(); i++)
            used[i] = 0;
    }

    void printPath() {
        for (auto now : path)
            if (now != int(sink - char_)) std::cout << char(now + char_) << " --> ";
            else std::cout << char(now + char_) << "\n";
    }

    bool deepSearch(int vertex) {                           //поиск в глубину: изначально все вершины помечены как не посещенные
        depth_ += 2;
        std::cout << std::setw(depth_) << ' ' << "Текущая вершина: " << char(vertex + char_) << "\n";

        if (vertex == (int)sink - char_) {                  //если дошли до стока
            std::cout << std::setw(depth_) << ' ' << "Путь найден!\n";
            std::cout << std::setw(depth_) << ' ';
            printPath();
            clear();
            return true;
        }
        else {
            used[vertex] = 1;                              //помечаем как посещенную
            int index = findVertex(vertex);
            if (index != -1) {
                path.push_back(index);                     //добавляем в путь
                return deepSearch(index);                  //продолжаем поиск от этой вершины
            }
            else {                                         //если таких вершин нет, берем предыдущую из списка посещенных
                used[vertex] = 2;                          //больше не рассматриваем
                path.pop_back();

                if (path.empty()) {
                    if (findVertex((int)source - char_) == -1) {
                        std::cout << std::setw(depth_) << ' ' << "Путь не найден!\n";
                        return false;    //если невозможно построить путь
                    }
                }
                depth_ -= 4;
                std::cout << std::setw(depth_ + 2) << ' ' << "Возвращаемся к предыдущей\n";
                return deepSearch(path.back());
            }
        }
    }

    int minCapacity() {                                    //поиск максимально возможного потока через найденный путь  
        int min = 9999;
        for (int i = 0, j = 1; j < path.size(); i++, j++) {
            std::cout << std::setw(depth) << ' ' << "Итерация: " << j << ". Максимальный поток: " << min << "\n";
            if (matrix[path[i]][path[j]] < min) {
                std::cout << std::setw(depth) << ' ' <<  matrix[path[i]][path[j]] << " < " << min << " - найдено ребро, с меньшей пропускной способностью.\n";
                    min = matrix[path[i]][path[j]];
            }
        }
        std::cout << std::setw(depth) << ' ' << "Найденный поток, пропускаемый через путь: " << min << "\n";
        return min;
    }

    void findEdge(char vertexSt, char vertexFn, int flow, int flag) { //если рассматриваемое ребро входит в список ребер, введенных пользователем, то запоминаем пропущенный поток
        for (int i = 0; i < listEdge.size(); i++) {
            if (listEdge[i].getVertSt() == vertexSt && listEdge[i].getVertFn() == vertexFn)
                listEdge[i].setFlow(flow, flag);
        }
    }

    void fillFlow(int flow) {                            //пропускание потока через путь
        for (int i = 0, j = 1; j < path.size(); i++, j++) {
            matrix[path[i]][path[j]] -= flow;           //уменьшение пропускной способности на величину потока
            findEdge((char)(path[i] + char_), (char)(path[j] + char_), flow, 1);

            matrix[path[j]][path[i]] += flow;           //обратные ребра
            findEdge((char)(path[j] + char_), (char)(path[i] + char_), flow, 0);
        }
    }

    static int cmpEdge(Edge a, Edge b) {                //для сортировки
        if (a.getVertSt() == b.getVertSt()) return a.getVertFn() < b.getVertFn();
        return a.getVertSt() < b.getVertSt();
    }

public:
    Ford_Falkerson() : numberEdges(0), maxFlow(0), depth(-1), depth_(-1), source('0'), sink('0'), char_(), used(SIZE, 0), matrix(SIZE, std::vector<int>(SIZE, 0)) {}

    void readMatrix(int flag) {
        if (flag) {
            std::cout << "Введите количество ориентированных ребер графа.\n";
            numberEdges = checkInt();
            std::cout << "Введите исток и сток графа.\n";
            std::cin >> source >> sink;
        }
        else {
            infile >> numberEdges;
            if (numberEdges <= 0) {
                std::cout << "Ошибка. Данные некорректны! Ожидается натуральное число.\n";
                exit(1);
            }
            infile >> source >> sink;
        }

        source = tolower(source);
        sink = tolower(sink);

        if (checkChar(source, sink)) {
            char vertex1;
            char vertex2;
            int weight;

            do {
                if (flag) {
                    std::cin >> vertex1 >> vertex2;
                    weight = checkInt();
                }
                else {
                    infile >> vertex1 >> vertex2;
                    infile >> weight;
                    if (weight <= 0) {
                        std::cout << "Ошибка. Данные некорректны! Ожидается натуральное число.\n";
                        exit(1);
                    }
                }

                vertex1 = tolower(vertex1);
                vertex2 = tolower(vertex2);

                if (isalpha(vertex1)) char_ = 97;                          //если граф записывается с помощью букв
                else char_ = 49;                                           //цифр

                matrix[vertex1 - char_][vertex2 - char_] = weight;         //заносим в матрицу

                Edge edge(vertex1, vertex2, weight);                       //и в список ребер
                listEdge.push_back(edge);

                numberEdges--;
            } while (numberEdges != 0);

            int flag2 = 0;                                                  //если из истока нет ребер, запуск алгоритма бессмысленен 
            for (int i = 0; i < SIZE; i++)
                if (matrix[source - char_][i] != 0) flag2 = 1;
            if (!flag2) {
                std::cout << "Нет ребер из истока.\n"; exit(1);
            }
            std::cout << "....................................................................\n";
            std::cout << " Введенные ребра: \n";
            depth++;
            output();
        }
        else {
            exit(1);                                                        //если исток = стоку
        }
    }

    bool algorithm() {                                                      //реализация алгоритма Форла-Фалкерсона
        path.push_back((int)source - char_);                                //добавляем исток в путь
        std::cout << std::setw(depth) << ' ' << "Запуск поиска в глубину.\n";
        if (!deepSearch((int)source - char_)) {                             //путей больше нет
            sort(listEdge.begin(), listEdge.end(), cmpEdge);
            std::cout << "....................................................................\n";
            std::cout << std::setw(depth) << ' ' << "Итоговый результат:\n";
            output();
            return true;
        }
        else {
            depth += 2;
            std::cout << std::setw(depth) << ' ' << "Запуск поиска максимального потока, то есть такого, который можно пропустить через любое ребро, входящее в путь.\n";
            fillFlow(minCapacity());                                        //пропускание потока
            path.clear();
            std::cout << "....................................................................\n";
            std::cout << std::setw(depth) << ' ' << "Промежуточный результат:\n";
            output();
            return algorithm();
        }
    }

    void output() {
        for (int i = 0; i < listEdge.size(); i++)
            if (listEdge[i].getVertFn() == sink) maxFlow += listEdge[i].getFlow();
        std::cout << std::setw(depth) << ' ' << "Текущее значение максимального потока: " << maxFlow << "\n";
        maxFlow = 0;

        std::cout << std::setw(depth) << ' ' << "Flow/Capacity\n";
        for (int i = 0; i < listEdge.size(); i++) {
            if (listEdge[i].getFlow() > 0)
                std::cout << std::setw(depth) << ' ' << listEdge[i].getVertSt() << " --" << listEdge[i].getFlow() << "/" << listEdge[i].getCapacity() << "--> " << listEdge[i].getVertFn() << "\n";
            else
                std::cout << std::setw(depth) << ' ' << listEdge[i].getVertSt() << " --0/" << listEdge[i].getCapacity() << "--> " << listEdge[i].getVertFn() << "\n";
        } std::cout << "\n";

        //stepik
        /*for (int i = 0; i < listEdge.size(); i++)
            if (listEdge[i].getVertFn() == sink) maxFlow += listEdge[i].getFlow();
        std::cout << maxFlow << "\n";
        for (int i = 0; i < listEdge.size(); i++) {
            if (listEdge[i].getFlow() > 0)
                std::cout << listEdge[i].getVertSt() << " " << listEdge[i].getVertFn() << " " << listEdge[i].getFlow() << "\n";
            else
                std::cout << listEdge[i].getVertSt() << " " << listEdge[i].getVertFn() << " " << '0' << "\n";
        }*/
    }
};

int main()
{
    setlocale(LC_ALL, "ru");
    char answer;
    int exit = 0;
    Ford_Falkerson start;

    do {
        std::cout << "Введите y, если будете вводить граф с консоли\n";
        std::cout << "n, если из файла.\n";
        std::cin >> answer;
        answer = tolower(answer);

        switch (answer) {
        case 'y':
            start.readMatrix(1);
            break;

        case 'n':
            start.readMatrix(0);
            break;

        default:
            std::cout << "Ошибка: введен неккоректный символ. Попробуйте еще раз.\n";
            exit = 1;
        }
    } while (exit == 1);

    start.algorithm();
    system("pause");
    return 0;
}