#include<unordered_map>
#include<queue>
#include<iostream>
#include<fstream>
#include<string>
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
    void helper(Node *root,string str,unordered_map<char,string> &my_map)
    {
        if(!root)
            return;

        if(root->ch!='\0'){
            my_map[root->ch] = str;
        }

        helper(root->left,str+"0",my_map);
        helper(root->right,str+"1",my_map);
    }

public:
    Huffman()
    {
        for(int i=0;i<256;i++) arr[i] = 0;
    }

    // taking a string as an input and counting frequency of each character.
    void counter(string line)
    {
        for(int i=0;i<line.length();i++)
            arr[line[i]]++;
    }


    // Functions to generate codes from the given tree
    unordered_map<char,string> generate_codes()
    {
        priority_queue<Node*,vector<Node*>,CompareNode> pq = make_queue();

        Node *root = create_tree(pq);

        unordered_map<char,string> my_map;
        string str = "";
        helper(root,str,my_map);
        return my_map;
    }
};


int main()
{
    ifstream test_file;
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





