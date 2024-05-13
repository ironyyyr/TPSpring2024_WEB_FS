#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

#include "Huffman.h"

struct Node {
    Node(const unsigned char& val)
        : freq(0), val(val), left(nullptr), right(nullptr) {}

    Node() : freq(0), val(), left(nullptr), right(nullptr) {}

    int freq;
    unsigned char val;
    Node* left;
    Node* right;
};

class BitWriter {
   public:
    BitWriter() : bitCount(0) {}

    void WriteBit(unsigned char bit) {
        if (bitCount % 8 == 0) {
            buffer.push_back(0);
        }
        if (bit) {
            buffer[bitCount / 8] |= 1 << (7 - bitCount % 8);
        }
        bitCount++;
    }

    void WriteByte(unsigned char byte) {
        if (bitCount % 8 == 0) {
            buffer.push_back(byte);
        } else {
            int offset = bitCount % 8;
            buffer[bitCount / 8] |= byte >> offset;
            buffer.push_back(byte << (8 - offset));
        }
        bitCount += 8;
    }

    const std::vector<unsigned char>& GetBuffer() const { return buffer; }

    size_t GetBitCount() const { return bitCount; }

   private:
    std::vector<unsigned char> buffer;
    size_t bitCount;
};

class BitReader {
   public:
    BitReader(std::vector<unsigned char>& buffer)
        : buffer(buffer), currentBit(0), currentByte(0) {}

    unsigned char ReadBit() {
        unsigned char byteBuffer = buffer[currentByte];
        unsigned char bit = (byteBuffer >> (7 - currentBit)) & 1;
        currentBit++;
        if (currentBit == 8) {
            currentByte++;
            currentBit = 0;
        }
        return bit;
    }

    unsigned char ReadByte() {
        unsigned char byteBuffer = buffer[currentByte];
        unsigned char byte = 0;
        if (currentBit == 0) {
            currentByte++;
            byte = byteBuffer;
        } else {
            unsigned char byteBuffer1 = buffer[currentByte] << currentBit;
            currentByte++;
            unsigned char byteBuffer2 = buffer[currentByte] >> (8 - currentBit);
            byte = byteBuffer1 | byteBuffer2;
        }
        return byte;
    }

   private:
    std::vector<unsigned char> buffer;
    size_t currentBit;
    size_t currentByte;
};

bool DFS(Node* node, std::stack<int>& myTableOfCodesStack,
         const unsigned char& data) {
    if (node == nullptr) {
        return false;
    }

    if (node->val == data && !node->left && !node->right) {
        return true;
    } else {
        myTableOfCodesStack.push(0);
        if (DFS(node->left, myTableOfCodesStack, data)) {
            return true;
        }
        myTableOfCodesStack.pop();
        myTableOfCodesStack.push(1);
        if (DFS(node->right, myTableOfCodesStack, data)) {
            return true;
        }
        myTableOfCodesStack.pop();
        return false;
    }
}

void serialyzeTree(Node* node, BitWriter& myBitWriter) {
    if (node == nullptr) {
        return;
    }

    if (!node->left && !node->right) {
        myBitWriter.WriteBit(1);
        myBitWriter.WriteByte(node->val);
    } else {
        serialyzeTree(node->left, myBitWriter);
        serialyzeTree(node->right, myBitWriter);
        myBitWriter.WriteBit(0);
    }
}

std::string buildCode(std::stack<int>& myTableOfCodesStack) {
    unsigned char code = 0;
    std::string symbCode;
    while (!myTableOfCodesStack.empty()) {
        symbCode.push_back(myTableOfCodesStack.top());
        myTableOfCodesStack.pop();
    }
    return symbCode;
}

void deleteTree(Node* node) {
    if (node->left) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<unsigned char> myVector;
    std::vector<Node*> tmpNodeMatr;
    tmpNodeMatr.resize(256);

    for (int i = 0; i < 256; ++i) {
        tmpNodeMatr[i] = new Node(i);
    }

    BitWriter myBitWriter;

    int usedSymbs = 0;

    unsigned char byte = 0;

    //Копирование поступающей информации в вектор
    while (original.Read(byte)) {
        myVector.push_back(byte);
        if (tmpNodeMatr[byte]->freq++ == 0) {
            usedSymbs++;
        }
    }

    auto iter =
        std::remove_if(tmpNodeMatr.begin(), tmpNodeMatr.end(),
                       [](const auto& node) { return node->freq == 0; });

    tmpNodeMatr.erase(iter, tmpNodeMatr.end());

    if (tmpNodeMatr.size() == 0) {
        return;
    }

    auto comp = [](Node* left, Node* right) {
        return left->freq > right->freq;
    };

    //Формирование кучи
    std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> myMinHeap(
        comp);

    //Построение дерева Хаффмана
    for (int i = 0; i < tmpNodeMatr.size(); ++i) {
        myMinHeap.push(tmpNodeMatr[i]);
    }

    while (myMinHeap.size() != 1) {
        Node* tmp = new Node();
        tmp->right = myMinHeap.top();
        tmp->freq += myMinHeap.top()->freq;
        myMinHeap.pop();
        tmp->left = myMinHeap.top();
        tmp->freq += myMinHeap.top()->freq;
        myMinHeap.pop();
        myMinHeap.push(tmp);
    }

    //Построение таблицы кодов
    std::unordered_map<unsigned char, std::string> myTableOfCodes;
    std::stack<int> myTableOfCodesStack;
    std::string codeOfSymb;

    Node* topNode = myMinHeap.top();

    for (int i = 0; i < tmpNodeMatr.size(); ++i) {
        DFS(topNode, myTableOfCodesStack, tmpNodeMatr[i]->val);
        myTableOfCodes[tmpNodeMatr[i]->val] = buildCode(myTableOfCodesStack);
    }

    //Записываем размер алфавита
    unsigned char byte1 = (usedSymbs >> 24) & 0xFF;
    myBitWriter.WriteByte(byte1);
    unsigned char byte2 = (usedSymbs >> 16) & 0xFF;
    myBitWriter.WriteByte(byte2);
    unsigned char byte3 = (usedSymbs >> 8) & 0xFF;
    myBitWriter.WriteByte(byte3);
    unsigned char byte4 = usedSymbs & 0xFF;
    myBitWriter.WriteByte(byte4);

    //Записываем длину сжатого сообщения для дальнейшей расшифровки
    int sizeOfData = 0;
    for (int i = 0; i < tmpNodeMatr.size(); ++i) {
        if (myTableOfCodes[tmpNodeMatr[i]->val].size() == 0) {
            sizeOfData += 1 * tmpNodeMatr[i]->freq;
        } else {
            sizeOfData += myTableOfCodes[tmpNodeMatr[i]->val].size() *
                          tmpNodeMatr[i]->freq;
        }
    }

    byte1 = (sizeOfData >> 24) & 0xFF;
    myBitWriter.WriteByte(byte1);
    byte2 = (sizeOfData >> 16) & 0xFF;
    myBitWriter.WriteByte(byte2);
    byte3 = (sizeOfData >> 8) & 0xFF;
    myBitWriter.WriteByte(byte3);
    byte4 = sizeOfData & 0xFF;
    myBitWriter.WriteByte(byte4);

    //Записываем дерево в буфер
    serialyzeTree(topNode, myBitWriter);

    //Кодируем данные
    for (int i = 0; i < myVector.size(); ++i) {
        std::string code = myTableOfCodes[myVector[i]];
        if (code.size() == 0) {
            myBitWriter.WriteBit((unsigned char)0);
        } else {
            for (int j = code.size() - 1; j >= 0; --j) {
                myBitWriter.WriteBit(code[j]);
            }
        }
    }

    auto tmpVec = myBitWriter.GetBuffer();

    for (int i = 0; i < tmpVec.size(); ++i) {
        compressed.Write(tmpVec[i]);
    }

    //Отчищаем динамические структуры
    deleteTree(topNode);
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    std::vector<unsigned char> tmpVector(0, 0);

    unsigned char byte = 0;

    while (compressed.Read(byte)) {
        tmpVector.push_back(byte);
    }

    if (tmpVector.size() == 0) {
        return;
    }

    BitReader myBitReader(tmpVector);

    //Считали размер алфавита
    unsigned char byte1 = myBitReader.ReadByte();
    unsigned char byte2 = myBitReader.ReadByte();
    unsigned char byte3 = myBitReader.ReadByte();
    unsigned char byte4 = myBitReader.ReadByte();

    int sizeOfAlphabet = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    std::vector<unsigned char> myVector;
    std::stack<Node*> myStack;

    //Считали размер сообщения в битах
    byte1 = myBitReader.ReadByte();
    byte2 = myBitReader.ReadByte();
    byte3 = myBitReader.ReadByte();
    byte4 = myBitReader.ReadByte();

    int codedMessageLengthInBit =
        (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

    int numOfReadedSymbs = 0;

    //Десереализовали дерево Хаффмана
    while (numOfReadedSymbs != sizeOfAlphabet || myStack.size() != 1) {
        int tmpBit = myBitReader.ReadBit();
        unsigned char tmpByte = 0;

        if (tmpBit == 1) {
            tmpByte = myBitReader.ReadByte();
            numOfReadedSymbs++;
            Node* tmpNode = new Node(tmpByte);
            myStack.push(tmpNode);
        } else {
            Node* tmpNode = new Node();
            Node* first = myStack.top();
            myStack.pop();
            Node* second = myStack.top();
            myStack.pop();
            tmpNode->right = first;
            tmpNode->left = second;
            myStack.push(tmpNode);
        }
    }

    Node* currPos = myStack.top();

    //Декодируем данные
    while (codedMessageLengthInBit) {
        int tmpBit = myBitReader.ReadBit();
        codedMessageLengthInBit--;
        if (tmpBit == -1) {
            break;
        }

        if (tmpBit == 1) {
            currPos = currPos->right;
        } else {
            currPos = currPos->left;
        }

        if (!currPos->left && !currPos->right) {
            original.Write(currPos->val);
            currPos = myStack.top();
        }
    }
}