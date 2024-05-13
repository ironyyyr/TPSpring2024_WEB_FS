/*
Постройте B-дерево минимального порядка t и выведите его по слоям.
В качестве ключа используются числа, лежащие в диапазоне [0..232-1]
Требования:
B-дерево должно быть реализовано в виде шаблонного класса.
Решение должно поддерживать передачу функции сравнения снаружи.
*/

#include <iostream>
#include <queue>
#include <sstream>

template <typename T>
class BTree {
    struct Node {
        Node(bool leaf) : leaf(leaf) {}
        ~Node() {
            for (Node* child : children) {
                delete child;
            }
        }

        bool leaf;
        std::vector<T> keys;
        std::vector<Node*> children;
    };

   public:
    BTree(size_t min_degree) : t(min_degree), root(nullptr) {}

    ~BTree() {
        if (root) {
            delete root;
        }
    }

    void Insert(const T& key) {
        if (!root) {
            root = new Node(true);
        }

        if (isNodeFull(root)) {
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(root, 0);
        }

        insertNonFull(root, key);
    }

    void printTreeWrapper() { printTree(root); }

   private:
    void printTree(Node* node) {
        std::queue<Node*> myQueque;
        myQueque.push(root);

        while (!myQueque.empty()) {
            int numOfElems = myQueque.size();

            for (int i = 0; i < numOfElems; ++i) {
                Node* tmp = myQueque.front();
                myQueque.pop();

                for (int j = 0; j < tmp->keys.size(); ++j) {
                    std::cout << tmp->keys[j] << " ";
                }

                if (!tmp->leaf) {
                    for (int j = 0; j < tmp->children.size(); ++j) {
                        myQueque.push(tmp->children[j]);
                    }
                }
            }

            std::cout << std::endl;
        }
    }

    bool isNodeFull(Node* node) { return node->keys.size() == 2 * t - 1; }

    void splitChild(Node* node, size_t index) {
        Node* nodeChild = node->children[index];
        Node* tmp = new Node(nodeChild->leaf);

        tmp->keys.assign(nodeChild->keys.begin() + t, nodeChild->keys.end());
        nodeChild->keys.resize(t - 1);

        if (!nodeChild->leaf) {
            tmp->children.assign(nodeChild->children.begin() + t,
                                 nodeChild->children.end());
            nodeChild->children.resize(t);
        }

        node->children.insert(node->children.begin() + index + 1, tmp);
        node->keys.insert(node->keys.begin() + index, nodeChild->keys[t - 1]);
    }

    void insertNonFull(Node* node, const T& key) {
        int pos = node->keys.size() - 1;

        if (node->leaf) {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && key < node->keys[pos]) {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        } else {
            while (pos >= 0 && key < node->keys[pos]) {
                pos--;
            }

            if (isNodeFull(node->children[pos + 1])) {
                splitChild(node, pos + 1);
                if (key > node->keys[pos + 1]) pos++;
            }
            insertNonFull(node->children[pos + 1], key);
        }
    }

    size_t t;
    Node* root;
};

int main() {
    int minDegree;
    std::cin >> minDegree;
    std::cin.ignore();

    BTree<long long int> myBTree(minDegree);

    std::string line;
    std::getline(std::cin, line);

    std::istringstream iss(line);

    long long int num;
    while (iss >> num) {
        myBTree.Insert(num);
    }

    myBTree.printTreeWrapper();

    return 0;
}