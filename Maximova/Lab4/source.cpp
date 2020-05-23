#include <iostream>
#include <string>
#include <vector>
#include <cmath>

class KMP{                                          //класс, реализующий КМП
private:

    int sizeTempl;                                  //длина шаблона
    int sizeText;                                   //длина текста
    int task;                                       //0 - поиск вхождений, 1 - сдвиг?
    int quantityBlock;                              //кол-во блоков для разделения текста
    int sizeBlock;                                  //размер блока
    int sizeBlockSk;                                //максимальный размер обрабатываемого блока

    std::string templP;                             //шаблон
    std::string textT;                              //текст

    std::vector <int> prefixFunc;                   //префикс-функция
    std::vector <int> indexEntry;                   //для результата
    std::vector <std::string> block;                //для хранения блоков


    void setTemplP(std::string P){                  //сеттеры
        templP = P;
    }

    void setTextT(std::string T){
        if(!task) textT = T;
        else textT = T + T;                         //для определения сдвига
    }

    void setQuantBlock(int quantBl){
        quantityBlock = quantBl;
    }


    int checkFirst(bool flag){                        //для проверки входных данных при поиске вхождений
        if(!sizeTempl || !sizeText) {                 // размер равен 0 - неверно
            if(flag) std::cout << "Ошибка: Введены не все исходные данные\n";
            return -1;
        }

        if(sizeTempl > sizeText) {                    // шаблон больше текста - неверно
            if(flag) std::cout << "Ошибка: Длина шаблона > Длины текста\n";
            return -1;
        }

        if(sizeTempl == sizeText && templP == textT) { // один и тот же текст - ответ 0
            if(flag) std::cout << "Введены два эквивалентных текста\n";
            return 0;
        }

        return 1;                                  //иначе запускаем алгоритм
    }

    int checkSecond(std::string T, std::string P, bool flag){      //для проверки входных данных при определении сдвига
        if(!T.length() || !P.length()) {                           // размер равен 0 - неверно
            if(flag) std::cout << "Ошибка: Введены не все исходные данные\n";
            return -1;
        }

        if(P.length() != T.length()) {                             // шаблон больше текста - неверно
            if(flag) std::cout << "Ошибка: Длина А должна быть равна длине В\n";
            return -1;
        }

        if(P.length() == T.length() && P == T) {                   // один и тот же текст - ответ 0
            if(flag) std::cout << "Введены два эквивалентных текста\n";
            return 0;
        }

        return 1;                                                  //иначе запускаем алгоритм
    }

    bool find(int elem){
        for(int i = 0; i < indexEntry.size(); i++){
            if(indexEntry[i] == elem) return true;
        }
        return false;
    }

    void trainingText(){                                           //разрезание исходного текста на блоки
        std::cout << "\nЭтап 1 - Разделение текста на k = " << quantityBlock << " блоков\n";
        std::cout << "Исходный текст: ";
        printText();
        std::cout << "Количество блоков k = " << quantityBlock;
        std::cout << "\n";

        block.resize(quantityBlock);
        sizeBlock = floor(sizeText / quantityBlock);                //размер блока
        if(sizeTempl != 1) sizeBlockSk = sizeBlock + (sizeTempl - 1);                  //максимальный размер сканируемого блока
        else sizeBlockSk = sizeBlock + 2;

        std::string temp;
        int index;

        std::cout << "Максимальный размер блока: " <<  sizeBlockSk << "\n";

        for(int i = 0; i < quantityBlock; i++){                     //разрезание на блоки
            index = sizeBlock * i;

            for(int j = index; j < sizeBlockSk + index; j++){
                if(j < sizeText)                                    //отрезаем от исходного текста необходимый блок
                    temp += textT[j];
                else
                    break;                                          //последний блок может быть меньше максимального размера
            }

            block[i] = temp;                                        //кладем в вектор блоков
            temp.clear();

            std::cout << "Блок " << i + 1 << ": ";
            printBlock(i);
            std::cout << "\n";
        }
        std::cout << "\n";
    }


    void createPrefixFunc(){                                        // формирование массива префикс-функции
        std::cout << "Этап 2 - Формирование массива префикс-функции\n";
        std::cout << "Обрабатываемый шаблон: ";
        printPattern();

        int j = 0;                                                    // j, i - указывают на рассматриваемые символы
        int i = 1;
        prefixFunc.push_back(0);                                      // prefixFunc[0] = 0 - всегда

        while(i < sizeTempl){                                         // пока не просмотрели весь шаблон
            std::cout << ".................................................\n";
            std::cout << "Обрабатка: шаблон[" << j << "] = " <<  templP[j];
            std::cout << " и шаблон[" << i << "] = " <<  templP[i] << "\n";

            if(templP[i] == templP[j]){                               //если символ повторяется
                std::cout << "Cимволы совпадают!\n";
                std::cout << "prefixFunc[" << i << "] = " << j + 1 << "\n";

                prefixFunc[i] = j + 1;                                // фиксируем значение в префикс-функции
                i++;                                                  // и сдвигаемся
                j++;
            }
            else{
                std::cout << "Cимволы не совпадают!\n";
                if(!j){                                               // j == 0
                    std::cout << "prefixFunc[" << i << "] = 0\n";

                    prefixFunc[i] = 0;
                    i++;
                }
                else{                                                 // если j не указывает на начало суффикса
                    std::cout << "j = prefixFunc[" << j - 1 << "] = " << prefixFunc[j-1] << "\n";

                    j = prefixFunc[j - 1];                            // присваеваем значения префикс-функции предыдущего
                                                                      // символа на который указывает j
                }
            }
        }

        std::cout << "\nПолученная префикс-функция:\n";
        printPrefixFunc();
    }


    void patternSearch(std::string block, int index){                               // поиск вхождений шаблона в текст
                                                                                    // T - текст P - шаблон
        int indexT = 0;                                                             // стартуем из начала текста и шаблона
        int indexP = 0;

        std::cout << "\nШаблон :";
        printPattern();
        std::cout << "Блок: ";
        printBlock(index);

        while(indexT != block.length()){                                            // пока не просмотрели весь текст
            std::cout << "\n\n";
            std::cout << "Символы для сравнения: шаблон[" << indexP << "] = " << templP[indexP];
            std::cout << " и блок[" << indexT << "] = " << block[indexT];
            std::cout << "\n";

            if(templP[indexP] == block[indexT]){                                    // если нашли совпадение
                std::cout << "Символы совпадают!\n";
                indexT++;
                indexP++;
                if(indexP == sizeTempl) {                                           //не дошли ли до конца шаблона?
                    std::cout << "!!!Нашли вхождение шаблона в текст: ";
                    std::cout << indexT+(sizeBlock * index) - (sizeTempl);
                    std::cout << "\n\n";
                    if(find(indexT+(sizeBlock * index) - (sizeTempl))) std::cout << "Вхождение уже записано\n";
                    else
                        indexEntry.push_back(indexT+(sizeBlock * index) - (sizeTempl)); //фиксируем индекс вхождения найденного шаблона
                }                                                                   //учитываем, что индексация в блоках не соответствует
            }                                                                       //индексации в исходном тексте
            else{
                std::cout << "Символы не совпадают!\n";

                if(!indexP)                                                         //indexP == 0
                    indexT++;                                                       //сдвигаемся по тексту
                else
                    indexP = prefixFunc[indexP - 1];                                //перемещаемся на элемент с индексом = префикс-функции предыдущего элемента
            }
        }
    }


    void alghorithmKMP(){                                                           //для вызова необходимых методов в строгом порядке
        trainingText();                                                             //1 этап - разрезание текста на блоки
        prefixFunc.resize(templP.length() - 1);                                     //для префикс-функции
        createPrefixFunc();                                                         //2 этап - формирования массива префикс-функции

        std::cout << "Этап 3 - Поиск вхождений шаблона в текст";

        for(int i = 0; i < quantityBlock; i++){
            std::cout << "\nОбрабатываемый блок: " << i + 1 << ") ";
            printBlock(i);
            patternSearch(block[i], i);                                             //3 этап - поиск вхождений шаблона в текст
        }

        std::cout << "\n";
        std::cout << "\nРЕШЕНИЕ:\n";
        printAnswer();                                                              //печать результата
    }


public:
    void readData(int task_){
        task = task_;                                                               //номер задания
        std::string P;                                                              //шаблон
        std::string T;                                                              //текст

        if(!task){                                                                  //поиск вхождений
            std::cout << "Пожалуйста, введите шаблон.\n";
            std::cin >> P;
            std::cout << "Пожалуйста, введите текст.\n";
            std::cin >> T;
        }
        else{                                                                       //определение сдвига
            std::cout << "Пожалуйста, введите A.\n";
            std::cin >> T;
            std::cout << "Пожалуйста, введите B.\n";
            std::cin >> P;

            if(checkSecond(T, P, false) != 1){                                      //проверка частных случаев
                std::cout << checkSecond(T, P, true);
                std::cout << "\n";
                exit(0);
            }
        }

        setTemplP(P);                                                               //инкапсуляция
        setTextT(T);

        sizeTempl = (int)templP.length();                                           //для удобства
        sizeText = (int)textT.length();


        if(!task && checkFirst(false) != 1){                                        //проверка частных случаев
                   std::cout << checkFirst(true);
                   std::cout << "\n";
                   exit(0);
               }

        int quantBl;
        std::cout << "Введите количество блоков, на которое хотите разделить текст.\n";
        std::cin >> quantBl;
        int flag = 0;

        while(!flag){                                                               //проверка кол-ва блоков
            if(quantBl <= 0 ||  ( sizeTempl > 1 && quantBl > (sizeText / (sizeTempl - 1)))
               ||  ( sizeTempl == 1 &&  quantBl > sizeText )){
                    std::cout << "Некорректное количество блоков.\n";
                    std::cout << "Попробуйте снова.\n";
                    std::cin >> quantBl;
            }
            else{
                    flag = 1;
            }
        }

        setQuantBlock(quantBl);

        alghorithmKMP();                                                            //вызов алгоритма
    }

    void printText(){
        for(int i = 0; i < textT.length(); i++){
            std::cout << textT[i];
        }std::cout << "\n";
    }

    void printBlock(int index){
        std::cout << block[index];
    }

    void printPattern(){
        for(int i = 0; i < templP.length(); i++){
            std::cout << templP[i];
        }std::cout << "\n";
    }

    void printPrefixFunc(){
        for(int i = 0; i < prefixFunc.size(); i++){
            std::cout <<"Шаблон[" << i << "] = " << templP[i] << " --> prefixFunc[" << i << "] = " << prefixFunc[i] << "\n";
        }std::cout << "\n";
    }

    void printAnswer(){                                                             //результат
        if(!task){                                                                  //поиск вхождений
            std::cout << "Вхождения шаблона в текст ";

            if(indexEntry.empty())
                std::cout << "не найдены: -1\n" ;

            else{
                std::cout << "найдены: " ;

                for(int k = 0; k < indexEntry.size(); k++){

                    if(indexEntry.size() - 1 != k)
                        std::cout << indexEntry[k] << ",";
                    else
                        std::cout << indexEntry[k] << "\n";
                    }
            }
        }
        else{                                                                       //выявление сдвига

            if(indexEntry.empty())
                std::cout << "A не является циклическим сдвигом B. Индекс начала строки В в А: -1\n";

            else{
                std::cout << "A является циклическим сдвигом B. Индекс начала строки В в А: ";
                std::cout << indexEntry[0];
                 std::cout << "\n";
            }
        }
    }

};

int main(int argc, const char * argv[]) {
    setlocale(LC_ALL, "rus");
    char solution = '0';
    int exit = 0;

    std::cout << "Введите a, если вы хотите вывести все вхождения шаблона Р в текст Т.\n";
    std::cout << "Введите b, если вы хотите определить является ли А циклическим сдвигом в В.\n";
    std::cin >> solution;

    do{
        switch(solution){
            case 'a':
                exit = 0;
                break;

            case 'b':
                exit = 0;
                break;

            default:
                std::cout << "Ошибка: некорректная команда - попробуйте снова.\n";
                std::cin >> solution;
                exit = 1;
        }
    }while(exit == 1);

    KMP start;                                      //класс, реализующий КМП
    start.readData((int)solution - 97);
    return 0;
}
