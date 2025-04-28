// ProstoCshka.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

// Импортим (подключаем) библиотеки
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Создаём необходимые функции для вычислений
double vuchislenie(char* vurazhenie); // функция, которая будет проводить вычисления
double log_factor(char* vurazhenie, int* num); // Функция, которая обрабатывает числа и скобки
double plus_minus(char* vurazhenie, int* num); // функция, которая обрабатывает операторы + и -
double mult_delenie(char* vurazhenie, int* num); // функция, которая обрабатывает операторы * и /
void skip_space(char* vurazhenie, int* num); // игнорим пробелы

// Мэйн - точка отсчёта, мэйн - наше всё
int main(void)
{
    // Сюда будет подано выражение
    char vurazhenie[256];
    printf("Enter expression for evulating: \n");
    fgets(vurazhenie, sizeof(vurazhenie), stdin);
    // обработка переноса строки
    char* ptr_n = strrchr(vurazhenie, '\n');
    if (ptr_n != NULL) {
        *ptr_n = '\0';
    }

    // Выводим результат вычисления до 4 знаков после запятой
    double resultat = vuchislenie(vurazhenie);
    printf("Result: %.4lf", resultat);

    return 0;
}

// Проводоим вычисление выражения
double vuchislenie(char* vurazhenie)
{
    // начианем с первого символа строки
    int i = 0;
    double total = plus_minus(vurazhenie, &i);
    // Игнорируем пробелы
    skip_space(vurazhenie, &i);
    // Если не обнаружилось конца строки, выводим предупреждение
    if (vurazhenie[i] != '\0') {
        fprintf(stderr, "Error! Not found symbol endline *_*. Last symbol: '%c'\n", vurazhenie[i]);
        exit(EXIT_FAILURE);
    }

    // Возвращаем полученное выражение
    return total;
}

// Пропускаем пробел или таблуляцию, переходим к следующему элементу строки
void skip_space(char* vurazhenie, int* num)
{
    while (vurazhenie[*num] == ' ' || vurazhenie[*num] == '\t') {
        (*num)++;
    }
}

// Пошла жара
double plus_minus(char* vurazhenie, int* num)
{
    // Создаем переменную для подсчёта, ей присваиваем вызов следющей функции
    double total = mult_delenie(vurazhenie, num);
    // Пока (если) в строке есть + и (или) -
    while (true) {
        // Пропускаем пробел и переходим к следующему элементу
        skip_space(vurazhenie, num);
        if (vurazhenie[*num] == '+') {
            (*num)++;
            // Проводим сложение
            total += mult_delenie(vurazhenie, num);
        }
        // Аналогично с минусом
        else if (vurazhenie[*num] == '-') {
            (*num)++;
            total -= mult_delenie(vurazhenie, num);
        }
        // Выходим из цикла, если нету больше + и -
        else {
            break;
        }
    }
    // Возвращаем полученное значение
    return total;
}
// Далее
double mult_delenie(char* vurazhenie, int* num)
{
    // Ссылаемся на следующую функцию в новой созданной переменной
    double total = log_factor(vurazhenie, num);
    // Пока есть * или / переходим к следующему элементу
    while (true) {
        skip_space(vurazhenie, num);
        if (vurazhenie[*num] == '*') {
            (*num)++;
            total *= log_factor(vurazhenie, num);
        }
        else if (vurazhenie[*num] == '/') {
            (*num)++;
            double delitel = log_factor(vurazhenie, num);
            // Если деление на ноль, выводим ошибку
            if (delitel == 0) {
                fprintf(stderr, "Error: Division by zero");
                exit(EXIT_FAILURE);
            }
            total /= delitel;
        }
        // Брикаем цикл, если не осталось * или /
        else {
            break;
        }
    }

    // Возвращаем подсчёт
    return total;
}
// Функция для проверки элемента на число
int proverka_chisla(int c)
{
    if (c >= '0' && c <= '9')
        return 1;

    else
        return 0;
}

// Главная функция
double log_factor(char* vurazhenie, int* num)
{
    // Пропускаем пробелы. Определяем знак числа для положителного и отрицательного вычисления
    skip_space(vurazhenie, num);
    int znak = 1;
    if (vurazhenie[*num] == '+') {
        (*num)++;
        skip_space(vurazhenie, num);
    }
    else if (vurazhenie[*num] == '-') {
        znak = -1;
        (*num)++;
        skip_space(vurazhenie, num);
    }

    // Создаем переменную для подсчёта
    double total;
    // Обрабатываем открывающуюся скобку
    if (vurazhenie[*num] == '(') {
        // Переходим к след. элементу
        (*num)++;
        // Вызываем функцию для нахождения плюса или минуса в скобке, которая вызывает функция на * и /, 
        // которая собой вызывает функция на обработку чисел и скобок
        total = plus_minus(vurazhenie, num);
        skip_space(vurazhenie, num);

        // Дошли до закрывающей скобки, переходим к след. элементу
        if (vurazhenie[*num] == ')') {
            (*num)++;
        }
        // Если не дошли, выводим ошибку
        else {
            fprintf(stderr, "Error: A closing parenthesis was expected\n");
            exit(EXIT_FAILURE);
        }
    }

    // Проверяем, что элемент это число
    else if (proverka_chisla(vurazhenie[*num])) {
        // Создаем временный массив, в котором будем хранить получившуюся строку без пробелов
        char buffer[256];
        int i = 0;
        // Пока элемент является числом, записываем его в массив
        while (proverka_chisla(vurazhenie[*num]) || vurazhenie[*num] == '.') {
            buffer[i] = vurazhenie[*num];
            i++;
            (*num)++;
        }
        // Заканчиваем строку
        buffer[i] = '\0';
        // Выводим числа из массива
        total = atof(buffer);
    }
    // Доп. проверка
    else {
        fprintf(stderr, "Error: A number or an opening parenthesis was expected\n");
        exit(EXIT_FAILURE);
    }

    // Возвращаем сформировавшееся число и определяем его знак
    return total * znak;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
