#include<unordered_map>
#include<queue>
#include<iostream>
#include<fstream>
#include<string>
#include<bitset>
using namespace std;
typedef pair<int, int> pi;

// Class defined for a data structure to store character along side frequency
// and to build the binary tree to get codes
class Node
{
public:
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node()
    {
        left= NULL;
        right = NULL;
        ch = '\0';
        freq = 0;
    }
};
// defined to make Min priority queue for Node class
struct CompareNode
{
    bool operator()(Node* const &lhs,Node* const &rhs)
    {
        return lhs->freq>rhs->freq && lhs->ch<rhs->ch;
    }
};



// Class for encapsulating Huffman process
class Huffman
{
    int arr[256]; // to store frequency of characters
    unordered_map<char,string> ch_to_code; //Hash map that stores character as key and code as value
    unordered_map<string,char> code_to_ch; //Hash map that stores character as value and code as key

     //priority queue to help make binary tree
    priority_queue<Node*,vector<Node*>,CompareNode> make_queue()
    {
        priority_queue<Node*,vector<Node*>,CompareNode> pq;
        for(int i=0;i<256;i++){
            if(this->arr[i]>0){
                Node *n = new Node();
                n->freq = this->arr[i];
                n->ch = i;
                pq.push(n);
            }
        }

        return pq;
    }

    //Function to create a binary tree of type Node from the given priority queue
    Node* create_tree(priority_queue<Node*,vector<Node*>,CompareNode> &pq)
    {
        while(pq.size()>1)
        {
            Node *n1 = pq.top();
            pq.pop();

            Node *n2 = pq.top();
            pq.pop();

            Node *n3 = new Node();
            n3->freq = n1->freq + n2->freq;

            n3->left = n1;
            n3->right = n2;
            pq.push(n3);
        }

        Node *root = pq.top();
        pq.pop();
        return root;
    }


    // Functions to generate codes from the given tree
    void helper(Node *root,string str)
    {
        if(!root)
            return;

        if(root->ch!='\0'){
            ch_to_code[root->ch] = str;
        }

        helper(root->left,str+"0");
        helper(root->right,str+"1");
    }

    // Functions to generate codes from the given tree
    void generate_codes()
    {
        priority_queue<Node*,vector<Node*>,CompareNode> pq = make_queue();

        Node *root = create_tree(pq);

        string str = "";
        helper(root,str);

        for(auto i=ch_to_code.begin();i!=ch_to_code.end();i++){
            code_to_ch[i->second] = i->first;
        }
    }


    // taking a string as an input and counting frequency of each character.
    vector<char> counter(string file_name)
    {
        ifstream input_file(file_name);
        vector<char> full_text ;

        if(input_file.is_open()){
            string line;
            while(getline(input_file,line)){
                for(int i=0;i<line.length();i++){
                    arr[line[i]]++;
                    full_text.push_back(line[i]);
                }
                arr[10]++; // adding \n value
                full_text.push_back('\n');
            }
        }
        input_file.close();
        return full_text;

    }

    vector<char> padding(vector<char> text)
    {
        int num = text.size();
        unsigned int pad_value = 32-(num%32);



        for(int i=0;i<pad_value;i++){
            text.push_back('0');
        }
        string pad_info = bitset<32>(pad_value).to_string();
        for(int i=0;i<pad_info.length();i++){
            text.insert(text.begin(),pad_info[i]);
        }

        return text;

    }


    vector<unsigned int> build_byte_array(vector<char> padded_text)
    {
        vector<unsigned int> byte_arr;

        for(int i=0;i<padded_text.size();i+=32)
        {
            string byte="";
            for(int j=i;j<i+32;j++){
                byte += padded_text[j];
            }
            unsigned int b = stoul(byte,nullptr,2);
            //cout<<b<<":"<<byte<<endl;

            byte_arr.push_back(b);
        }

        return byte_arr;
    }


    vector<char> get_padded_from_byte_array(vector<unsigned int> &byte_arr)
    {
        vector<char> padded_text;

        for(int i=0;i<byte_arr.size();i++)
        {

        }
    }

public:
    Huffman()
    {
        for(int i=0;i<256;i++) arr[i] = 0;
    }


    void compress(string file_name)
    {
        vector<char> full_text = counter(file_name);

        generate_codes();

        vector<char> encoded_text;

        for(int i=0;i<full_text.size();i++){
            string temp = ch_to_code[full_text[i]];

            for(int j=0;j<temp.length();j++)
                encoded_text.push_back(temp[j]);
        }

        //Padding of Encoded text
        vector<char> padded_text = padding(encoded_text);

        vector<unsigned int> byte_array = build_byte_array(padded_text);




        ofstream output("compressed.bin",ios::binary);
        for(int i=0;i<byte_array.size();i++){
            unsigned int a = byte_array[i];
            char bytes = (char)a;
            output.write((char*)&bytes,sizeof(bytes));
        }

        output.close();

        cout<<file_name<<" is successfully compressed."<<endl;
    }



    void decompress(string file_name)
    {
        ifstream c_file(file_name,ios::binary);
        char data;

        vector<unsigned int> byte_arr;
        while(1)
        {
            c_file.read((char*)&data,sizeof(char));
            if(!c_file.good()) break;
            unsigned int a = (unsigned int)data;
            byte_arr.push_back(a);

            vector<char> padded_text = get_padded_from_byte_array(byte_arr);
        }





        /*
        string line;
        while(getline(c_file,line)){
            code+=line;
        }
        c_file.close();

        ofstream ret_file("ret.txt");
        string temp="";
        for(int i=0;i<code.length();i++)
        {
            temp+=code[i];
            if(code_to_ch[temp]!=0){
                ret_file<<code_to_ch[temp];
                temp = "";
            }
        }

        ret_file.close();

        cout<<"Decompressed"<<endl;


        */

    }
};


int main()
{


    Huffman h;
    h.compress("test.txt");
    //h.decompress("compressed.bin");





    /*ifstream test_file;
    test_file.open("test.txt");

    Huffman huff_test;

    if(test_file.is_open()){
        string line;
        while(getline(test_file,line)){
            huff_test.counter(line);
        }
    }
    test_file.close();

    unordered_map<char,string> test_codes = huff_test.generate_codes();

    ofstream bin_file("binary.bin",ios::binary);
    test_file.open("test.txt");

    string line;

    while(getline(test_file,line))
    {
        string output = "";
        for(int i=0;i<line.length();i++){
            output+=test_codes[line[i]];
        }

        bin_file.write((char*)&output,sizeof(string));
    }

    test_file.close();
    bin_file.close();

    unordered_map<string,char> code_to_ch;

    for(auto i=test_codes.begin();i!=test_codes.end();i++){
        code_to_ch[i->second] = i->first;
    }


    fstream file("binary.bin",ios::binary| ios ::in);
    ofstream ret("Retrieve.txt");

    string out;
    while(1){
        file.read((char*)&out,sizeof(string));
        string temp="";

            for(int i=0;i<=out.length();i++){
                if(code_to_ch[temp]!=0){
                    ret<<code_to_ch[temp];
                    temp="";
                }

                temp+=line[i];
            }

        if(!file.good()) break;
        cout<<out;
    }
    file.close();














    /*
    ofstream output_file("output.txt");

    test_file.open("test.txt");
    if(test_file.is_open()){
        string line;
        while(getline(test_file,line)){
            string output="";
            cout<<line;
            for(int i=0;i<line.length();i++){
                output+=test_codes[line[i]];
                //cout<<output;
            }
            output_file<<output;
            output_file<<"\n";

        }
    }

    test_file.close();
    output_file.close();




    /*
    unordered_map<string,char> code_to_ch;

    for(auto i=test_codes.begin();i!=test_codes.end();i++){
        code_to_ch[i->second] = i->first;
    }

    test_file.open("output.txt");
    ofstream ret("Retrieve.txt");

    if(test_file.is_open()){
        string line;
        while(getline(test_file,line)){
            string temp="";

            for(int i=0;i<=line.length();i++){
                if(code_to_ch[temp]!=0){
                    ret<<code_to_ch[temp];
                    temp="";
                }

                temp+=line[i];
            }

            ret<<"\n";
        }
    }

    test_file.close();
    ret.close();


    */







    /*
    string line;
    line = "This is test for huffman coding.";



    Huffman test;
    test.counter(line);
    unordered_map<char,string> test_codes = test.generate_codes();

    string output="";
    for(int i=0;i<line.length();i++){
        output+=test_codes[line[i]];
    }

    cout<<output<<endl;

    unordered_map<string,char> code_to_ch;

    for(auto i=test_codes.begin();i!=test_codes.end();i++){
        code_to_ch[i->second] = i->first;
    }
    string temp="";
    for(int i=0;i<=output.length();i++){
        if(code_to_ch[temp]!=0){
            cout<<code_to_ch[temp];
            temp="";
        }

        temp+=output[i];
    }
    */



}





