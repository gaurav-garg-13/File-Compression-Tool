#include<unordered_map>
#include<queue>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
typedef pair<int, int> pi;

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

struct CompareNode
{
    bool operator()(Node* const &lhs,Node* const &rhs)
    {
        return lhs->freq>rhs->freq && lhs->ch<rhs->ch;
    }
};

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

unordered_map<char,string> generate_codes(Node *root)
{
    unordered_map<char,string> my_map;
    string str = "";
    helper(root,str,my_map);
    return my_map;
}

void print_tree(Node *root)
{
    queue<Node*> q;
    q.push(root);
    while (q.size()!=0){
        Node* temp = q.front();
        q.pop();
        cout<<"Root: "<<temp->ch<<":"<<temp->freq;
        if (temp->left!=NULL){
            cout<<";L "<<temp->left->ch<<":"<<temp->left->freq;
            q.push(temp->left);
        }
        if(temp->right!=NULL){
            cout<<";R "<<temp->right->ch<<":"<<temp->right->freq;
            q.push(temp->right);
        }
        cout<<endl;
    }
}
int main()
{
    string line;
    line = "This is test for huffman coding.";

    int a[256];
    for(int i=0;i<256;i++) a[i]=0;

    // Getting frequency of each character

    for(int i=0;i<line.length();i++){
        a[line[i]]++;
    }

    //Making priority queue of Nodes


    priority_queue<Node*,vector<Node*>,CompareNode> pq;

    for(int i=0;i<256;i++){
        if(a[i]>0){
            Node *n = new Node();
            n->freq = a[i];
            n->ch = i;
            pq.push(n);
        }
    }

    Node *test = pq.top();
   // cout<<"TEst"<<test->ch<<test->freq<<endl;

    //Make binary Tree

    Node *root = create_tree(pq);


   // print_tree(root);



    //Generate Code and store it in hasmap

    unordered_map<char,string> ch_to_code;
    ch_to_code = generate_codes(root);

    /*
    unordered_map<char,string>::iterator mine;
    mine = ch_to_code.begin();

    while(mine!=ch_to_code.end()){
        cout<<mine->first<<":"<<mine->second<<endl;
        mine++;
    }
    */

    //make another string with codes
    string output="";
    for(int i=0;i<line.length();i++){
        output+=ch_to_code[line[i]];
    }

    cout<<output<<endl;


    unordered_map<string,char> code_to_ch;

    for(auto i=ch_to_code.begin();i!=ch_to_code.end();i++){
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


    getchar();

}
