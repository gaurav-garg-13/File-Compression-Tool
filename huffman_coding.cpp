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
        return lhs->freq>rhs->freq;
    }
};



// Class for encapsulating Huffman Coding process
class Huffman
{
    int arr[256]; // to store frequency of characters

    string fileName; //to store name of File to compress

     //Hash map that stores character as key and code as value for compression
    unordered_map<char,string> ch_to_code;

    //Hash map that stores character as value and code as key for decompression
    unordered_map<string,char> code_to_ch;

public:
    //Constructor that initializes character counter to 0 and file name
    Huffman(string f_name)
    {
        this->fileName = f_name;
        for(int i=0;i<256;i++) arr[i] = 0;
    }

private:

    // taking a string as an input and counting frequency of each character.
    vector<char> counter()
    {
        ifstream input_file(this->fileName);
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

    //Function to make priority queue to help make binary tree
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
            this->ch_to_code[root->ch] = str;
        }

        helper(root->left,str+"0");
        helper(root->right,str+"1");
    }

    // Functions to generate codes from the given tree
    void generate_codes()
    {
        priority_queue<Node*,vector<Node*>,CompareNode> pq = make_queue();

        Node *root = create_tree(pq);

        helper(root,"");

        for(auto i=this->ch_to_code.begin();i!=this->ch_to_code.end();i++){
            this->code_to_ch[i->second] = i->first;
        }
    }


    //Generate Encoded text
    vector<char> get_encoded_text(vector<char> &full_text)
    {
        vector<char> en_text;
        for(int i=0;i<full_text.size();i++){
            string temp = ch_to_code[full_text[i]];

            for(int j=0;j<temp.length();j++)
                en_text.push_back(temp[j]);
        }
        return en_text;
    }


    // Padding the Encoded String
    vector<char> padding(vector<char> text)
    {

        int num = text.size();
        unsigned int pad_value = 32-(num%32);

        for(int i=0;i<pad_value;i++){
            text.push_back('0');
        }


        string pad_info = bitset<32>(pad_value).to_string();

        for(int i=pad_info.length()-1;i>=0;i--){
            text.insert(text.begin(),pad_info[i]);
        }

        return text;

    }


    //Function to build byte array from padded text
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

            byte_arr.push_back(b);
        }

        return byte_arr;
    }


    vector<char> get_padded_from_byte_array(vector<unsigned int> &byte_arr)
    {
        vector<char> padded_text;

        for(int i=0;i<byte_arr.size();i++)
        {
            string byte_value = bitset<32>(byte_arr[i]).to_string();
            for(int j=0;j<byte_value.size();j++){
                padded_text.push_back(byte_value[j]);
            }
        }

        return padded_text;
    }

    vector<char> remove_padding(vector<char> padded_text)
    {
        string num="";
        for(int i=0;i<32;i++){
            num+=padded_text[0];
            padded_text.erase(padded_text.begin());
        }

        unsigned int n = stoul(num,nullptr,2);

        for(int i=0;i<n;i++){
            padded_text.pop_back();
        }

        return padded_text;

    }

public:

    void compress()
    {
        //Counting frequency of characters
        vector<char> full_text = counter();

        //Generating codes
        generate_codes();

        //Generating the encoded string
        vector<char> encoded_text;
        encoded_text = get_encoded_text(full_text);

        //Padding of Encoded string
        vector<char> padded_text = padding(encoded_text);

        //Getting byte array for writing bytes to file
        vector<unsigned int> byte_array = build_byte_array(padded_text);

        //Writing the generated array to file
        ofstream output;
        output.open("Compressed.bin",ios::binary|ios::out);

        if(output.is_open()){
            for(int i=0;i<byte_array.size();i++){
                unsigned int temp = byte_array[i];
                output.write(reinterpret_cast<char*>(&temp),sizeof(temp));
            }
            output.close();
        }

        cout<<fileName<<" is successfully compressed."<<endl;
    }



    void decompress(string file_name)
    {
        //Reading from compressed file
        ifstream c_file(file_name,ios::binary|ios::in);
        vector<unsigned int> byte_arr;

        while(1)
        {
            unsigned int a;
            c_file.read((char*)&a,sizeof(a));
            if(!c_file.good()) break;
            byte_arr.push_back(a);
        }
        c_file.close();

        vector<char> padded_text = get_padded_from_byte_array(byte_arr);


        vector<char> encoded_text = remove_padding(padded_text);


        vector<char> text;

        for(auto i:ch_to_code){
            code_to_ch[i.second] = i.first;
        }

        string temp = "";
        for(int i=0;i<encoded_text.size();i++){
            temp+=encoded_text[i];
            if(code_to_ch[temp]!=0){
                text.push_back(code_to_ch[temp]);
                temp = "";

            }
        }

        ofstream r_file("Retrieved.txt");

        for(int i=0;i<text.size();i++){
            r_file<<text[i];
        }

        r_file.close();
        cout<<"Successfully decompressed."<<endl;







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


    Huffman h("test.txt");
    h.compress();
    h.decompress("Compressed.bin");
}





