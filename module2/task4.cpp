/*
В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была
далеко не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе
приходилось выгонять из шеренги за плохо начищенные сапоги. Однако солдаты в
процессе прихода и ухода должны были всегда быть выстроены по росту – сначала
самые высокие, а в конце – самые низкие. За расстановку солдат отвечал
прапорщик, который заметил интересную особенность – все солдаты в части разного
роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять
солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом
в строе он должен становится. Требуемая скорость выполнения команды - O(log n).
*/

#include <iostream>

struct AvlComparator {
    AvlComparator() {}

    bool equal(const int x, const int y) { return x == y; }

    bool more(const int& x, const int& y) { return x > y; }
};

template <typename T, typename Comparator>
class AvlTree {
    struct Node {
        Node(const T& data)
            : data(data),
              left(nullptr),
              right(nullptr),
              height(1),
              numOfTops(1) {}

        T data;
        Node* left;
        Node* right;
        size_t height;
        size_t numOfTops;
    };

   public:
    AvlTree(const Comparator& cmp) : cmp(cmp), root(nullptr) {}

    ~AvlTree() { destroyTree(root); }

    void Add(const T& data) {
        root = addInternal(root, data);
        std::cout << determKStat(root, data) << std::endl;
    }

    void Delete(const T& k) {
        int tmpData = getKStatNum(root, k);
        root = deleteInternal(root, tmpData);
    }

   private:
    int determKStat(Node* node, int data) {
        if (cmp.equal(data, node->data)) {
            return getNumOfTops(node->left);
        }

        if (cmp.more(data, node->data)) {
            return determKStat(node->left, data);
        } else {
            return determKStat(node->right, data) + getNumOfTops(node->left) +
                   1;
        }
    }

    int getKStatNum(Node* node, const int k) {
        if (getNumOfTops(node->left) == k) {
            return node->data;
        }

        if (getNumOfTops(node->left) > k) {
            return getKStatNum(node->left, k);
        } else {
            return getKStatNum(node->right, k - getNumOfTops(node->left) - 1);
        }
    }

    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* deleteInternal(Node* node, const T& data) {
        if (!node) {
            return nullptr;
        }

        if (cmp.more(data, node->data)) {
            node->left = deleteInternal(node->left, data);
        } else if (cmp.more(node->data, data)) {
            node->right = deleteInternal(node->right, data);
        } else {
            Node* left = node->left;
            Node* right = node->right;

            delete node;

            if (!right) {
                return left;
            }

            Node* min = nullptr;
            Node* tmpNode = findAndRemoveMin(right, min);
            min->left = left;
            min->right = tmpNode;
            return doBalance(min);
        }
        return doBalance(node);
    }

    Node* findAndRemoveMin(Node* node, Node*& min) {
        if (!node->left) {
            min = node;
            return node->right;
        }
        node->left = findAndRemoveMin(node->left, min);
        return doBalance(node);
    }

    Node* addInternal(Node* node, const T& data) {
        if (!node) {
            return new Node(data);
        }

        if (cmp.more(data, node->data))
            node->left = addInternal(node->left, data);
        else
            node->right = addInternal(node->right, data);

        return doBalance(node);
    }

    int getHeight(Node* node) { return node ? node->height : 0; }

    int getNumOfTops(Node* node) { return node ? node->numOfTops : 0; }

    int getBalance(Node* node) {
        return getHeight(node->right) - getHeight(node->left);
    }

    void fixHeight(Node* node) {
        node->height =
            std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    void fixNumOfTops(Node* node) {
        node->numOfTops =
            getNumOfTops(node->left) + getNumOfTops(node->right) + 1;
    }

    Node* rotateLeft(Node* node) {
        Node* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixNumOfTops(node);
        fixHeight(tmp);
        fixNumOfTops(tmp);
        return tmp;
    }

    Node* rotateRight(Node* node) {
        Node* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixNumOfTops(node);
        fixHeight(tmp);
        fixNumOfTops(tmp);
        return tmp;
    }

    Node* doBalance(Node* node) {
        fixHeight(node);
        fixNumOfTops(node);
        switch (getBalance(node)) {
            case 2: {
                if (getBalance(node->right) < 0) {
                    node->right = rotateRight(node->right);
                }
                return rotateLeft(node);
            }
            case -2: {
                if (getBalance(node->left) > 0) {
                    node->left = rotateLeft(node->left);
                }
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    Comparator cmp;
    Node* root;
};

int main() {
    int numOfCommands;
    std::cin >> numOfCommands;
    AvlComparator tmpComparator;
    AvlTree<int, AvlComparator> myAvlTree(tmpComparator);

    for (int i = 0; i < numOfCommands; ++i) {
        int k, val;
        std::cin >> k >> val;
        switch (k) {
            case 1:
                myAvlTree.Add(val);
                break;

            case 2:
                myAvlTree.Delete(val);
                break;
        }
    }

    return 0;
}