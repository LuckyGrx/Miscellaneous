#include "Section.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "argc error!" << endl;
        return -1;
    }
    ofstream ofs(argv[1]);
    if (!ofs) {
        cerr << "create command file error!" << endl;
        return -1;
    }
    ifstream ifs(argv[2]);
    if (!ifs) {
        cerr << "open db file error!" << endl;
        return -1;
    }
    Section section(ofs, ifs, argv[1]);
    section.readDb();
    section.createSectionCommand();
    section.readCommand();
    ofs.close();
    ifs.close();

    srand((unsigned)time(NULL));
    for (int i = 0; i < 10; ++i)
        cout << rand()%100 << '\t';
    cout << endl;
    return 0;
}