//Алгоритм А*
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>

std::ifstream infile("test.txt");

class Edge {						//ребро
private:
	char name;
	float weight;

public:
	Edge(char name, float weight);
	char getName();
	float getWeight();
};

Edge::Edge(char name, float weight) {
	this->name = name;
	this->weight = weight;
}

char Edge::getName() {
	return name;
}

float Edge::getWeight() {				//вес ребра
	return weight;
}
//_____________________________________________________________________
class Vertex {						//вершина
private:
	char name;
	char parent;
	float funcG;					//g(x) - наименьшая стоимость пути в x из стартовой вершины
	float funcH;					//h(x) - эвристическое приближение стоимости пути от x до конечной цели
	float funcF;					//f(x) = g(x) + h(x) - длина пути до цели
	std::vector <Edge> edges;		//смежные ребра

public:
	Vertex(char name);

	void setParent(char parent);
	void setG(float g);
	void setH(char fnsh);
	void setF();
	void setEdge(Edge edge);

	char getName();
	char getParent();
	float getG();
	float getH();
	float getF();
	std::vector <Edge>& getEdge();
};

Vertex::Vertex(char name) {
	this->name = name;
	parent = '0';
	funcG = funcH = funcF = -1;
}

void Vertex::setParent(char parent) {	//сеттеры
	this->parent = parent;
}
void Vertex::setG(float g) {
	funcG = g;
}
void Vertex::setH(char fnsh) {
	funcH = abs((int)fnsh - (int)name);
}
void Vertex::setF() {
	funcF = funcG + funcH;
}
void Vertex::setEdge(Edge edge) {
	edges.push_back(edge);
}

char Vertex::getName() {				//геттеры
	return name;
}
char Vertex::getParent() {
	return parent;
}
float Vertex::getG() {
	return funcG;
}
float Vertex::getH() {
	return funcH;
}
float Vertex::getF() {
	return funcF;
}
std::vector <Edge>& Vertex::getEdge() {
	return edges;
}
//_____________________________________________________________________
class BinaryHeap {					//бинарная куча
private:
	std::vector <Vertex> arr;		//на базе вектора
	bool cmp(Vertex a, Vertex b);
	void swap(int a, int b);
	int parent(int i);
	int leftChild(int i);
	int rightChild(int i);
	void heapify(int i);

public:

	void addHeap(Vertex elem);
	void heapDelete();
	int find(char elem);
	Vertex find_(char elem);
	void outHeap();
	void outArr();
	Vertex getMin();
	void set(int pos, Vertex vertex);
	int sizeHeap();
	bool isEmpty();
};

bool  BinaryHeap::cmp(Vertex a, Vertex b) {
	if (a.getF() == b.getF()) return a.getName() < b.getName() ? true : false;		//если приоритеты равны
	else return a.getF() < b.getF() ? true : false;
}

void  BinaryHeap::swap(int a, int b) {		//используется в heapify
	Vertex temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

int  BinaryHeap::parent(int i) {				//вычисление индексов родителей и потомков
	return (i - 1) / 2;
}

int  BinaryHeap::leftChild(int i) {
	return 2 * i + 1;
}

int  BinaryHeap::rightChild(int i) {
	return 2 * i + 2;
}

void  BinaryHeap::heapify(int i) {			//для восстановления свойств кучи
	int cur = i;
	int left = leftChild(i);
	int right = rightChild(i);

		//проверка
	if (left < arr.size() && cmp(arr[left], arr[cur])) {
		cur = left;
	}

	if (right < arr.size() && cmp(arr[right], arr[cur])) {
		cur = right;
	}
		
	if (cur != i) {				//если свойство нарушено
		swap(i, cur);
		heapify(cur);
	}
}
	
void  BinaryHeap::addHeap(Vertex elem) {		//добавление элемента в конец и перемещение наверх, в соответствии с приоритетом
	arr.push_back(elem);
	int i = arr.size() - 1;
	while (i > 0 && cmp(arr[i], arr[parent(i)])) {
		swap(i, parent(i));
		i = parent(i);
	}
}

void  BinaryHeap::heapDelete() {				//удаление элемента с минимальным приоритетом (так как min-heap)
	if (arr.size() < 1) {
		std::cout << "Ошибка: нет элемента для удаления!\n";
		exit(1);
	}

	arr[0] = arr[arr.size() - 1];	//на его место ставится элемент с конца
	arr.pop_back();
	heapify(0);						//который потом переходит на нужную позицию
}

int  BinaryHeap::find(char elem) {				//возвращает индекс искомого элементв
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i].getName() == elem) {
			return i;
		}
	}
	return -1;
}

Vertex  BinaryHeap::find_(char elem) {			//возвращает сам элемент
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i].getName() == elem) {
			return arr[i];
		}
	}
}

void  BinaryHeap::outHeap() {					//для печати в виде кучи
	int i = 0;
	int j = 1;

	while (i < arr.size()) {
		while ((i < j) && (i < arr.size())) {
			std::cout << arr[i].getName() << " ";
			i++;
		}
		std::cout << "\n";
		j = j * 2 + 1;
	}
}

void  BinaryHeap::outArr() {						//для печати в виде массива
	for (int i = 0; i < arr.size(); i++) {
		std::cout << arr[i].getName() << " ";
	}
	std::cout << "\n";
}

Vertex  BinaryHeap::getMin() {					//минимум всегда в корне
	if (arr.size() > 0) return arr[0];
	else std::cout << "Ошибка: пустая куча"; exit(1);
}

void  BinaryHeap::set(int pos, Vertex vertex) {
	arr[pos] = vertex;
}

int  BinaryHeap::sizeHeap() {
	return arr.size();
}

bool  BinaryHeap::isEmpty() {
	return (arr.size() == 0);
}
//_____________________________________________________________________
class PriorityQueue {					//очередь с приоритетом
private:
	BinaryHeap binHeap;					//на базе бинарной кучи
public:									//интерфейс очереди
	void push(Vertex elem);
	void pop();
	void set(int pos, Vertex a);
	void out();
	Vertex find_(char value);
	Vertex top();
	int find(char value);
	int size();
	bool empty();
};

							//интерфейс очереди
void PriorityQueue::push(Vertex elem) {
	binHeap.addHeap(elem);
}
void PriorityQueue::pop() {
	binHeap.heapDelete();
}
void PriorityQueue::set(int pos, Vertex a) {
	binHeap.set(pos, a);
}
void PriorityQueue::out() {
	binHeap.outArr();
}
Vertex PriorityQueue::find_(char value) {
	return binHeap.find_(value);
}
Vertex PriorityQueue::top() {
	return binHeap.getMin();
}
int PriorityQueue::find(char value) {
	return binHeap.find(value);
}
int PriorityQueue::size() {
	return binHeap.sizeHeap();
}
bool PriorityQueue::empty() {
	return binHeap.isEmpty();
}
//_____________________________________________________________________

bool readingGraph(std::vector <Vertex>& graph, char finish, int & weightPath) {				//считывание ориентированного графа

	char vertex1, vertex2;
	float weightEdge;
	bool newVertex;
	bool finishNotExist = true;												//обработка ошибок пользователя

	while (infile >> vertex1 >> vertex2 >> weightEdge) {
		if (vertex2 == finish) finishNotExist = false;
		if (graph[0].getName() == finish) {
			if (vertex1 == graph[0].getName() && vertex2 == finish) {
				weightPath = weightEdge;
			}
		}
		Edge edge(vertex2, weightEdge);
		newVertex = true;

		for (int i = 0; i < graph.size(); i++) {
			if (graph[i].getName() == vertex1) {							//если встречали вершину - добавляем новое ребро
				graph[i].setEdge(edge);
				newVertex = false;
			}
		}

		if (newVertex) {													//если новая вершина
			Vertex vertex(vertex1);
			vertex.setEdge(edge);
			graph.push_back(vertex);
		}
	}

	graph[0].setG(0);														//заполняем св-ва вершины
	graph[0].setH(finish);
	graph[0].setF();

	return finishNotExist;
}

bool isProcessed(char name, std::vector <char> processed) {												//обработана ли вершина? в векторе нет find
	for (int i = 0; i < processed.size(); i++) {
		if (processed[i] == name) {
			return true;
		}
	}
	return false;
}

bool isDeadlock(char name, std::vector <Vertex> graph) {												//проверка на тупик
	for (int i = 0; i < graph.size(); i++) {
		if (graph[i].getName() == name) {																//если есть в векторе вершин, имеющих ребра - то не тупик
			return true;
		}
	}
	return false;
}

Vertex find(char name, std::vector <Vertex> graph){												//получение соседней вершины
	for (int i = 0; i < graph.size(); i++) {
		if (graph[i].getName() == name) {
			return graph[i];
		}
	}
}

void newValueFunc(Vertex & vertex, char parent, float g, char finish) {								//обновляем значения функций - свойства вершины
	vertex.setG(g);
	vertex.setH(finish);
	vertex.setF();
	vertex.setParent(parent);
}

void printInfo(Vertex vertex) {																			//для вывода промежуточных данных
	std::cout << "\nСвойства вершины " << vertex.getName() << ": \n";
	std::cout << "g(x) = " << vertex.getG() << " \nh(x) = " << vertex.getH() << " \nf(x) = " << vertex.getF() << " \n\n";
}

void printInfo(PriorityQueue queue, std::vector <char> processed, int &iter) {
	std::cout << "--------------------------------";
	std::cout << "\nИтерация: " << ++iter << "\n\n";
	std::cout << "Открытые вершины: ";
	queue.out();
	std::cout << "Закрытые вершины: ";
	for (int i = 0; i < processed.size(); i++) {
		std::cout << processed[i] << " ";
	} std::cout << "\n\n";
}

bool findPathAstar(std::vector <Vertex> graph, std::map <char, char>& minPath, char finish, char start, int & weightPath) {			//поиск кратчайшего пути - итеративная функция
	int iter = 0;
	float gFunc;																						//минимальный путь до текущей вершины

	std::vector <char> processed;																		//массив обработанных вершин (хранит имена вершин)
																//очередь с приоритетом, хранящая вершины, которые нужно обработать
	PriorityQueue queue;
					//добавляем стартовую вершину в очередь 
	queue.push(graph[0]);

	while (!queue.empty()) {	
		printInfo(queue, processed, iter);
																//берем из отсортированной очереди вершину с наим приоритетом
		Vertex curVertex = queue.top();
		std::cout << "Обрабатываемая вершина: " << curVertex.getName() << "\n\n";

		if (curVertex.getName() != graph[0].getName()) { 
			minPath.insert(std::make_pair(curVertex.getName(), curVertex.getParent()));
		}

			//убираем из очереди
		queue.pop();

		processed.push_back(curVertex.getName());														//отмечаем как просмотренную

		if (curVertex.getName() == finish) {	//дошли до финиша
			weightPath = curVertex.getF();
			std::cout << "Кратчайший путь построен.\n";
			return true;
		}
		else {					  																		
		
			if (!curVertex.getEdge().empty()) {															//если есть соседи - просматриваем
				
				for (int i = 0; i < curVertex.getEdge().size(); i++) {
					std::cout << "Сосед: " << curVertex.getEdge()[i].getName() << " ";
					if (isDeadlock(curVertex.getEdge()[i].getName(), graph)) {							//если сосед не является тупиком или это финишная вершина(!)
																									
						if (!isProcessed(curVertex.getEdge()[i].getName(), processed)) {				//сосед не был обработан - работаем с соседом
							Vertex neighbour = find(curVertex.getEdge()[i].getName(), graph);
							gFunc = curVertex.getG() + curVertex.getEdge()[i].getWeight();				//минимальный путь от старта до рассматриваемого соседа
		
										//не записан в очередь
							if(queue.find(curVertex.getEdge()[i].getName()) == -1){															//то нужно присвоить значение и записать в очередь
								newValueFunc(neighbour, curVertex.getName(), gFunc, finish);			//то обновляем параметры
													//добавляем в очередь
								queue.push(neighbour);
								printInfo(neighbour);
								
							}
							else {																		//записан
																										//проверяем, не нашли ли новый минимальный путь?
								if (gFunc <	queue.find_(neighbour.getName()).getG()) {		//нашли новый минимальный путь
									newValueFunc(neighbour, curVertex.getName(), gFunc, finish);		//то обновляем параметры
									
									int index = queue.find(neighbour.getName());
									queue.set(index, neighbour);
									printInfo(neighbour);
								}
								else {
									printInfo(queue.find_(neighbour.getName()));
								}

							}
							

						}
						else {																			//уже обрабатывали соседа 
							std::cout << " <-- уже обработанa!\n\n";
							continue;
						}
					}
					else {	//тупик
						std::cout << " <-- тупик!\n\n";
						continue;
					}
				}
			}
			else {																						//проблемы с графом
				return false;
			}
		}
	}
}

void printPath(std::stack <char>& answer) {
	std::cout << "--------------------------------";
	std::cout << "\nПОЛУЧЕННЫЙ ПУТЬ:\n";
	do {
		if(answer.size() != 1) std::cout << answer.top() << " --> ";
		else std::cout << answer.top();
		answer.pop();
	} while (!answer.empty());

}

void buildPath(char start, char finish, std::stack <char>& answer, std::map <char, char> minPath) {		 //восстанавливаем путь
	char curNode = finish;
	answer.push(curNode);

	while (curNode != start) {
		curNode = minPath[curNode];
		answer.push(curNode);
	}
	printPath(answer);																					//вызов печати пути
}

int main() {
	setlocale(LC_ALL, "rus");

	std::vector <Vertex> graph;																			//считанный ориентированный граф
	std::map <char, char> minPath;																		//кратчайший путь
	std::stack <char> answer;																			//результат
	int weightPath = 0;

	char start, finish;
	infile >> start >> finish;

	Vertex vertexSt(start);																				//чтобы старт - как вершина, всегда был в начале массива граф
	graph.push_back(vertexSt);
	Vertex vertexFn(finish);
	graph.push_back(vertexFn);

	if (!readingGraph(graph, finish, weightPath)) {																	//считывание 
		if (weightPath) {
			std::cout << "ПОЛУЧЕННЫЙ ПУТЬ: " << start << "\nДлина пути = " << weightPath << "\n";
		}
		
		else {
			std::cout << "Запуск А*.\n";
			if (findPathAstar(graph, minPath, finish, start, weightPath)) {													//алгоритм построения кратчайшего пути
				buildPath(vertexSt.getName(), finish, answer, minPath);										//строим путь по карте и печатаем
				std::cout << "\n\nДлина пути = " << weightPath << "\n";
			}
			else {
				std::cout << "Граф введен неправильно.\n";													//предупреждение ошибок
			}
		}
			
	}
	else {
		std::cout << "Граф введен неправильно.\n";														//если переход к финишу не возможен ни через какое ребро
	}

	return 0;
}
