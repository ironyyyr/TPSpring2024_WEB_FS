/*Реализовать очередь с динамическим зацикленным буфером (на основе
динамического массива). Требования: Очередь должна быть реализована в виде
класса.*/

#include <cassert>
#include <iostream>

class DynArray {
   public:
    DynArray()
        : buffer(new int[1]),
          sizeOfArray(1),
          realNumOfElements(0),
          head(0),
          tail(0) {}

    ~DynArray() { delete[] buffer; }

    DynArray(const DynArray& other) = delete;
    DynArray& operator=(const DynArray& other) = delete;

    //Пересоздать массив, если больше нет места
    void IncreaseOfArray() {
        sizeOfArray = std::max(sizeOfArray * 2, 2);
        int* tmpBuffer = new int[sizeOfArray];

        for (int i = 0; i < realNumOfElements; ++i) {
            tmpBuffer[i] = buffer[(head + i) % (sizeOfArray / 2)];
        }

        head = 0;
        tail = realNumOfElements;
        delete[] buffer;
        buffer = tmpBuffer;
    }

    //Уменьшить массив в 2 раза, если есть возможность для сохранения памяти
    void DecreaseOfArray() {
        int* tmpBuffer = new int[sizeOfArray / 2];

        for (int i = 0; i < realNumOfElements; ++i) {
            tmpBuffer[i] = buffer[(head + i) % sizeOfArray];
        }

        head = 0;
        tail = realNumOfElements;
        delete[] buffer;
        buffer = tmpBuffer;
        sizeOfArray /= 2;
    }

    void Push(int element) {
        if (sizeOfArray == realNumOfElements) {
            IncreaseOfArray();
        }

        assert(realNumOfElements < sizeOfArray && buffer != 0);
        buffer[tail++ % sizeOfArray] = element;
        realNumOfElements++;
    }

    int Pop() {
        assert(buffer != 0);

        if (IsEmpty()) {
            return -1;
        }

        int tmpElem = buffer[head % sizeOfArray];
        head++;
        realNumOfElements--;
        if (sizeOfArray > 1 && sizeOfArray / 2 > realNumOfElements) {
            DecreaseOfArray();
        }
        return tmpElem;
    }

    bool IsEmpty() { return realNumOfElements == 0; }

   private:
    int* buffer = new int[1];
    int sizeOfArray;
    int realNumOfElements;
    int head;
    int tail;
};

int main() {
    DynArray dynArray;

    int numOfCommands = 0;
    std::cin >> numOfCommands;

    for (int i = 0; i < numOfCommands; ++i) {
        int operation = 0, value = 0;
        std::cin >> operation >> value;

        switch (operation) {
            case 2: {
                int tmpVal = dynArray.Pop();
                if (tmpVal != value) {
                    std::cout << "NO" << std::endl;
                    return 0;
                }
                break;
            }

            case 3: {
                dynArray.Push(value);
                break;
            }
        }
    }
    std::cout << "YES" << std::endl;
    return 0;
}
