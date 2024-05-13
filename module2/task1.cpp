/*
Реализуйте структуру данных типа “множество строк” на основе динамической
хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
строчных латинских букв. Хеш-функция строки должна быть реализована с помощью
вычисления значения многочлена методом Горнера. Начальный размер таблицы должен
быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
когда коэффициент заполнения таблицы достигает 3/4. Структура данных должна
поддерживать операции добавления строки в множество, удаления строки из
множества и проверки принадлежности данной строки множеству. 1_1. Для разрешения
коллизий используйте квадратичное пробирование. i-ая проба g(k, i)=g(k, i-1) + i
(mod m). m - степень двойки.
*/

#include <iostream>
#include <vector>

const size_t DEFAULT_SIZE = 8;

enum class KeyStatus {

};

class StringHasher {
   public:
    StringHasher(size_t prime = 53) : prime(prime) {}

    size_t operator()(const std::string& str) {
        size_t hash = 0;
        for (int i = 0; i < str.size(); ++i) {
            hash = hash * prime + str[i];
        }
        return hash;
    }

   private:
    size_t prime;
};

template <typename T, typename Hasher>
class HashTable {
   public:
    HashTable(size_t initialSize = DEFAULT_SIZE)
        : size(0), table(initialSize, "NIL") {}

    ~HashTable() {}

    bool Add(const T& key) {
        if (calcFullness() > 0.75) {
            grow();
        }

        size_t hash = hasher(key) % table.size();

        int firstDelCell = -1;
        for (int i = 0; i < table.size(); ++i) {
            hash = (hash + (i * (i + 1) / 2)) % table.size();
            if (table[hash] == key) {
                return false;
            }

            if (table[hash] == "DEL" && firstDelCell == -1) {
                firstDelCell = hash;
            }

            if (table[hash] == "NIL") {
                table[hash] = key;
                size++;
                return true;
            }
        }
        table[firstDelCell] = key;
        size++;
        return true;
    }

    bool Has(const T& key) {
        size_t hash = hasher(key) % table.size();

        for (int i = 0; i < table.size(); ++i) {
            hash = (hash + (i * (i + 1) / 2)) % table.size();
            if (table[hash] == key) {
                return true;
            }

            if (table[hash] == "DEL") {
                continue;
            }

            if (table[hash] == "NIL") {
                return false;
            }
        }
        return false;
    }

    bool Delete(const T& key) {
        size_t hash = hasher(key) % table.size();

        for (int i = 0; i < table.size(); ++i) {
            hash = (hash + (i * (i + 1) / 2)) % table.size();
            if (table[hash] == key) {
                table[hash] = "DEL";
                size--;
                return true;
            }

            if (table[hash] == "DEL") {
                continue;
            }

            if (table[hash] == "NIL") {
                return false;
            }
        }
        return false;
    }

   private:
    void grow() {
        std::vector<T> newTable(table.size() * 2, "NIL");

        for (int i = 0; i < table.size(); ++i) {
            if (table[i] != "NIL" && table[i] != "DEL") {
                size_t hash = hasher(table[i]) % newTable.size();
                for (int j = 0; j < table.size(), newTable[hash] != "NIL";
                     ++j) {
                    hash = (hash + (j * (j + 1) / 2)) % newTable.size();
                }
                newTable[hash] = table[i];
            }
        }

        std::swap(table, newTable);
    }

    float calcFullness() { return size / (double)table.size(); }

    std::vector<T> table;
    size_t size;
    Hasher hasher;
};

int main() {
    HashTable<std::string, StringHasher> myHashTable;

    char op;
    std::string key;

    while (std::cin >> op >> key) {
        switch (op) {
            case '?': {
                std::cout << (myHashTable.Has(key) ? "OK" : "FAIL")
                          << std::endl;
                break;
            }
            case '+': {
                std::cout << (myHashTable.Add(key) ? "OK" : "FAIL")
                          << std::endl;
                break;
            }
            case '-': {
                std::cout << (myHashTable.Delete(key) ? "OK" : "FAIL")
                          << std::endl;
                break;
            }
        }
    }

    return 0;
}