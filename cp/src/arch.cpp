#include <filesystem>
#include <iostream>
#include <ctime> 
#include <cmath>
#include <map>

#include "huffman.hpp"
#include "lz77.hpp"


std::string compress(std::string filename, bool flag9){
    if(flag9){
        int size = lz77In(filename);
        std::string res = huffmanIn(filename, size);
        remove(filename.c_str());
        return res;
    } else {
        return huffmanIn(filename, -1);
    }
}

std::string decompress(std::string filename, bool flag9){
    if(flag9){
        filename = huffmanOut(filename);
        std::string res = lz77Out(filename);
        //remove(filename.c_str());
        return res;
    } else {
        return huffmanOut(filename);
    }
}


void remFile(std::string filename, bool flagK, bool flagD){
    if(!flagK){
        if(flagD){
            remove((filename.substr(0, filename.size()-4) + ".keys").c_str());
        }
        remove(filename.c_str());
    }
}


void directoryTrav(std::string filename, std::map<std::string, bool> &flags){
    std::vector<std::string> trav;
    for (auto elem: std::filesystem::directory_iterator(filename)){
        trav.push_back(elem.path());
    }
    for(auto path: trav){
        if( path.substr(path.size() - 4, path.size()) == ".txt" || 
            path.substr(path.size() - 4, path.size()) == ".arc" ){
            if(flags["-d"]){
                std::string decomp = decompress(path, flags["-9"]);
            } else {
                compress(path, flags["-9"]);
            }
            remFile(path, flags["-k"], flags["-d"]);
        } else if(path.substr(path.size() - 5, path.size()) != ".keys"){
            directoryTrav(path, flags);
        }
    }
}


void countLoss(std::string filename){
    std::ifstream idescr(filename, std::ios::binary);
    int size, cnt = 0;
    char byte;
    for(int i = 0; i < 4; i++){
        idescr.get(byte);
    }
    for(int i = 0; i < 4; i++){
        idescr.get(byte);
        for(int cnt = 0; cnt < BYTESIZE; cnt++){
            size <<= 1;
            size += (byte >> 7)? 1:0; 
            byte <<= 1;
        }
    }
    while(idescr.get(byte)){
        cnt += 8;
    }
    if(cnt > size){
        std::cout << "There were no losses" << std::endl;
    } else {
        std::cout << "Lost " << size - cnt << " byte(s)" << std::endl;
    }
    idescr.close();
}


void analisys(std::string filename, bool flag){
    std::ifstream idescr(filename, std::ios::binary);
        int size = 0, dsize = 0;
        char byte = 0;
        for(int i = 0; i < 4; i++){
            idescr.get(byte);
            for(int cnt = 0; cnt < BYTESIZE; cnt++){
                dsize <<= 1;
                dsize += (byte >> 7)? 1:0; 
                byte <<= 1;
            }
        }
        for(int i = 0; i < 4; i++){
            idescr.get(byte);
            for(int cnt = 0; cnt < BYTESIZE; cnt++){
                size <<= 1;
                size += (byte >> 7)? 1:0; 
                byte <<= 1;
            }
        }
    idescr.close();
    std::ifstream keys(filename.substr(0, filename.size() - 4) + ".keys", std::ios::binary);
        int ksize = 0;
        for(int i = 0; i < 32; i++){
            idescr >> byte;
        }
        for(int i = 0; i < 4; i++){
            idescr >> byte;
            for(int cnt = 0; cnt < BYTESIZE; cnt++){
                ksize += (byte >> 7)? 1:0; 
                ksize <<= 1;
                byte <<= 1;
            }
        }
    keys.close();
    size += ksize;
    size /= BYTESIZE;
    double ratio = (double)size / dsize * 100;
    std::cout << '\t' << "compressed" << '\t' << "decompressed" << '\t';
    std::cout << "ratio" << "\t\t" << "filename" << std::endl;
    std::cout << "\t" << size << "\t\t" << dsize << "\t\t";
    std::cout << round(ratio * 100) / 100.0 << "%" << "\t\t";
    std::cout << filename.substr(0, filename.size() - 4) + ".txt" << std::endl;
}

int main(int argc, char** argv){
    if(argv[1][0] != '-'){
        for(int i=1; i < argc; i++){
            huffmanIn(argv[i], -1);
            remove(argv[i]);
        }
    } else {
        std::map<std::string, bool> flags{
            {"-d", false}, {"-c", false},
            {"-k", false}, {"-l", false},
            {"-r", false}, {"-t", false},
            {"-1", false}, {"-9", false},
            {"-", false}
        };
        for(int i=1; i < argc; i++){
            if(argv[i][0] == '-'){
                flags[argv[i]] = true;
            }
        }
        if(flags["-1"] && flags["-9"] || flags["-"] && flags["-d"] || flags["-r"] && (flags["-c"] || flags["-"])){
            std::cout << "Error: flags -1 and -9 announced at the same time" << std::endl;
            return 0;
        }
        if(flags["-"]){
            std::string input;
            std::cin >> input;
            std::ofstream odescr("input.txt");
            odescr << input;
            odescr.close();
            flags["-k"] = false;
        }
        std::string filename = (flags["-"])? "input.txt" : argv[argc - 1];
        if(flags["-t"]){
            countLoss(filename);
            return 0;
        }
        if(flags["-l"]){ 
            if(filename.substr(filename.size() - 4, filename.size()) == ".arc"){
                analisys(filename, flags["-9"]);
            } else {
                std::cout << "Error: wrong type of file" << std::endl;
            }
            return 0;
        }
        if(flags["-r"]){
            directoryTrav(filename, flags);
        } else {
            unsigned int start_time = clock();
            std::string decomp;
            if(flags["-d"]){
                decomp = decompress(filename, flags["-9"]);
            } else {
                compress(filename, flags["-9"]);
            }
            unsigned int end_time = clock();
            std::cout << "time = " << end_time - start_time << " ms.\n";
            if(flags["-c"]){
                std::ifstream output(decomp);
                unsigned char C;
                while(output >> C){
                    std::cout << C;
                }
                output.close();
                std::cout << std::endl;
                remove(decomp.c_str());
            }
            remFile(filename, flags["-k"], flags["-d"]);
        }
    }
}

