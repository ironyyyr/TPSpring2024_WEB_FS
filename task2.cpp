/*Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел
B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс k
минимального элемента массива A, равного или превосходящего B[i]: A[k] >= B[i].
Если такого элемента нет, выведите n. n, m ≤ 10000. Требования:  Время работы
поиска k для каждого элемента B[i]: O(log(k)). Внимание! В этой задаче для
каждого B[i] сначала нужно определить диапазон для бинарного поиска размером
порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный
поиск. Формат входных данных. В первой строчке записаны числа n и m. Во второй и
третьей массивы A и B соответственно.
*/

#include <iostream>

int* makeAndFillMatrix(int numOfCells);
int expSearch(int* matrixN, int sizeOfMatrix, int currElem);
int binarySearch(int* matrix, int compValue, int sizeOfMatrix, int lowerBorder,
                 int upperBorder);

int main() {
    int N, M;
    std::cin >> N >> M;

    int* matrixN = makeAndFillMatrix(N);
    int* matrixM = makeAndFillMatrix(M);

    for (int i = 0; i < M; ++i) {
        int res = expSearch(matrixN, N, matrixM[i]);
        if (res == -1) {
            std::cout << N << ' ';
        } else {
            std::cout << res << ' ';
        }
    }

    delete matrixM;
    delete matrixN;

    return 0;
}

int* makeAndFillMatrix(int numOfCells) {
    int* matrix = new int[numOfCells];

    for (int i = 0; i < numOfCells; ++i) {
        std::cin >> matrix[i];
    }

    return matrix;
}

int expSearch(int* matrixN, int sizeOfMatrix, int compValue) {
    int upperBorder = 1;

    while (matrixN[upperBorder] < compValue && upperBorder < sizeOfMatrix) {
        upperBorder *= 2;
    }

    int lowerBorder = upperBorder / 2;

    if (upperBorder > sizeOfMatrix) {
        upperBorder = sizeOfMatrix;
    }

    return binarySearch(matrixN, compValue, sizeOfMatrix, lowerBorder,
                        upperBorder);
}

int binarySearch(int* matrix, int compValue, int sizeOfMatrix, int lowerBorder,
                 int upperBorrder) {
    while (lowerBorder < upperBorrder) {
        int middle = (upperBorrder + lowerBorder) / 2;
        if (matrix[middle] < compValue) {
            lowerBorder = middle + 1;
        } else {
            upperBorrder = middle;
        }
    }

    return (lowerBorder == sizeOfMatrix || matrix[lowerBorder] < compValue)
               ? -1
               : lowerBorder;
}