#ifndef LZ77_HPP
#define LZ77_HPP


#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

struct TTriplet {
    int offset;
    int length;
    char nextSymbol;
};


std::string toString(int n){
    std::string temp = (n)? "":"0";
    while(n){
        temp += n % 10 + '0';
        n /= 10;
    }
    std::string res(temp.size(), '*');
    for(int i = 0; i < temp.size(); i++){
        res[i] = temp[temp.size() - i - 1];
    }
    return res;
}

int lz77In(std::string &filename) {
    std::string str = "";
    char inputC;
    std::ifstream idescr(filename);
        while(idescr >> inputC){
            str += inputC;
        }
    idescr.close();
    filename = filename.substr(0, filename.size() - 4) + "_lz77.txt";
    std::ofstream odescr(filename);
    int ptr = 0;
    while (ptr < str.length()) {
        int maxOffset = 0;
        int maxSize = 0;
        char nextSymbol = str[ptr];
        for (int i = 0; i < ptr; ++i) {
            if (str[i] == str[ptr]) {
                int tempOffset = ptr - i;
                int tempSize = 1;
                char tempNext = str[ptr + 1];
                for (int j = 1;
                     j + i < str.length() && str[j + i] == str[ptr + j]; ++j)
                {
                    tempSize++;
                    tempNext = str[ptr + tempSize];
                }
                if (tempSize >= maxSize) {
                    maxOffset = tempOffset;
                    maxSize = tempSize;
                    if (ptr + maxSize == str.length()) {
                        nextSymbol = '\n';
                    } else {
                        nextSymbol = tempNext;
                    }
                }
            }
        }
        odescr << toString(maxOffset) << ' ' << toString(maxSize) << ' ' << nextSymbol << '\t';
        ptr += maxSize + 1;
    }
    odescr.close();
    return str.size();
}


int toInt(std::string &n){
    int res = 0;
    for(char elem: n){
        res *=10;
        res += elem - '0';
    }
    return res;
}

std::string lz77Out(std::string filename) {
    std::string res = "";
    TTriplet replace;
    std::string offset, length;
    std::ifstream idescr(filename);
    while(idescr >> offset >> length){
        replace.offset = toInt(offset);
        replace.length = toInt(length);
        int ptr = (int)res.length();
        for (int j = 0; j < replace.length; ++j) {
            res.push_back(res[ptr - replace.offset + j]);
        }
        if(idescr >> replace.nextSymbol){
            res.push_back(replace.nextSymbol);  
        } else {
            break;
        }
    }
    idescr.close();
    std::ofstream odescr(filename);
    odescr << res;
    odescr.close();    
    return filename;
}

#endif // LZ77_HPP

