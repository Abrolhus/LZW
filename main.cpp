#include <iostream>
#include <map>
#include <fstream>
using namespace std;
int main(int argc, const char** argv){
    string decompress(ifstream& in, unsigned int dictSize=256);
    void lzw(ifstream& in, ofstream& of, unsigned int dictSize=256);
    ifstream arq(argv[1]);
    if(argc == 3){
        ofstream out(argv[2], ios::binary);
        lzw(arq, out);
    }
    else{
        string result = decompress(arq);
        cout << result << endl;
    }
    return 0;
}
void lzw(ifstream& in, ofstream& of, unsigned int dictSize=256){
    map<string, unsigned short> dict;
    for(unsigned short i = 0; i < dictSize; i++){
        // dict.insert({string((&"" [ (char)i])), i});
        string aux = "";
        dict.insert({aux + (char)i, i});
        cout << aux + (char)i << ", ";
    }
    char next;
    string cur ="";
    int count = 0;
    unsigned short aux;
    string line;
    while(std::getline(in, line)){
        for(int i = 0; i < line.size()-1; i++){
            next = line.at(i+1);
            cur += line.at(i);
            if(dict.find(cur + next) != dict.end()){
                // cout << "[" << cur + next << "] ";
                continue;
            }
            else{
                // cout << cur << "";
                dict.insert({cur + next, dictSize+count}); // inserts {"str", 256} ... {"str2", 257} ....
                count++;
                aux = (unsigned short)dict.at(cur);
                of.write(reinterpret_cast<char *>(&aux), sizeof(unsigned short)); // from https://github.com/facundobrondo/lzw/blob/master/LZW.cpp
                // out += dict.at(cur); // adds to output
                // cout << "out last: "<< endl;
                // cout << out.at(out.size()-1);
                //cout << endl;
                // cout << dict.at(cur) << endl;
                cur = ""; // resets current string;
            }
        }
        string auxx = "";
        aux = (unsigned short)dict.at(auxx + line.at(line.size() - 1)); // TODO
        of.write(reinterpret_cast<char *>(&aux), sizeof(unsigned short)); // from https://github.com/facundobrondo/lzw/blob/master/LZW.cpp
    }
}
string decompress(ifstream& in, unsigned int dictSize=256){
    map<unsigned short, string> reverseDict;
    for(unsigned short i = 0; i < dictSize; i++){
        string aux = "";
        reverseDict.insert({i, aux + (char)i});
    }
    unsigned short cur;
    unsigned short next;
    int count = 0;
    string aux;
    string result;
    string strCur, strNext;
    in.read(reinterpret_cast<char *>(&cur), sizeof(unsigned short));
    strCur = reverseDict.at(cur);
    result += strCur;
    while(in.read(reinterpret_cast<char *>(&next), sizeof(unsigned short))){
        strNext = reverseDict.at(next);
        result += strNext;
        reverseDict.insert({dictSize + count, strCur + strNext[0]});
        count++;
        strCur = strNext;
    }
    return result;
}


