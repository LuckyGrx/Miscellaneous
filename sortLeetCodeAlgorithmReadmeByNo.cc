#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>
using namespace std;

 /*
  * 将LeetCode-Algorithm库中的Readme.md文件内容按索引(题目编号)排序
  * ifs：输入文件流
  * ofs：输出文件流
  */
void sortLeetCodeAlgorithmReadmeByNo(ifstream &ifs, ofstream &ofs) {
    vector<string> titles;
    map<string, string> indexes;
    string line;
    int indexBegin, indexEnd;
    while (getline(ifs, line)) {
        if (line.size() > 4) {
            if (line[0] == '|' && isdigit(line[1]) && isdigit(line[2]) && isdigit(line[2]))
                titles.push_back(line);
            if (line[0] == '[') {
                indexBegin = line.find_first_of('[') + 1;
                indexEnd = line.find_first_of(']'); 
                indexes[line.substr(indexBegin, indexEnd - indexBegin)] = line;
            }
        }
    }
    sort(titles.begin(), titles.end());
    string index;
    int idx = 1;
    for (auto &title : titles) {
        indexBegin = title.find_first_of('[', title.find_first_of('[') + 1) + 1;
        indexEnd = title.find_first_of(']', title.find_first_of(']') + 1);
        index = title.substr(indexBegin, indexEnd - indexBegin);
        title.replace(indexBegin, indexEnd - indexBegin, to_string(idx));
        indexes[index].replace(1, indexes[index].find_first_of(']') - 1, to_string(idx));
        ++idx;
    }
    for (auto &line : titles)
        ofs << line << endl;
    ofs << endl;
    auto iter = indexes.begin();
    while (iter != indexes.end()) {
        ofs << iter->second << endl;
        ++iter;
    }
    ifs.close();
    ofs.close();
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "args error!" << endl;
        return -1;
    }
    ifstream ifs(argv[1]);
    if (!ifs) {
        cerr << "Open File Error!" << endl;
        return -1;
    }
    ofstream ofs(argv[2]);
    sortLeetCodeAlgorithmReadmeByNo(ifs, ofs);
    return 0;
}