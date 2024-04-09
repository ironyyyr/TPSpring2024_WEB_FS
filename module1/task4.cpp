/*В операционной системе Technux есть планировщик процессов.
Каждый процесс характеризуется:
приоритетом P
временем, которое он уже отработал t
временем, которое необходимо для завершения работы процесса T
Планировщик процессов выбирает процесс с минимальным значением P * (t + 1),
выполняет его время P и кладет обратно в очередь процессов. Если выполняется
условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол-во переключений процессора.
Формат входных данных:  Сначала вводится кол-во процессов. После этого процессы
в формате P T Формат выходных данных: Кол-во переключений процессора.
*/

#include <cassert>
#include <iostream>

struct Process {
    Process() : priority(0), workedTime(0), neededTime(0) {}

    Process(int priority, int workedTime, int neededTime)
        : priority(priority), workedTime(workedTime), neededTime(neededTime) {}

    int priority, workedTime, neededTime;

    bool operator<(const Process& other) const {
        return (priority * (neededTime + 1) <
                other.priority * (other.neededTime + 1));
    }

    bool operator>(const Process& other) const {
        return (other.neededTime > workedTime);
    }

    bool operator+=(const Process& other) {
        return (workedTime += other.priority);
    }
};

std::istream& operator>>(std::istream& in, Process& process) {
    in >> process.priority >> process.neededTime;
    process.workedTime = 0;
    return in;
}

class ProcessComparator {
   public:
    bool operator()(const Process& first, const Process& second) {
        return first < second;
    }

    bool operator()(const int& first, const int& second) {
        return first < second;
    }
};

template <typename T, typename Comparator>
class Heap {
   public:
    Heap() : heap(new T[0]), sizeOfHeap(0) {}

    ~Heap() { delete[] heap; };

    Heap(const Heap& other) = delete;
    Heap& operator=(const Heap& other) = delete;

    void Add() {
        IncreaseHeap();
        std::cin >> heap[sizeOfHeap - 1];

        siftUp(sizeOfHeap - 1);
    }

    void Extract() {
        assert(!IsEmpty());
        if (heap[0] > heap[0]) {
            heap[0] += heap[0];
            numOfOperations++;
        } else {
            heap[0] = heap[sizeOfHeap - 1];
            DecreaseHeap();

            if (!IsEmpty()) {
                siftDown(0);
            }
        }
    }

    int numOfOperations = 0;

    bool IsEmpty() { return sizeOfHeap == 0; }

   private:
    T* heap = new T[0];
    int sizeOfHeap;
    Comparator cmp;

    void siftDown(int i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        int smallest = i;

        if (cmp(left, sizeOfHeap) && cmp(heap[left], heap[i])) {
            smallest = left;
        }

        if (cmp(right, sizeOfHeap) && cmp(heap[right], heap[smallest])) {
            smallest = right;
        }

        if (smallest != i) {
            std::swap(heap[i], heap[smallest]);
            siftDown(smallest);
        }
    }

    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (cmp(heap[i], heap[parent])) {
                std::swap(heap[i], heap[parent]);
                i = parent;
            } else {
                return;
            }
        }
    }

    void IncreaseHeap() {
        sizeOfHeap = sizeOfHeap + 1;

        T* tmpHeap = new T[sizeOfHeap];

        for (int i = 0; i < sizeOfHeap - 1; ++i) {
            tmpHeap[i] = heap[i];
        }

        delete[] heap;
        heap = tmpHeap;
    }

    void DecreaseHeap() {
        sizeOfHeap = sizeOfHeap - 1;

        T* tmpHeap = new T[sizeOfHeap];

        for (int i = 0; i < sizeOfHeap; ++i) {
            tmpHeap[i] = heap[i];
        }

        delete[] heap;
        heap = tmpHeap;
    }
};

int main() {
    int sizeHeap;
    std::cin >> sizeHeap;

    Heap<Process, ProcessComparator> myHeap;

    for (int i = 0; i < sizeHeap; ++i) {
        myHeap.Add();
    }

    while (!myHeap.IsEmpty()) {
        myHeap.Extract();
    }

    std::cout << myHeap.numOfOperations;

    return 0;
}