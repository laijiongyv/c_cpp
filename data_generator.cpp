#include <iostream>
#include<ctime>
#include<fstream>
using namespace std;

#define FILENAME "data/datafile"

int main(){
    ofstream ofs;
    ofs.open(FILENAME);
    srand((unsigned int)time(NULL));
    int nums = rand() % 10 + 10;
    while(nums--){
        ofs << rand() % 100 << endl;
    }
    ofs.close();
    return 0;
}