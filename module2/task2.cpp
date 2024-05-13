/*
Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки. Т.е.,
при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то
узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Выведите элементы в порядке pre-order (сверху вниз).

Рекурсия запрещена.
*/

#include <iostream>
#include <stack>

template <typename T>
class BinaryTree {
    struct Node {
        Node(const T& data) : data(data), left(nullptr), right(nullptr) {}

        T data;
        Node* left;
        Node* right;
    };

   public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() { destroyTree(root); }

    void Add(const T& data) {
        if (root == nullptr) {
            root = new Node(data);
        }

        Node** currNode = &root;

        while (*currNode) {
            Node& node = **currNode;
            if (data < node.data) {
                currNode = &node.left;
            } else if (data > node.data) {
                currNode = &node.right;
            } else {
                return;
            }
        }
        *currNode = new Node(data);
    }

   private:
    void destroyTree(Node* root) {
        std::stack<Node*> myStack;
        myStack.push(root);
        while (!myStack.empty()) {
            Node* currNode = myStack.top();
            myStack.pop();

            std::cout << currNode->data << " ";

            if (currNode->right) {
                myStack.push(currNode->right);
            }

            if (currNode->left) {
                myStack.push(currNode->left);
            }

            delete currNode;
        }
    }

    Node* root;
};

int main() {
    BinaryTree<int> myBinaryTree;

    int numOfElems = 0;
    std::cin >> numOfElems;

    for (int i = 0; i < numOfElems; ++i) {
        int tmpElem;
        std::cin >> tmpElem;
        myBinaryTree.Add(tmpElem);
    }

    return 0;
}
