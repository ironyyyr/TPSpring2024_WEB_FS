/*
Дан массив строк. Количество строк не больше 105. Отсортировать массив методом
поразрядной сортировки MSD по символам. Размер алфавита - 256 символов.
Последний символ строки = ‘\0’.
*/

#include <iostream>
#include <string>

void countingSort(std::string* stringArray, int left, int right, int pos);

int main() {
    std::string* stringArray = new std::string[100000];
    std::string myString;

    int length = 0;

    while (std::cin >> myString) {
        stringArray[length] = myString;
        length++;
    }

    countingSort(stringArray, 0, length, 0);

    for (int i = 0; i < length; ++i) {
        std::cout << stringArray[i] << std::endl;
    }

    delete[] stringArray;

    return 0;
}

void countingSort(std::string* stringArray, int left, int right, int pos) {
    if (right - left <= 1) {
        return;
    }

    int* buffArray = new int[257]();

    for (int i = left; i < right; ++i) {
        buffArray[stringArray[i][pos]]++;
    }

    for (int i = 1; i < 256; ++i) {
        buffArray[i] += buffArray[i - 1];
    }

    std::string* tmpStringArray = new std::string[right - left];

    for (int i = left; i < right; ++i) {
        buffArray[stringArray[i][pos]]--;
        tmpStringArray[buffArray[stringArray[i][pos]]] = stringArray[i];
    }

    for (int i = left; i < right; ++i) {
        stringArray[i] = tmpStringArray[i - left];
    }

    for (int i = 1; i < 256; ++i) {
        if (buffArray[i + 1] - buffArray[i] <= 1) {
            continue;
        }
        countingSort(stringArray, buffArray[i] + left, buffArray[i + 1] + left,
                     pos + 1);
    }

    delete[] buffArray;
    delete[] tmpStringArray;
}
