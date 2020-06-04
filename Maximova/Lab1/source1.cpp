#include <iostream> //доделать оформление+отчёт
#include <fstream>
#include <cmath>
#include <stack>
#include <ctime>
#define unInt unsigned int
using namespace std;

unInt g_covered;								//площадь покрытия
unInt g_area;									//площадь стола
unInt g_addColor;

struct SQUARE {
	unInt x, y, size;						//координаты верхнего левого угла квадрата, длина его стороны
};

unInt** makeArray(unInt& N);				//функция для создания двумерного динамического массива?
void initArray(unInt& N, unInt** arr);		//инициализация массива
void printArray(unInt& N, unInt** arr);		//печать массива
void copyArray(unInt** arr1, unInt** arr2, unInt& N);
void deleteArray(unInt& N, unInt** arr);	//очищение памяти
void printStack(stack <SQUARE>* a);		  //для печати промежуточных результатов и ответа

void training(unInt N, unInt** table, stack <SQUARE>* a);											//оптимизация
int backTracking(unInt** table, unInt** res, unInt N, stack <SQUARE>* Zcur, stack <SQUARE>* Zopt); //бэктрекинг
bool checkPos(unInt** table, unInt i, unInt j, unInt side, unInt N);
void paintSquare(unInt** table, unInt i, unInt j, unInt side, stack <SQUARE>* a);			// для рисования квадратов
unInt sizeSide(unInt** table, unInt i, unInt j, unInt side, unInt N);
void deleteSquare(unInt i, unInt j, unInt side, unInt** table, stack <SQUARE>* a);			//удаление квадратов

void case1(unInt N, unInt** table, stack <SQUARE>* Zcur);			//кратно 2
void case2(unInt N, unInt** table, stack <SQUARE>* Zcur);			//кратно 3
void case3(unInt N, unInt** table, stack <SQUARE>* Zcur);			//кратно 5

int dataCorrect();

int main() {
	setlocale(LC_ALL, "rus");
	clock_t start, end;				//для подсчета времени работы программы
	unInt exit = 1;
	stack <SQUARE>* curTilling = new stack <SQUARE>;			//текущее замощение стола (координаты, длина сторон)
	stack <SQUARE>* optTilling = new stack <SQUARE>;		//оптимальное замощение стола - оценка сверху

	cout << "\t\t\t\t\033[0;35mУСЛОВИЯ ЗАДАЧИ:\033[0m\n";			//условия задачи
	cout << "   У Вовы много квадратных обрезков доски. Их стороны изменяютнся от 1 до sizeTable-1, и у него есть \n неограниченное число обрезков любого размера.";
	cout << " Но ему очень хочется получить большую столешницу \n - квадрат размера sizeTable. Он может получить ее, собрав из уже имеющихся обрезков(квадратов).\n";
	cout << " Внутри столешницы не должно быть пустот, обрезки не должны выходить за предеы столешницы и не должны \n перекрываться. Кроме того, Вова хочет использовать ";
	cout << "минимально возможное число обрезков.\n";
	cout << "\n   \033[0;35mВХОДНЫЕ ДАННЫЕ:\033[0m\n";
	cout << "   Размер столешницы - одно целое число sizeTable (2<=sizeTable<=30).\n";
	cout << "\n   \033[0;35mВЫХОДНЫЕ ДАННЫЕ:\033[0m\n";
	cout << "   Одно число minCount, задающее минимальное количество обрезков(квадратов), из которых можно \n построить столешницу(квадрат) заданного размера sizeTable.\n";
	cout << " Далее идет minCount строк, каждая из которых содержит три целых числа - координаты левого верхнего \n угла(1<=x, y<=sizeTable) и длину стороны соответствующего квадрата\n";

	while (exit) {		//для повторного использования
		start = clock();
		cout << "\n Введите размер столешницы (sizeTable).\n";
		unInt sizeTable = dataCorrect();				//длина стороны квадрата

		unInt** table = makeArray(sizeTable);					//двумерный массив - для хранения замощения стола
		initArray(sizeTable, table);
		unInt** res = makeArray(sizeTable);					//двумерный массив - для хранения замощения стола
		initArray(sizeTable, res);

		g_covered = 0;					//стол не покрыт
		g_area = pow(sizeTable, 2);

		cout << "Стол, который нужно заполнить.\n";
		printArray(sizeTable, table);
		cout << "\n";

		if (sizeTable % 2 == 0) {														//кратно 2
			cout << "\033[0;33mЧастный случай: размер стола кратен двум.\033[0m\n";
			cout << "Результат:\n";
			case1(sizeTable, table, curTilling);
		}
		else if (sizeTable % 3 == 0) {													//кратно 3
			cout << "\033[0;33mЧастный случай: размер стола кратен трём.\033[0m\n";
			cout << "Результат:\n";
			case2(sizeTable, table, curTilling);
		}
		else if (sizeTable % 5 == 0) {													//кратно 5
			cout << "\033[0;33mЧастный случай: размер стола кратен пяти.\033[0m\n";
			cout << "Результат:\n";
			case3(sizeTable, table, curTilling);
		}
		else {																			//N - простое число
			cout << "\033[0;33mШАГ 1: применение оптимизации.\033[0m\n";
			cout << "\n";
			training(sizeTable, table, curTilling);

			cout << "\033[0;33mШАГ 2: запуск backtracking.\033[0m\n";
			cout << "\n";
			backTracking(table, res, sizeTable, curTilling, optTilling);
		}

		deleteArray(sizeTable, table);
		end = clock();
		cout << "\033[0;32mВремя работы программы: " << (end - start) / CLK_TCK << " сек.\033[0m\n";
		cout << "\033[0;36mЕсли вы хотите продолжить работу с программой, введите любое положительное число, иначе - 0.\033[0m\n";
		exit = dataCorrect();
	}
	system("pause");
	return 0;
}

int dataCorrect() { //функция, проверяющая корректность входных данных
	int number = 0;

	while (!(cin >> number) || number < 0) {
		cout << "\033[0;31mОшибка. Данные некорректны! Ожидается положительное число.\033[0m" << endl;
		cin.clear(); //сброс коматозного состояние cin
		cin.ignore(1000, '\n');
		fflush(stdin); //очистка потока ввода
	}

	cin.get();
	return number;
}

void case1(unInt N, unInt** table, stack <SQUARE>* Zcur) {		//если кратно 2
	unInt wSquare = N / 2;

	paintSquare(table, 0, 0, wSquare, Zcur);		//сохраняем в стек
	printArray(N, table);							//печатаем каждое добавление квадрата

	paintSquare(table, 0, wSquare, wSquare, Zcur);
	printArray(N, table);

	paintSquare(table, wSquare, 0, wSquare, Zcur);
	printArray(N, table);

	paintSquare(table, wSquare, wSquare, wSquare, Zcur);
	printArray(N, table);
	printStack(Zcur);										//результат
}

void case2(unInt N, unInt** table, stack <SQUARE>* Zcur) { 		//кратно 3
	unInt wBigSq = (2 * N) / 3;
	unInt wSmallSq = N - wBigSq;

	paintSquare(table, 0, 0, wBigSq, Zcur);
	printArray(N, table);

	paintSquare(table, 0, wBigSq, wSmallSq, Zcur);
	printArray(N, table);

	paintSquare(table, wSmallSq, wBigSq, wSmallSq, Zcur);
	printArray(N, table);

	paintSquare(table, wBigSq, 0, wSmallSq, Zcur);
	printArray(N, table);

	paintSquare(table, wBigSq, wSmallSq, wSmallSq, Zcur);
	printArray(N, table);

	paintSquare(table, wBigSq, wBigSq, wSmallSq, Zcur);
	printArray(N, table);
	printStack(Zcur);
}

void case3(unInt N, unInt** table, stack <SQUARE>* Zcur) {	  //кратно 5
	unInt wBigSq = (3 * N) / 5;
	unInt wMedSq = N - wBigSq;
	unInt wSmallSq = wMedSq / 2;

	paintSquare(table, 0, 0, wBigSq, Zcur);
	printArray(N, table);

	paintSquare(table, 0, wBigSq, wMedSq, Zcur);
	printArray(N, table);

	paintSquare(table, wBigSq, 0, wMedSq, Zcur);
	printArray(N, table);

	paintSquare(table, wBigSq, wMedSq, wMedSq, Zcur);
	printArray(N, table);

	paintSquare(table, wMedSq, wBigSq, wSmallSq, Zcur);
	printArray(N, table);

	paintSquare(table, wMedSq, N - wSmallSq, wSmallSq, Zcur);
	printArray(N, table);

	paintSquare(table, wBigSq, N - wSmallSq, wSmallSq, Zcur);
	printArray(N, table);

	paintSquare(table, N - wSmallSq, N - wSmallSq, wSmallSq, Zcur);
	printArray(N, table);
	printStack(Zcur);
}

void training(unInt N, unInt** table, stack <SQUARE>* a) { //для оптимально и текущего замощений
	cout << "\033[0;32mОптимизация - рисование первых трёх квадратов.\033[0m\n";
	unInt i, j;
	unInt side1 = (N / 2) + 1;				//сторона большого квадрата
	unInt side2 = N - side1;					//два средний квадрата

	cout << "\033[0;36mРисование большого квадрата:\033[0m\n";
	paintSquare(table, N / 2, N / 2, side1, a);	//рисуем большой квадрат
	printArray(N, table);
	cout << "\n";

	cout << "\033[0;36mРисование левого соседнего квадрата:\033[0m\n";
	paintSquare(table, side1, 0, side2, a);	//рисуем средний квадрата
	printArray(N, table);
	cout << "\n";

	cout << "\033[0;36mРисование правого соседнего квадрата:\033[0m\n";
	paintSquare(table, 0, side1, side2, a);
	printArray(N, table);
	cout << "\n";
}

int backTracking(unInt** table, unInt** res, unInt N, stack <SQUARE>* Zcur, stack <SQUARE>* Zopt) {
	ofstream fout("Test.txt", ios_base::app);
	unInt sizeSq = N - 1;;
	unInt check;
	unInt i, j;
	unInt iter = 0; 
	unInt size;
	unInt minCount = N * N + 1;
	unInt flag = 0;

	while (Zcur->size() != g_area) {						        //пока стол не разбит на одиночные квадраты
		iter++;
		cout << "Итерация - " << iter << "\n";
		fout << "Итерация - " << iter << "\n";

		if (g_area != g_covered && !flag) {				        	//пока не заполнили
			sizeSq = (N / 2) + 1;								//sizeSq - начинаем поиск с максимальной стороны

			cout << "\033[0;36mСтартовая длина стороны для добавляемого квадрата " << sizeSq << ".\033[0m\n";
			fout << "Стартовая длина стороны для добавляемого квадрата " << sizeSq << ".\n";

			check = Zcur->size();						//для проверки: добавился ли квадрат?
			for (i = 0; i < N / 2 + 1; i++) {
				for (j = 0; j < N / 2 + 1; j++) {

					if (Zcur->size() >= minCount) {		//если текущее разбиение, даёт результат хуже
						flag = 1;
						break;
					}

					if (!table[i][j]) {					//если есть свободные позиции
						cout << "\033[0;32mНайдена свободная позиция с координатами х = " << i + 1 << " y = " << j + 1 << ".\033[0m\n";	//печать промежуточных значений
						fout << "Найдена свободная позиция с координатами х = " << i + 1 << " y = " << j + 1 << ".\n";
						size = sizeSide(table, i, j, sizeSq, N);
						cout << "\033[0;32mДобавление квадрата со стороной " << size << ".\033[0m\n";
						fout << "Добавление квадрата со стороной " << size << ".\n";

						paintSquare(table, i, j, size, Zcur);		//рисуем и добавляем в стек
						//печать промежуточных значений
						cout << "\033[0;36mПолученное замощение:\033[0m\n";
						fout << "Полученное замощение:";
						printArray(N, table);
						cout << "\n";
					}
				}
			}
			if (check == Zcur->size()) {
				sizeSq--;
				cout << "\033[0;31mУменьшение стартовой длины стороны квадрата = " << sizeSq << ".\033[0m\n";
				fout << "Уменьшение стартовой длины стороны квадрата = " << sizeSq << ".\n";
			}
		}

		else if (g_area == g_covered || flag) {				//если нет свободных позиций
			if (flag) cout << "\033[0;31mТекущее замощение превышает минимальное - прекращаем обработку данного случая.\033[0m\n";
			flag = 0;

			if (Zcur->size() < minCount) {		       //если нашли лучшее замощение
				*Zopt = *Zcur;
				minCount = Zcur->size();
				copyArray(table, res, N);
				cout << "\033[0;32mНовое оптимальное замощение: " << minCount << ".\033[0m\n";
				fout << "Новое оптимальное замощение: " << minCount << ".\n";
			}

			while (!Zcur->empty() && Zcur->top().size == 1) {		//удаляем все единичные блоки
				if (Zcur->top().x == 1 && Zcur->top().y == 1) {		//крайний случай
					deleteSquare(Zcur->top().x - 1, Zcur->top().y - 1, Zcur->top().size, table, Zcur);
					//печать результата
					cout << "\033[0;31mПоследний шаг: удаление квадрата с позиции: x = " << Zcur->top().x << ", y = " << Zcur->top().y << ".\033[0m\n";
					fout << "Последний шаг: удаление квадрата с позиции: x = " << Zcur->top().x << ", y = " << Zcur->top().y << ".\n";
					printArray(N, table);
					cout << "\n";
					cout << "\033[0;32mРЕЗУЛЬТАТ:\033[0m\n";
					fout << "РЕЗУЛЬТАТ:\n";
					printArray(N, res);
					cout << "\n";
					printStack(Zopt);

					return 0;			//завершение
				}

				cout << "\033[0;31mУдаление квадрата с позиции: x = " << Zcur->top().x << ", y = " << Zcur->top().y << ".\033[0m\n";
				fout << "Удаление квадрата с позиции: x = " << Zcur->top().x << ", y = " << Zcur->top().y << ".\n";

				deleteSquare(Zcur->top().x - 1, Zcur->top().y - 1, Zcur->top().size, table, Zcur);

				printArray(N, table);
				cout << "\n";
			}
			if (!Zcur->empty()) {									//если есть, что удалять
				unInt a = Zcur->top().x - 1;	                	//сохраняем значения
				unInt b = Zcur->top().y - 1;
				sizeSq = Zcur->top().size - 1;

				cout << "\033[0;31mУдаление квадрата с позиции x = : " << Zcur->top().x << ", y = " << Zcur->top().y << ".\033[0m\n";
				fout << "Удаление квадрата с позиции x = : " << Zcur->top().x << ", y = " << Zcur->top().y << ".\n";

				deleteSquare(a, b, Zcur->top().size, table, Zcur); //удаляем и создаем там же с размером на -1

				printArray(N, table);				//печать промежуточных значений
				cout << "\n";
				fout << "\n";
				cout << "\033[0;32mДобавление квадрата на позицию: x = " << a + 1 << ", y = " << b + 1 << " со стороной: " << sizeSq << "\033[0m\n";
				fout << "Добавление квадрата на позицию: x = " << a + 1 << ", y = " << b + 1 << " со стороной: " << sizeSq << "\n";

				paintSquare(table, a, b, sizeSq, Zcur);		//рисуем и добавляем в стек

				printArray(N, table);
				cout << "\n";
				fout << "\n";
			}
		}
	}
}

void deleteSquare(unInt i, unInt j, unInt side, unInt** table, stack <SQUARE>* a) {			//для удаления квадратов
	cout << "\033[0;33mВызов функции удаления.\033[0m\n";
	g_covered -= pow(side, 2);			//уменьшаем плоащадь покрытия
	cout << "На данный момент покрыто " << (double)g_covered / ((double)g_area / 100) << " %.\n";

	for (unInt x = i; x < i + side; x++) {
		for (unInt y = j; y < j + side; y++) {
			table[x][y] = 0;
		}
	}
	a->pop();			//удаляем из стека
}

void paintSquare(unInt** table, unInt i, unInt j, unInt side, stack <SQUARE>* a) {
	ofstream fout("Test.txt", ios_base::app);
	fout << "Вызов функции рисования.\n";
	cout << "\033[0;33mВызов функции рисования.\033[0m\n";
	g_covered += pow(side, 2);
	fout << "На данный момент покрыто " << (double)g_covered / ((double)g_area / 100) << " %.\n";
	cout << "На данный момент покрыто " << (double)g_covered / ((double)g_area / 100) << " %.\n";
	unInt color = a->size() + 1;

	for (unInt x = i; x < i + side; x++) {
		for (unInt y = j; y < j + side; y++) {
			table[x][y] = color;
			g_addColor = color;
		}
	}

	a->push(SQUARE{ i + 1, j + 1, side });
	fout.close();
}

unInt sizeSide(unInt** table, unInt i, unInt j, unInt side, unInt N) { //какой квадрат можно вписать
	int flag = 0;
	for (int k = side; k > 0; k--) {
		if (checkPos(table, i, j, k, N)) {
			return k;
			break;
		}
	}
}

bool checkPos(unInt** table, unInt i, unInt j, unInt side, unInt N) {		//можно ли нарисовать квадрат из выбранной координаты
	unInt flag = 0;

	if ((i + side) <= N && (j + side) <= N) {		//не заходит за границы
		for (unInt x = i; x < i + side; x++) {
			for (unInt y = j; y < j + side; y++) {
				if (table[x][y]) {
					flag++;
				}
			}
		}
	}

	else {
		return flag;
	}

	if (flag) return false;		//возвращаем результат
	else return true;
}

unInt** makeArray(unInt& N) {	//N - длина стороны стола
	unInt** arr = new unInt * [N];

	for (unInt i = 0; i < N; i++) {
		arr[i] = new unInt[N];
	}
	return arr;
}

void initArray(unInt& N, unInt** arr) {			//инициализация массива нулями
	for (unInt i = 0; i < N; i++) {
		for (unInt j = 0; j < N; j++) {
			arr[i][j] = 0;
		}
	}
}

void printArray(unInt& N, unInt** arr) {
	ofstream fout("Test.txt", ios_base::app);

	for (unInt i = 0; i < N; i++) {
		for (unInt j = 0; j < N; j++) {
			if (arr[i][j] == g_addColor) {
				cout << "\033[0;327m" << arr[i][j] << "\033[0m" << " ";
				fout << arr[i][j] << " ";
			}
			else {
				cout << arr[i][j] << " ";
				fout << arr[i][j] << " ";
			}
		}
		cout << endl;
		fout << endl;
	}

	fout.close();
}

void copyArray(unInt** arr1, unInt** arr2, unInt& N) {
	for (unInt i = 0; i < N; i++) {
		for (unInt j = 0; j < N; j++) {
			arr2[i][j] = arr1[i][j];
		}
	}
}

void printStack(stack <SQUARE>* a) {
	unInt size = a->size();
	cout << "\033[0;32mМинимальное количество квадратов: \033[0m" << size << "\n";
	cout << "\033[0;32mКоординаты верхних углов квадратов и длины их сторон:" << "\033[0m\n";

	for (unInt i = 0; i < size; i++) {
		cout << a->top().x << " " << a->top().y << " " << a->top().size << endl;
		a->pop();
	}
}

void deleteArray(unInt& N, unInt** arr) {
	for (unInt i = 0; i < N; i++) {
		delete[] arr[i];
	}
	delete[] arr;
}
