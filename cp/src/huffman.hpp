#ifndef  HUFFMAN_HPP
#define HUFFMAN_HP

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

const unsigned int BYTESIZE = 8;

class Node{
    public:
        char character;
        int fraq;
        Node *left, *right;
        Node(char C, int F, Node *L, Node *R):
                        left(L), right(R),
                        fraq(F), character(C) {}
};

class HuffmanTree{
    public:
        Node *root;
        HuffmanTree(std::string filename, std::map<char, std::string> &keys);
        HuffmanTree(std::string filename, std::string codes, std::map<std::string, char> &keys);
        void printTree(Node* ptr, int blank);
        void serialTree(Node* ptr, std::string &res);

    private:
        std::vector<Node*> fraqChar;
        std::vector<Node*> fraqTree;
        std::pair<Node*, Node*> minFraq(Node *node1, Node *node2, Node *node3, Node *node4);
        static bool compareFraq(const Node *first, const Node *second){
            return (first->fraq < second->fraq);
        }
        void addToFraqTree(Node *first, Node *second){
            fraqTree.push_back(new Node('*', first->fraq + second->fraq, first, second));
        }
        void сountСharaсters(std::string filename);
        void growTree();
        
        void buildTree(Node *current, std::string::iterator &cnt);

        void getCodes(Node *ptr, std::string res, char side,
                    std::map<char, std::string> &codes);
};

HuffmanTree::HuffmanTree(std::string filename, std::map<char, std::string> &keys): fraqChar(), fraqTree(){
    сountСharaсters(filename);
    growTree();// printTree(root, 0);
    if(!root){
        std::cout << "Error:couldn't build tree\n";
    }
    getCodes(root, "", '*', keys);
}

void HuffmanTree::сountСharaсters(std::string filename){
    std::map<char, int> temp;
    
    std::string str;
    std::ifstream descr(filename);
    std::getline(descr, str);
    for(char index: str){
        temp[index]++;
    };
    descr.close();
    for(std::pair<char, int> elem: temp){
        fraqChar.push_back(new Node(elem.first, 
                                    elem.second, 
                                    nullptr, 
                                    nullptr));
    }
    std::sort(fraqChar.begin(), fraqChar.end(), compareFraq);
}

std::pair<Node*, Node*> HuffmanTree::minFraq(Node *node1, Node *node2, Node *node3, Node *node4){
    if(node1->fraq + node2->fraq <= node3->fraq + node4->fraq){
        return std::make_pair(node1, node2);
    } else {
        return std::make_pair(node3, node4);
    }
}

void HuffmanTree::growTree(){
    while(!fraqChar.empty() || !fraqTree.empty()){
        if(fraqChar.size() >= 2 && fraqTree.size() >= 2){
            std::pair<Node*, Node*> temp_res = minFraq(fraqChar[0], fraqChar[1], fraqTree[0], fraqTree[1]);
            std::pair<Node*, Node*> res = minFraq(temp_res.first, temp_res.second, fraqChar[0], fraqTree[0]);
            if(res.second == fraqTree[0]){
                addToFraqTree(res.first, res.second);
                fraqChar.erase(fraqChar.begin());
                fraqTree.erase(fraqTree.begin());
            } else if(res.second == fraqTree[1]){
                addToFraqTree(res.first, res.second);
                fraqTree.erase(fraqTree.begin());
                fraqTree.erase(fraqTree.begin());
            } else {
                addToFraqTree(res.first, res.second);
                fraqChar.erase(fraqChar.begin());
                fraqChar.erase(fraqChar.begin());
            }
        } else if(fraqChar.size() == 1 && fraqTree.size() >= 2){
            std::pair<Node*, Node*> res = minFraq(fraqChar[0], fraqTree[0], fraqTree[0], fraqTree[1]);
            if(res.second == fraqTree[0]){
                addToFraqTree(res.first, res.second);
                fraqChar.erase(fraqChar.begin());
                fraqTree.erase(fraqTree.begin());
            } else {
                addToFraqTree(res.first, res.second);
                fraqTree.erase(fraqTree.begin());
                fraqTree.erase(fraqTree.begin());
            }
        } else if(fraqChar.size() >= 2 && fraqTree.size() == 1){
            std::pair<Node*, Node*> res = minFraq(fraqChar[0], fraqChar[1], fraqChar[0], fraqTree[0]);
            if(res.second == fraqTree[0]){
                addToFraqTree(res.first, res.second);
                fraqChar.erase(fraqChar.begin());
                fraqTree.erase(fraqTree.begin());
            } else {
                addToFraqTree(res.first, res.second);
                fraqChar.erase(fraqChar.begin());
                fraqChar.erase(fraqChar.begin());
            }
        } else if(fraqChar.empty() && fraqTree.size() >= 2){
                addToFraqTree(fraqTree[0], fraqTree[1]);
                fraqTree.erase(fraqTree.begin());
                fraqTree.erase(fraqTree.begin());
        } else if(fraqChar.size() >= 2 && fraqTree.empty()){
                addToFraqTree(fraqChar[0], fraqChar[1]);
                fraqChar.erase(fraqChar.begin());
                fraqChar.erase(fraqChar.begin());
        } else if(fraqChar.size() == 1 && fraqTree.size() == 1){
                addToFraqTree(fraqChar[0], fraqTree[0]);
                fraqChar.erase(fraqChar.begin());
                fraqTree.erase(fraqTree.begin());
        } else if(fraqChar.empty() && fraqTree.size() == 1){
                root = fraqTree[0];
                return;
        } else if(fraqChar.size() == 1 && fraqTree.empty()){
                fraqTree.push_back(fraqChar[0]);
                fraqChar.erase(fraqChar.begin());
                root = fraqTree[0];
                return;
        }
    }
}

void HuffmanTree::printTree(Node *ptr,int blank) {
    if (!ptr) {
        return;
    } else {
        printTree(ptr->left, ++blank);
        blank--;
        for (int i=0; i<blank; ++i) {
            std::cout << "  ";
        }
        std::cout << ptr->character << std::endl;
    }
    printTree(ptr->right, ++blank);
}

void HuffmanTree::getCodes(Node *ptr, std::string res, char side, 
                                std::map<char, std::string> &codes){
    if(!ptr) return;
    if(side == 'l'){
        res += '0';
    } else if(side == 'r'){
        res += '1';
    }
    if (!(ptr->left||ptr->right)) {
        codes[ptr->character] = res;
        return;
    }

    getCodes(ptr->left, res, 'l', codes);
    getCodes(ptr->right, res, 'r', codes);
}

void HuffmanTree::serialTree(Node* ptr, std::string &res){
    if(!ptr){
        res = "";
        return;
    } else if(ptr->left||ptr->right){
        res += '0';
        serialTree(ptr->left, res);
        serialTree(ptr->right, res);
    } else {
        res += '1';
        unsigned char byte = ptr->character;
        for(int cnt = 0; cnt < BYTESIZE; cnt++){
            res += (byte >> 7)? '1':'0';
            byte <<= 1;
        }
    }
}


std::string toBin(int number){
    std::string rev = "";
    while(number){
        rev += (number % 2)? '1':'0';
        number >>= 1;
    }
    std::string res(4 * BYTESIZE, '0');
    for(int i = 0; i < rev.size(); i++){
        res[res.size() - 1 - i] = rev[i];
    }
    return res;
}

std::string writeBinFile(std::string filename, std::string exp, std::string binaries, int dsize){
    filename = filename.substr(0, filename.size() - 4) + exp;
    std::ofstream odescr(filename, std::ios::binary);
    char byte = 0;
    int size = binaries.size();
    binaries = toBin(dsize) + toBin(size) + binaries;
    for(int cnt = 0; cnt < binaries.size(); cnt++){
        byte |= (binaries[cnt] - '0')? 1:0;
        if(!((cnt + 1) % BYTESIZE)){
            odescr.put(byte);
            byte = 0;
            continue;
        }
        if((cnt + 1) == binaries.size()){ 
            byte <<= (binaries.size() % BYTESIZE)? BYTESIZE - (cnt + 1) % BYTESIZE : 0;
            odescr.put(byte);
        }
        byte <<= 1;
    }
    odescr.close();
    return filename; 
}

std::string huffmanIn(std::string filename, int size){
    std::map<char, std::string> keys;
    HuffmanTree tree = HuffmanTree(filename, keys);
    std::string codes = "";
    tree.serialTree(tree.root, codes);
    writeBinFile(filename, ".keys", codes, 127);
    //========================================
    std::string T, CT = "";
    std::ifstream idescr(filename);
    std::getline(idescr, T);
    for(char input_char: T){
        CT += keys[input_char];
    }
    idescr.close();
    return writeBinFile(filename, ".arc", CT, (size == -1)? T.size(): size); 
}




HuffmanTree::HuffmanTree(std::string filename, std::string codes, std::map<std::string, char> &keys){
    root = (codes.empty())? nullptr: new Node('*', 0, nullptr, nullptr);
    if(!root){
        std::cout << "Error: couldn't deserial tree" << std::endl;
    }
    std::string::iterator index = codes.begin();
    buildTree(root, index);

    std::map<char, std::string> reversed;
    getCodes(root, "", '*', reversed);
    for(auto elem: reversed){
        keys[elem.second] = elem.first;
    }
}

void HuffmanTree::buildTree(Node *current, std::string::iterator &cnt){
    if(*cnt == '0'){
        current->left = new Node('*', 0, nullptr, nullptr);
        buildTree(current->left, ++cnt);
        if(*(cnt + 1) == '0' || *(cnt + 1) == '1'){
            current->right = new Node('*', 0, nullptr, nullptr);
            buildTree(current->right, ++cnt);
        }
    } else if(*cnt == '1'){
        unsigned char byte = (*(++cnt) - '0')? 1:0;
        for(int rad = 1; rad < BYTESIZE; rad++){
            byte <<= 1;
            byte |= (*(++cnt) - '0')? 1:0;
        }
        current->character = byte;
    }
}


void readBinFile(std::string filename, std::string &str){
    std::ifstream idescr(filename, std::ios::binary);
    char byte;
    for(int i=0; i<4; i++){
        idescr.get(byte);
    }
    int size = 0;
    for(int i=0; i<4; i++){
        idescr.get(byte);
        for(int cnt=0; cnt < BYTESIZE; cnt++){
            size <<= 1;
            size += (byte >> 7)? 1:0;
            byte <<= 1;
        }
    }
    while(idescr.get(byte)){
        for(int cnt = 0; cnt < BYTESIZE; cnt++){
            str += (byte >> 7)? '1':'0'; 
            byte <<= 1;
        }
    }
    str = str.substr(0, size);
    idescr.close();
}

std::string huffmanOut(std::string filename){
    std::string codes = "";
    readBinFile(filename.substr(0, filename.size() - 4) + ".keys", codes);
    std::map<std::string, char> keys;
    HuffmanTree tree = HuffmanTree(filename, codes, keys);
    //======================================================
    std::string CT = "";
    readBinFile(filename, CT);
    std::string code = "";
    filename = filename.substr(0, filename.size() - 4) + "_unarc.txt";
    std::ofstream odescr(filename);
    for(char bit: CT){ 
        code += bit;
        if(keys.find(code) != keys.end()){
            odescr << keys[code];
            code = ""; 
        }
    } 
    odescr.close();
    return filename;
}


#endif //HUFFMAN_HPP


