/*
Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию
Partition реализуйте методом прохода двумя итераторами от конца массива к
началу.
*/

#include <cmath>
#include <iostream>
#include <random>

template <typename T, typename Comparator = std::less<T>>
int partition(T* array, int left, int right, Comparator cmp = Comparator()) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> destrib(left, right);
    int randomIndex = destrib(gen);

    if (right - left < 0) {
        return 0;
    }

    std::swap(array[left], array[randomIndex]);

    int i = right;
    for (int j = right; j > left; --j) {
        if (cmp(array[left], array[j])) {
            std::swap(array[i], array[j]);
            --i;
        }
    }
    std::swap(array[i], array[left]);
    return i;
}

template <typename T, typename Comparator = std::less<T>>
T kthStatistic(T* array, int k, int left, int right,
               Comparator cmp = Comparator()) {
    while (1) {
        int pivotPos = partition(array, left, right, cmp);

        if (pivotPos == k) {
            return array[pivotPos];
        }

        if (pivotPos > k) {
            right = pivotPos - 1;
        }

        if (pivotPos < k) {
            left = pivotPos + 1;
        }
    }
}

int main() {
    int n = 0;
    std::cin >> n;

    int* array = new int[n];

    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }

    std::cout << kthStatistic(array, n / 10, 0, n - 1) << std::endl;
    std::cout << kthStatistic(array, n / 2, 0, n - 1) << std::endl;
    std::cout << kthStatistic(array, n * 9 / 10, 0, n - 1) << std::endl;

    delete[] array;

    return 0;
}