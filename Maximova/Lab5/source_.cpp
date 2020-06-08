#include <iostream> //разделить на файлы
#include <vector>
#include <map>
#include <algorithm> //sort

#define MaxText    1000000
#define MaxPattern 750

class Vertex {                              //вершина
private:

    bool endStr;                            //флаг = true, если в этой вершине оканчивается какая-либо (из имеющихся) строка-образец
    std::map <char, int> nextVertex;        //вершины-потомки: если в словаре есть, например, элемент с ключом 'A' - это означает,
                                            //что от текущей вершины можно пройти по ребру 'A'
    std::map <char, int> transitions;      
    int numbPattern;                        //номер строки-образца, обозначаемого этой вершиной
    int suffixLink;                         //суффиксные ссылки

    char nameEdge;
    int parent;                             //предок для реализации SufLink
    char parentGoCur;                       //символ, по которому их предка можно попасть в текущюю вершину (для суффиксных ссылок)

public:
    Vertex() : endStr(false), nameEdge('0'), numbPattern(-1), suffixLink(-1), parent(-1), parentGoCur('0') {}

    //Vertex(bool endStr, int numbPattern) : Vertex() { //делегирующий конструктор
    //    this->endStr = endStr;
    //    this->numbPattern = numbPattern;
    //}

    //сеттеры
    void setEndStr(bool value) {
        endStr = value;
    }

    void setNextVertex(char symb, int value) {
        nextVertex.insert(std::make_pair(symb, value));
    }

    void setNumbPattern(int value) {
        numbPattern = value;
    }

    void setSuffixLink(int direction) {
        suffixLink = direction;
    }

    void setNameEdge(char name) {
        nameEdge = name;
    }

    void setParent(int parent) {
        this->parent = parent;
    }

    void setTransitions(char symb, int index) {
        transitions.insert(std::make_pair(symb, index));
    }

    //геттеры
    std::map <char, int>& getTransitions() {
        return transitions;
    }

    bool getEndStr() {
        return endStr;
    }

    std::map <char, int>& getNextVertex() {
        return nextVertex;
    }

    int getNumbPattern() {
        return numbPattern;
    }

    int getSuffixLink() {
        return suffixLink;
    }

    char getNameEdge() {
        return nameEdge;
    }

    int getParent() {
        return parent;
    }

    bool findMapElem(char symb) {
        auto it = nextVertex.find(symb);
        if (it == nextVertex.end())
            return false;
        return true;
    }

    bool findMapTranzit(char symb) {
        auto it = transitions.find(symb);
        if (it == transitions.end())
            return false;
        return true;
    }

};



class Bohr {  //бор
private:

    int maxArcs;                                    //максимальное количество дуг,исходящих из одной вершины
    int numbPattern;                                 //кол-во паттернов

    size_t maxSizeBohr;                              //максимальный размер бора
    size_t realSizeBohr;                             //реальный размер бора

    std::string string_;                             //строка поиска

    std::vector <Vertex> bohr;                       //бор, с который будем работать
    std::vector <std::string> patterns;              //строки - шаблоны
    std::vector <std::pair <int, int>> result;       //i - позиция в тексте с 1; p - номер образца

public:

    Bohr(std::string string_, int numbPattern, std::vector <std::string> patterns, size_t lengthPatterns) : realSizeBohr(0), maxArcs(0) {
        this->string_ = string_;
        this->numbPattern = numbPattern;
        this->patterns = patterns;

        maxSizeBohr = lengthPatterns + 1;
        bohr.resize(maxSizeBohr);               //в худшем случае все-строки образцы не имеют одинаковых префиксов ->
                                                //-> верхняя оценка числа вершин бора = сумма длин всек строк-образцов + 1 (root)
                                                //свободная память потом освобождается
        realSizeBohr = 1;                       //корень - инициализирован при создании
        bohr[0].setSuffixLink(0);               //суффиксная ссылка от корня ведет в корень
        addPatterns();
    }

    void initVertex(char curSymb, int parent) { //инициализация вершин
        bohr[realSizeBohr].setEndStr(false);
        bohr[realSizeBohr].setNumbPattern(-1);
        bohr[realSizeBohr].setParent(parent);
        bohr[realSizeBohr].setNameEdge(curSymb);
        realSizeBohr++;
    }

    void  addVertex(int curIndex, char curSymb) {  //добавление вершин в бор
        initVertex(curSymb, curIndex);
        bohr[curIndex].setNextVertex(curSymb, realSizeBohr - 1);
    }

    void addPatterns() {
        std::cout << "****************************************************\n";
        std::cout << "Этап первый: добавление строк-образцов в бор.\n";

        int curIndex;                                   //начинаем из корня бора - индекс текущий вершины в боре
        char curSymb;

        for (size_t i = 0; i < numbPattern; i++) {      //добавление в бор всех строк 
            curIndex = 0;                               //добавление новых строк начинается с корня
            std::cout << "\nДобавление шаблона " << patterns[i] << " в бор.\n";
            for (size_t j = 0; j < patterns[i].size(); j++) { //проходим по строке
                
                curSymb = patterns[i][j];                     //текущий символ

                if (!bohr[curIndex].findMapElem(curSymb)) {   //если ребро еще не добавлено -добавляем
                    addVertex(curIndex, curSymb);
                        std::cout << "Создание ребра: --" << curSymb << "-->\n";
                }
                else {
                        std::cout << "Ребро --" << curSymb << "--> уже создано.\n";
                }

                curIndex = bohr[curIndex].getNextVertex()[curSymb];
            }

            std::cout << "Добавление ребра завершено.\n";
            bohr[curIndex].setEndStr(true);
            bohr[curIndex].setNumbPattern(i + 1);           //номер образца в ответ выводить с 1
        }

        bohr.resize(realSizeBohr);                          //освобождаем незанимаемую память
        std::cout << "\nКоличество вершин в боре = " << bohr.size() << "\n";
        std::cout << "****************************************************\n";

                                                             //поиск максимального количества дуг, исходящих из одной вершины
        findEntry();
    }

    int setGetTransition(int indexVertex, char symbol) {   //переходы

        if (bohr[indexVertex].findMapTranzit(symbol)) {    //у вершины есть переход по символу
            std::cout << "Переход из вершины " << indexVertex << " по символу " << symbol << " уже вычислен:\n";
            std::cout << indexVertex << " --" << symbol << "--> " << bohr[indexVertex].getTransitions()[symbol] << "\n\n";
        }
        else {                                              //нет перехода по символу в словаре - находим его
            std::cout << "Переход из вершины " << indexVertex << " по символу " << symbol << " еще не вычислен.\n";

            if (bohr[indexVertex].findMapElem(symbol)) {    //есть ли ребро с нужным символом
                bohr[indexVertex].setTransitions(symbol, bohr[indexVertex].getNextVertex()[symbol]);
                std::cout << "Найден переход: " << indexVertex << " --" << symbol << "--> " << bohr[indexVertex].getTransitions()[symbol] << "\n\n";
                //устанавливаем переход по символу с индексом = номеру вершины, в которую мы передем из текущем по ребру с этим символом
            }
            else {
                if (!indexVertex) { 
                    bohr[indexVertex].setTransitions(symbol, 0); //добавляем переход
                    std::cout << "Найден переход: " << indexVertex << " --" << symbol << "--> " << bohr[indexVertex].getTransitions()[symbol] << "\n\n";
                }
                else {//иначе переходим по суффиксной ссылке и ищем переход там
                    std::cout << "Ищем переход:\n";
                    bohr[indexVertex].setTransitions(symbol, setGetTransition(setGetSuffixLink(indexVertex), symbol));
                    std::cout << "Найден переход: " << indexVertex << " --" << symbol << "--> " << bohr[indexVertex].getTransitions()[symbol] << "\n\n";
                }
            }
        }
        return bohr[indexVertex].getTransitions()[symbol]; //возвращаем переход
    }


    int setGetSuffixLink(int indexVertex) {             //суффикусные ссылки
;

        if (bohr[indexVertex].getSuffixLink() != -1) {  //есть суффиксная ссылка
            std::cout << "Суффиксная ссылка уже вычислена: ";
            std::cout << indexVertex << "-->" << bohr[indexVertex].getSuffixLink() << "\n\n";
        }

        else {                                          //нужно установить
            if (!bohr[indexVertex].getParent()) {       //если родитель корень
                std::cout << "Суффиксная ссылка еще не вычислена: ";
                bohr[indexVertex].setSuffixLink(0);     //устанавливаем ссылку
                std::cout << "Суффиксная ссылка уже вычислена: ";
                std::cout << indexVertex << "-->" << bohr[indexVertex].getSuffixLink() << "\n\n";
            }

            else {                                      //нужно вычислить
                                                        //переход по ссылке предка, а после переход от вершины, 
                                                        //в которую пришли, по символу на ребре от предка до нашей вершины
                std::cout << "Суффиксная ссылка еще не вычислена.\n";
                std::cout << "\nВычислим ее, перейдя по суффиксной ссылке родителя - " << bohr[indexVertex].getParent() << "\n";
                bohr[indexVertex].setSuffixLink(setGetTransition(setGetSuffixLink(bohr[indexVertex].getParent()), bohr[indexVertex].getNameEdge()));
                std::cout << indexVertex << "-->" << bohr[indexVertex].getSuffixLink() << "\n\n";
            }

        }

        return bohr[indexVertex].getSuffixLink();
    }

    void findEntry() {                                                  //поиск вхождений в тексте
        std::cout << "Запуск поиска по строке: " << string_ << "\n";
        int curIndex = 0;                                               //поиск с корня

        for (size_t i = 0; i < string_.length(); i++) {                 //бежим по строке
            std::cout << "Текущая вершина: " << curIndex << "\n";
            curIndex = setGetTransition(curIndex, string_[i]);          //осуществляем переход

            for (size_t j = curIndex; j > 0; j = setGetSuffixLink(j)) { //проходим по суффикс ссылкам, пока не попадем в корень

                if (bohr[j].getEndStr()) {                              //нашли вхождение шаблона в текст
                    std::cout << "Пришли в конечную вершину: " << curIndex << "\n";
                    std::cout << "Найден шаблон: " << patterns[bohr[j].getNumbPattern() - 1] << "\n";
                    std::cout << "Индекс вхождения = " << (i + 1) - (patterns[bohr[j].getNumbPattern() - 1].length() - 1);
                    std::cout << "\nНомер шаблона = " << bohr[j].getNumbPattern() << "\n";
                    result.push_back(std::make_pair((i + 1) - (patterns[bohr[j].getNumbPattern() - 1].length() - 1), bohr[j].getNumbPattern()));
                }
                std::cout << "\nПереход по суффиксной ссылке на вершину " << curIndex << "\n";
            }
        }

        coutResult();
    }

    static int cmp(std::pair <int, int> a, std::pair <int, int> b) {    //для сортировки
        if (a.first == b.first) return a.second < b.second;
        return a.first < b.first;
    }

    void findMaxArcs() {                                                //вычисление максимальнгого количества дуг
        std::cout << ".............................................................\n";
        std::cout << "Вычисление максимального количества дуг из одной вершины: \n";

        std::cout << "Текущий максимум = " << maxArcs << "\n";
        for (size_t i = 0; i < bohr.size(); i++) {

            
            std::cout << "Сравниваем количество дуг с текущим максимальным: \n";

            if (bohr[i].getNextVertex().size() > maxArcs) {
                std::cout << bohr[i].getNextVertex().size() << " > " << maxArcs << "\n";
                maxArcs = bohr[i].getNextVertex().size();
                std::cout << "Текущий максимум = " << maxArcs << "\n";
            }  
            else {
                std::cout << bohr[i].getNextVertex().size() << " <= " << maxArcs << "\n";
                std::cout << "Текущий максимум = " << maxArcs << "\n";
            }
        }
        std::cout << "Результат: " << maxArcs << "\n";
        std::cout << ".............................................................\n";
    }

    std::string cutText() {                                             //вырезание найденных шаблонов из текста
        std::cout << "-------------------------------------------------------------\n";
        std::cout << "Вырезание шаблонов из текста\n";
        std::string temp;
        for (size_t i = 0; i < result.size(); i++) {
            for (size_t j = result[i].first - 1; j < (result[i].first + patterns[result[i].second - 1].size()) - 1; j++) {
                string_[j] = '#';
            }
        }
        std::cout << "Замена всех шаблонов на символ # - результат: ";
        std::cout << string_ << "\n";

        for (size_t k = 0; k < string_.size(); k++) {
            if (string_[k] != '#')
                temp.push_back(string_[k]);
        }
        std::cout << "Вырезание # - результат: " << temp << "\n";
        std::cout << "-------------------------------------------------------------\n";
        return temp;
    }

    void coutResult() {                                                 //печать результата

        std::cout << "\nЗадания для индивидулизации:\n";
        findMaxArcs();
        string_ = cutText();

        std::cout << "\nИтоговое решение:\n";
        std::cout << "вхождение шаблона в строку поиска / номер шаблона : \n";

        sort(result.begin(), result.end(), cmp);                        //сортировка

        for (size_t i = 0; i < result.size(); i++)
            std::cout << result[i].first << " " << result[i].second << "\n";

        std::cout << "Остаток строки поиска: ";
        for (size_t k = 0; k < string_.size(); k++) {
            std::cout << string_[k];
        }
        std::cout << "\nМаксимальное количество дуг, исходящих из одной вершины в боре: " << maxArcs << "\n";
    }
};

class Reader {
private:
    std::string string_;                                                //строка, в которой осуществляется поиск вхождений
    std::vector <std::string> patterns;                                 //строки-шаблоны
    int numbPattern;                                                    //количество строк-образцов

public:
    Reader() : string_("0"), numbPattern(0) {
        startRead();
    }

    void check(std::string& string_) {
        int exit = 1;

        do {
            std::cin >> string_;
            if (string_.length() > MaxText) {
               std::cout << "Недопустимый размер строки.\n";
               std::cout << "Попробуйте снова.\n";
            }
            else
                exit = 0;
        } while (exit);
    }

    void check(int& numbPattern)                                            //проверка корректность введенных данных
    {
        while (!(std::cin >> numbPattern))
        {
              std::cout << " Ошибка: данные некорректны! Ожидается число.\n";
              std::cin.clear();                                           	//сброс коматозного состояния cin 
              std::cin.ignore(1000, '\n');
            fflush(stdin);		                                            //очистка потока ввода
        }

        std::cin.get();
    }

    void checkData(std::string& temp) {
        int exit = 1;

        do {
            std::cin >> temp;
            if (temp.length() > MaxPattern) {
                  std::cout << "Недопустимый размер шаблона.\n";
                  std::cout << "Попробуйте снова.\n";
            }
            else if (temp.length() > string_.length()) {
                  std::cout << "Длина шаблона не может превышать длину строки.\n";
                  std::cout << "Попробуйте снова.\n";
            }
            else
                exit = 0;
        } while (exit);
    }

    void startRead() {
        size_t lengthPatterns = 0;

        std::cout << "Введите строку, в которой будет осуществлятся поиск шаблонов.\n";
        check(string_);

        std::cout << "Введите количество шаблонов.\n";
        check(numbPattern);

        std::cout << "Введите шаблон(ы).\n";
        std::string temp;
        for (int i = 0; i < numbPattern; i++) {
            checkData(temp);
            lengthPatterns += temp.length();
            patterns.push_back(temp);
        }

        Bohr bohr(string_, numbPattern, patterns, lengthPatterns);
    }
};

int main()
{
    setlocale(LC_ALL, "rus");

    std::cout << "Ввод данных.\n";
    Reader now_;

    return 0;
}