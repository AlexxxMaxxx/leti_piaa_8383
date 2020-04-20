//ЖАДНЫЙ АЛГОРИТМ
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
std::ifstream infile("test.txt");

class Edge {                            //РЕБРО
private:
    char name;                          //вершина ребра
    float weight;                       //вес ребра
    bool visit;

public:
    Edge(char name, float weight);

    void setName(char name);            //сеттеры
    void setWeight(float weight);
    void setVisit(bool visit);

    char getName();                     //геттеры
    float getWeight();
    bool getVisit();
};

Edge::Edge(char name, float weight) {
    this->name = name;
    this->weight = weight;
    visit = false;
}
                                        //сеттеры
void Edge::setName(char name) {
    this->name = name;
}

void Edge::setWeight(float weight) {
    this->weight = weight;
}

void Edge::setVisit(bool visit) {
    this->visit = visit;
}
                                        //геттеры
char Edge::getName() {
    return name;
}

float Edge::getWeight() {
    return weight;
}

bool Edge::getVisit() {
    return visit;
}
//---------------------------------

class Vertex {                          //ВЕРШИНА
private:
    char name;
    std::vector <Edge> edge;            //вектор смежных ребер

public:
    Vertex(char name);

    void setEdge(Edge edge);            //сеттеры

    char getName();                     //геттеры
    std::vector <Edge>& getEdge();
};

Vertex::Vertex(char name) {
    this->name = name;
}
                                          //сеттеры
void Vertex::setEdge(Edge edge) {
    this->edge.push_back(edge);
}
                                          //геттеры
char Vertex::getName() {
    return name;
}

std::vector <Edge>& Vertex::getEdge() {
    return edge;
}
//---------------------------------

bool readingGraph(std::vector <Vertex>& vertexGraph, char& finish) {                //считывание графа
    char vertex1, vertex2;
    float weightEdge;
    bool newVertex;
    bool finishNotExist = true;

    while (infile >> vertex1 >> vertex2 >> weightEdge && vertex1 != '!') {          //для окончания считывания вводим !!!
        if (vertex1 == vertex2) {
            continue;
        }
        if (vertex2 == finish) finishNotExist = false;                              //проверка что ребро до финишной вершины существует
        Edge e(vertex2, weightEdge);
        newVertex = true;

        for (int i = 0; i < vertexGraph.size(); i++) {
            if (vertexGraph[i].getName() == vertex1) {                              // если уже встречали вершину - добавляем новое ребро
                vertexGraph[i].setEdge(e);
                newVertex = false;
            }
        }

        if (newVertex) {                                                            //если новая вершина
            Vertex v(vertex1);
            v.setEdge(e);
            vertexGraph.push_back(v);
        }
    }
    return finishNotExist;
}

bool checkDeadlock(std::vector <Vertex>& vertex, char dealock) {   //проверка не является ли вершина тупиком
    bool flag = false;
    for (int i = 0; i < vertex.size(); i++) {
        if (vertex[i].getName() == dealock) {                       //если нет в векторе вершин, имеющих наборы смежных ребер
            flag = true;                                            //не тупик
        }
    }
    return flag;                                                    //тупик
}

void minEdge(Edge& min, Vertex& curVertex) {                        //для выбора ребра с минимальным весом
    int index = 0;
    for (int j = 0; j < curVertex.getEdge().size(); j++) {          //пробегаемся по ребрам
        if (min.getWeight() > curVertex.getEdge()[j].getWeight() && curVertex.getEdge()[j].getVisit() == false) { //нашли меньше
            min.setName(curVertex.getEdge()[j].getName());
            min.setWeight(curVertex.getEdge()[j].getWeight());
            index = j;
        }
    }
    curVertex.getEdge()[index].setVisit(true);                      //отмечаем ребро как посещенное
}

int check(std::vector <Vertex>& vertex, char name) {                //на случаи когда из родителя вершины-тупика, нет других ребер
    bool flag = false;
    for (int i = 0; i < vertex.size(); i++) {
        if (vertex[i].getName() == name) {
            if (vertex[i].getEdge().size() > 1) {
                flag = true;
            }
        }
    }
    return flag;
}

bool findPath(char start, char finish, std::vector <Vertex>& vertex, std::stack <char>& path) { //построение оптимального пути - рекурсивная функция
  
    path.push(start);                                               //записываем текущую стартовую вершину в текущий путь
    std::cout << "----------------------------------------------\n";
    std::cout << "\nПереход на (" << start << ")\n\n";

    if (start == finish) {                                          //оптимальный путь построен
        std::cout << "Достигнут финиш!\n";
        return true;
    }

    else {                                                          //продолжаем построение оптимального пути
        if (checkDeadlock(vertex, start)) {                         //если не тупик
            for (int i = 0; i < vertex.size(); i++) {
                if (vertex[i].getName() == start) {
                    std::cout << "Рассматриваемые ребра: \n";
                    for (int j = 0; j < vertex[i].getEdge().size(); j++) {
                        std::cout << start << " --"<< vertex[i].getEdge()[j].getWeight() << "--> " << vertex[i].getEdge()[j].getName() << "\n";
                    }std::cout << "\n";
                    if (vertex[i].getEdge().size() == 1) {          //если у текущей вершины только ОДНО смежное ребро
                        std::cout << "Ребро с минимальным весом: " << start << " --" << vertex[i].getEdge()[0].getWeight() << "--> " << vertex[i].getEdge()[0].getName() << "\n";
                        vertex[i].getEdge()[0].setVisit(true);      //то переходим по нему
                        return findPath(vertex[i].getEdge()[0].getName(), finish, vertex, path/*, weightPath*/);
                    }

                    else {
                        Edge min('0', 999);
                        minEdge(min, vertex[i]);                    //нужно выбрать самое дешевое ребро
                        std::cout << "Ребро с минимальным весом: " << start << " --" << min.getWeight() << "--> " << min.getName() << "\n";
                        return findPath(min.getName(), finish, vertex, path);
                    }
                }
            }
        }
        else {                                                      //если тупик
            std::cout << "Тупик!\n";
            do {
                std::cout << "Возвращаемся назад!\n";
                path.pop();                                         //убираем из пути
                start = path.top();                                 //стартом становится предыдущая вершина
            } while (!check(vertex, start));                        //проверка можно ли из нового старта пойти еще куда-то кроме тупика
            path.pop();                                             //которая будет добавлена в путь еще раз - поэтому убираем повтор
            return findPath(start, finish, vertex, path);
        }
    }
}

std::stack <char> coupStack(std::stack <char>& path) {              //переворачивание стека (cba-->abc)
    std::stack <char> path2;;
    while (!path.empty()) {
        path2.push(path.top());
        path.pop();
    }
    return path2;
}

void printPath(std::stack <char> path) {                            //вывод содержимого стека
    std::cout << "----------------------------------------------\n";
    std::cout << "\nOптимальный путь:\n";
    do {
        if (path.size() != 1) std::cout << path.top() << " --> ";
        else std::cout << path.top();
        path.pop();
    } while (!path.empty());
    std::cout << "\n";
}

int main() {
    setlocale(LC_ALL, "ru");

    std::vector <Vertex> vertexGraph;                               //вектор вершин - граф
    std::stack <char> path;                                         //для хранения пути   

    char start, finish;                                             //старт и финиш
    infile >> start >> finish;
    std::cout << "\n\nПостроения оптимального пути из " << start << " в " << finish << ".\n";

    if (start == finish) std::cout << "\nOптимальный путь:\n" << start;
    else
        if (!readingGraph(vertexGraph, finish)) {                        //считывание графа
            std::cout << "\nЗапуск Жадного Алгоритма\n\n";
            findPath(start, finish, vertexGraph, path);                  //построение пути - жадный алгорит
            path = coupStack(path);                                     //восстанавление пути
            printPath(path);                                            //печать пути
        }
        else {
            std::cout << "Граф введен неправильно!\n";
        }
    return 0;
}