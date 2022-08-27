#include<iostream>
#include<fstream>
using namespace std;


int main()

{

    ifstream f("test.txt");
    fstream f2("binary.bin", ios::binary | ios::out);

    if(!f2.is_open()) cout<<"Binary not open";

    string line;

    while(getline(f,line)){
        f2.write((char*)&line,sizeof(string));
    }

    f.close();
    f2.close();

    fstream file("binary.bin",ios::binary| ios ::in);
    string out;
    while(1){
        file.read((char*)&out,sizeof(string));

        if(!file.good()) break;
        cout<<out;
    }
    file.close();
}
