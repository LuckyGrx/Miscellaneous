#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <time.h>
#include <algorithm>
#include <map>
using namespace std;

typedef struct db {
    string name;
    vector<int> values;
}db;

class Section {
private:
    ofstream& ofs;
    ifstream& ifs;
    string commandFileName;
    string dbFileName;
    map<int, db> dbs;
    map<string, int> keyIndex;
    static ofstream allOfs; 
private:
    void createSectionDelete();
    void createSectionSet(string key, int value);
    void createSectionAdd(string key, int value);
    void createSectionQueryKeyCompareValue(string key, int value);
    void createSectionQueryKeyK(string key, int k);
    void createSectionQueryKeyKList(string key, int k);
    void createSectionSumKey(string key);

    int getRandomValue(int first, int last);
    
    string getRandomKey();

    void setKeyIndex(string valueNames);

    void getQueryDb(map<int, db>::iterator iter, ofstream& queryOfs);

    void readDeleteCommand(stringstream& ss);
    void readSetCommand(stringstream& ss);
    void readAddCommand(stringstream& ss);
    void readQueryCommand(stringstream& ss);
    void readSumCommand(stringstream& ss);

    void executeDeleteCommand(int id1, int id2);
    void executeSetCommand(int id1, int id2, string key, int value);
    void executeAddCommand(int id1, int id2, string key, int value);
    void executeQueryCommand(int id1, int id2, string key, string compare, int value);
    void executeSumCommand(int id1, int id2, string key);
public:
    Section(ofstream& ofs, ifstream& ifs, string commandFileName) : ofs(ofs), ifs(ifs), commandFileName(commandFileName) {
        srand((unsigned)time(NULL));
    }
    ~Section() {
    }
    void createSectionCommand();

    void readCommand();
    void readDb();
};

ofstream Section::allOfs("SectionResult.txt");

void Section::createSectionCommand() {
    // Delete
    createSectionDelete();
    // Query
    createSectionQueryKeyCompareValue(getRandomKey(), getRandomValue(0, 100));
    ofs << endl;

    for (int idx = 0; idx < 10; ++idx) {
        // Set
        createSectionSet(getRandomKey(), getRandomValue(0, 100));
        // Query
        createSectionQueryKeyCompareValue(getRandomKey(), getRandomValue(0, 100));
    }
    ofs << endl;

    for (int idx = 0; idx < 10; ++idx) {
        // Add
        createSectionAdd(getRandomKey(), getRandomValue(0, 100));
        // Query
        createSectionQueryKeyCompareValue(getRandomKey(), getRandomValue(0, 100));
    }
    ofs << endl;

    for (int idx = 0; idx < 10; ++idx) {
        // Sum
        createSectionSumKey(getRandomKey());
    }
}

void Section::createSectionDelete() {
    int first = getRandomValue(1, 50000);
    int last = getRandomValue(first, first + 10000);
    ofs << "DELETE\t" << first << "\t" << last << endl;
}

void Section::createSectionSet(string key, int value) {
    int first = getRandomValue(1, 50000);
    int last = getRandomValue(first, first + 50000);
    ofs << "SET  \t" << first << "\t" << last << "\t" << key << "\t" << value << endl;
}

void Section::createSectionAdd(string key, int value) {
    int first = getRandomValue(1, 50000);
    int last = getRandomValue(first, first + 50000);
    ofs << "ADD  \t" << first << "\t" << last << "\t" << key << "\t" << value << endl;
}

void Section::createSectionQueryKeyCompareValue(string key, int value) {
    vector<string> label{">", "<", "=", "<=", ">=", "!="};
    int index = getRandomValue(0, label.size() - 1);
    int first = getRandomValue(1, 50000);
    int last = getRandomValue(first, first + 50000);
    ofs << "QUERY\t" << first << "\t" << last << "\t" << key << "\t" << label[index] << "\t" << value << endl;
}

void Section::createSectionSumKey(string key) {
    int first = getRandomValue(1, 50000);
    int last = getRandomValue(first, first + 50000);
    ofs << "SUM  \t" << first << "\t" << last << "\t" << key << endl; 
}

int Section::getRandomValue(int first, int last) {
    return rand() % (last - first + 1) + first;
}

string Section::getRandomKey() {
    vector<string> keyVec;
    auto iter = keyIndex.begin();
    while (iter != keyIndex.end()) {
        keyVec.push_back(iter->first);
        ++iter;
    }
    int index = getRandomValue(0, keyVec.size() - 1);
    return keyVec[index];
}

void Section::readCommand() {
    ifstream ifs(commandFileName);
    if (!ifs) {
        cerr << "open file error!" << endl;
        return ;
    }
    string line;
    while (getline(ifs, line)) {
        stringstream ss(line);
        string command;
        ss >> command;
        if (command == "DELETE")
            readDeleteCommand(ss);
        else if (command == "SET")
            readSetCommand(ss);
        else if (command == "ADD")
            readAddCommand(ss);
        else if (command == "QUERY")
            readQueryCommand(ss);
        else if (command == "SUM")
            readSumCommand(ss);
    }
}

void Section::readDeleteCommand(stringstream& ss) {
    cout << "I am DELETE" << endl;
    int id1, id2;
    ss >> id1 >> id2;
    executeDeleteCommand(id1, id2);
}

void Section::readSetCommand(stringstream& ss) {
    cout << "I am SET" << endl;
    int id1, id2, value;
    string key;
    ss >> id1 >> id2 >> key >> value;
    executeSetCommand(id1, id2, key, value);
}

void Section::readAddCommand(stringstream& ss) {
    cout << "I am ADD" << endl;
    int id1, id2, value;
    string key;
    ss >> id1 >> id2 >> key >> value;
    executeAddCommand(id1, id2, key, value);
}

void Section::readQueryCommand(stringstream& ss) {
    cout << "I am QUERY" << endl;
    int id1, id2;
    ss >> id1 >> id2;
    string key, label;
    ss >> key >> label;
    if (">" == label || "<" == label || "=" == label || "<=" == label || ">=" == label || "!=" == label) {
        int value;
        ss >> value;
        executeQueryCommand(id1, id2, key, label, value);
    }
}

void Section::readSumCommand(stringstream& ss) {
    cout << "I am SUM" << endl;
    int id1, id2;
    string key;
    ss >> id1 >> id2 >> key;
    executeSumCommand(id1, id2, key);
}

void Section::readDb() {
    string dbName, valuesName;
    getline(ifs, dbName);
    getline(ifs, valuesName);
    setKeyIndex(valuesName);
    string db;
    int id, value;
    string name;
    while (getline(ifs, db)) {
        stringstream ss(db);
        ss >> id >> name;
        vector<int> tmpValues;
        while (ss >> value)
            tmpValues.push_back(value);
        dbs[id] = {name, tmpValues};
    }
}

void Section::setKeyIndex(string valueNames) {
    stringstream ss(valueNames);
    string valueName;
    int index = 0;
    while (ss >> valueName) {
        if (',' == valueName[0]) {
            if (index != 0)
                keyIndex[valueName.substr(2)] = index - 1;
            ++index;
        }
    }
}

void Section::executeDeleteCommand(int id1, int id2) {
    auto iter = dbs.begin();
    while (iter != dbs.end()) {
        if (iter->first >= id1 && iter->first <= id2)
            iter = dbs.erase(iter);
        else
            ++iter;
    }
}

void Section::executeSetCommand(int id1, int id2, string key, int value) {
    auto iter = dbs.begin();
    while (iter != dbs.end()) {
        if (iter->first >= id1 && iter->first <= id2) {
            for (int idx = 0; idx < iter->second.values.size(); ++idx) {
                if (keyIndex[key] == idx) {
                    iter->second.values[idx] = value;
                    break;
                }
            }
        }
        ++iter;
    }
}

void Section::executeAddCommand(int id1, int id2, string key, int value) {
    auto iter = dbs.begin();
    while (iter != dbs.end()) {
        if (iter->first >= id1 && iter->first <= id2) {
            for (int idx = 0; idx < iter->second.values.size(); ++idx) {
                if (keyIndex[key] == idx) {
                    iter->second.values[idx] += value;
                    break;
                }
            }
        }
        ++iter;
    }
}

void Section::getQueryDb(map<int, db>::iterator iter, ofstream& queryOfs) {
    queryOfs << iter->first << " " << iter->second.name << " ";
    allOfs << iter->first << " " << iter->second.name << " ";
    for_each(iter->second.values.begin(), iter->second.values.end(), [&queryOfs](int value) {
        queryOfs << value << " ";
        allOfs << value << " ";
    });
    queryOfs << endl;
    allOfs << endl;
}

void Section::executeQueryCommand(int id1, int id2, string key, string compare, int value) {
    static int No = 0;
    cout << "execute Query Command" << endl;
    string queryFileName;
    ofstream queryOfs;
    if (No == 0)                  // Delete Command
        queryFileName = "Delete_" + to_string(No + 1) + "_Query.txt";
    else if (No >= 1 && No <= 10) // Set Command
        queryFileName = "Set_" + to_string(No) + "_Query.txt"; 
    else                          // Add Command
        queryFileName = "Add_" + to_string(No - 10) + "_Query.txt"; 
    queryOfs.open(queryFileName);
    ++No;
    if (!queryOfs) {
        cerr << "create query file error!" << endl;
        return ;
    }
    auto iter = dbs.begin();
    while (iter != dbs.end()) {
        if (iter->first >= id1 && iter->first <= id2) {
            for (int idx = 0; idx < iter->second.values.size(); ++idx) {
                if (keyIndex[key] == idx) {
                    if (">" == compare) {
                        if (iter->second.values[idx] > value) {
                            getQueryDb(iter, queryOfs);
                        }
                    } else if ("<" == compare) {
                        if (iter->second.values[idx] < value) {
                            getQueryDb(iter, queryOfs);
                        }
                    } else if ("=" == compare) {
                        if (iter->second.values[idx] == value) {
                            getQueryDb(iter, queryOfs);
                        }
                    } else if ("<=" == compare) {
                        if (iter->second.values[idx] <= value) {
                            getQueryDb(iter, queryOfs);
                        }
                    } else if (">=" == compare) {
                        if (iter->second.values[idx] >= value) {
                            getQueryDb(iter, queryOfs);
                        }
                    } else if ("!=" == compare) {
                        if (iter->second.values[idx] != value) {
                            getQueryDb(iter, queryOfs);
                        }
                    }
                    break;
                }
            }
        }
        ++iter;
    }
    queryOfs.close();
}

void Section::executeSumCommand(int id1, int id2, string key) {
    cout << "execute Sum Command" << endl;
    static ofstream sumOfs("Sum_1_Result.txt");
    if (!sumOfs) {
        cerr << "create sum file error!" << endl;
        return ;
    }
    auto iter = dbs.begin();
    int sum = 0;
    while (iter != dbs.end()) {
        if (iter->first >= id1 && iter->first <= id2) {
            for (int idx = 0; idx < iter->second.values.size(); ++idx) {
                if (keyIndex[key] == idx)
                    sum += iter->second.values[idx];
            }
        }
        ++iter;
    }
    sumOfs << sum << " " << endl;
    allOfs << sum << " " << endl;
}

