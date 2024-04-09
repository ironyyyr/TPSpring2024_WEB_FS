/*
На числовой прямой окрасили N отрезков. Известны координаты левого и правого
концов каждого отрезка (Li и Ri). Найти сумму длин частей числовой прямой,
окрашенных ровно в один слой
*/

#include <iostream>

struct Point {
    Point() : value(0), layerStart(0) {}

    Point(int value, int layerStart) : value(value), layerStart(layerStart) {}

    int value, layerStart;
};

class PointComparator {
   public:
    PointComparator() {}

    bool operator()(const Point& point1, const Point& point2) {
        return point1.value < point2.value;
    }
};

template <typename T, typename Comparator>
void MergeSort(T* array, int length, Comparator cmp = Comparator()) {
    if (length <= 1) {
        return;
    }

    int firstLength = length / 2;
    int secondLength = length - firstLength;
    MergeSort(array, firstLength, cmp);
    MergeSort(array + firstLength, secondLength, cmp);
    T* resArray = new T[length];
    Merge(array, firstLength, array + firstLength, secondLength, resArray, cmp);
    memcpy(array, resArray, sizeof(T) * length);
    delete[] resArray;
}

template <typename T, typename Comparator>
void Merge(T* arrayFirstPart, int firstLength, T* arraySecondPart,
           int secondLength, T* resArray, Comparator cmp) {
    int leftIndex = 0;
    int rightIndex = 0;
    int resultIndex = 0;

    while (leftIndex < firstLength && rightIndex < secondLength) {
        if (cmp(arrayFirstPart[leftIndex], arraySecondPart[rightIndex])) {
            resArray[resultIndex++] = arrayFirstPart[leftIndex++];
        } else {
            resArray[resultIndex++] = arraySecondPart[rightIndex++];
        }
    }

    while (leftIndex < firstLength) {
        resArray[resultIndex++] = arrayFirstPart[leftIndex++];
    }

    while (rightIndex < secondLength) {
        resArray[resultIndex++] = arraySecondPart[rightIndex++];
    }
}

int main() {
    int sizeOfArray;
    int length = 0;
    std::cin >> sizeOfArray;

    Point* myPointArray = new Point[sizeOfArray * 2];

    for (int i = 0; i < sizeOfArray * 2; ++i) {
        std::cin >> myPointArray[i].value;

        if (i % 2) {
            myPointArray[i].layerStart = -1;
        } else {
            myPointArray[i].layerStart = 1;
        }
    }

    MergeSort(myPointArray, sizeOfArray * 2, PointComparator());

    int width = 0;
    int singleLayerStart = myPointArray[0].value;
    int singleLayerEnd = myPointArray[0].value;
    int flagOfPrevWidth = 0;

    for (int i = 0; i < sizeOfArray * 2; ++i) {
        width += myPointArray[i].layerStart;
        if (width == 1) {
            flagOfPrevWidth = 1;
            singleLayerStart = myPointArray[i].value;
        } else if (width < 1 || width > 1) {
            singleLayerEnd = myPointArray[i].value;
            if (flagOfPrevWidth == 1) {
                length += singleLayerEnd - singleLayerStart;
            }
            flagOfPrevWidth = 0;
        }
    }

    std::cout << length;

    delete[] myPointArray;

    return 0;
}