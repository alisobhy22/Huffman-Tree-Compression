#include<iostream>
#include<string>
#include<bitset>
#include<queue>
#include<unordered_map>
#include<cmath>
#include <fstream>

using namespace std;

class Node {
public:
    char x;
    int counter;
    Node* left, * right;
};

Node* getNode(char x, int counter, Node* left, Node* right) {
    Node* tr = new Node();
    tr->x = x;
    tr->counter = counter;
    tr->left = left;
    tr->right = right;
    return tr;
}

class comp {
public:
    bool operator()(Node* left, Node* right) {
        return left->counter > right->counter;
    }
};

void encode(Node* root, string str, unordered_map<char, string>& huffman) {
    if (!root)
        return;

    if (!root->left && !root->right)
        huffman[root->x] = str;

    encode(root->left, str + "0", huffman);
    encode(root->right, str + "1", huffman);
}

string decode(Node* root, string q) {
    string answer = "";
    Node* temp = root;
    for (int i = 0; i < q.size(); i++) {
        if (q[i] == '0')
            temp = temp->left;
        else
            temp = temp->right;

        if (!temp->left && !temp->right) {
            answer += temp->x;
            temp = root;
        }

    }

    return answer + '\0';
}


void Decompress()
{
    ifstream input;
    input.open("Tree.txt");
    string line;
    vector<char> word;
    vector<string> codekey;
    while (!input.eof())
    {
        getline(input, line);
        if (line != "")

        {
            word.push_back(line[0]);
            string binary_code = "";
            for (int i = 0; i < line.size(); i++)
            {

                while ((line[i] != '0') && (line[i] != '1'))
                    i++;
                binary_code = binary_code + line[i];
            }
            codekey.push_back(binary_code);
        }
    }
    ifstream in;
    in.open("Compressed.bin", std::ofstream::binary);
    string line2;
    string compressed_text = "";
    while (!in.eof())
    {
        getline(in, line2);
        compressed_text = compressed_text + line2;
    }

    string bin = "";
    const int x = compressed_text.size();
    for (int i = 0; i < compressed_text.size(); i++)
    {
        bitset<8> temp(compressed_text[i]);
        bin = bin + temp.to_string();
    }

    int index;
    for (int i = 0; i < word.size(); i++)
        if (word[i] == EOF)
        {
             index = i;
        }
    int i = 0;
    string temp_2 = "";
    string decompressed = "";
    while (i != bin.size())
    {
        temp_2 = temp_2 + bin[i];
        if (temp_2 == codekey[index])
        {
            break;
        }
        for (int i = 0; i < codekey.size(); i++)
        {
            if (temp_2 == codekey[i])
            {
                decompressed = decompressed + word[i];
                temp_2 = "";
            }

        }
        i++;
    }
    ofstream out;
    out.open("Decompressed.txt");
    out << decompressed;
    out.close();

}

void buildHuffman(string txt) {

    unordered_map<char, int> counter;

    for (auto x : txt) {
        counter[x]++;
    }

    priority_queue<Node*, vector<Node*>, comp> tree;

    for (auto p : counter) {
        tree.push(getNode(p.first, p.second, NULL, NULL));
    }

    while (tree.size() != 1) {
        Node* left = tree.top(); tree.pop();
        Node* right = tree.top(); tree.pop();

        int sum = left->counter + right->counter;
        tree.push(getNode(NULL, sum, left, right));
    }

    Node* root = tree.top();


    unordered_map<char, string> huffman;
    encode(root, "", huffman);

    ofstream mtree;
    mtree.open("Tree.txt");

    for (auto p : huffman)
        mtree << p.first << " : " << p.second << endl;
    mtree.close();


    string rs = "";
    for (char x : txt)
        rs += huffman[x];



    ofstream out;
    out.open("Compressed.bin", std::ofstream::binary);
    int i = 0;
    string x = "";
    while (i < rs.size())
    {
        string bin;
        if (i + 8 < rs.size())
            for (int j = i; j < i + 8; j++)
                bin = bin + rs[j];
        else
            for (int j = i; j < rs.size(); j++)
            {
                bin = bin + rs[j];

            }
        i = i + 8;

        while (bin.size() % 8 != 0)
            bin = bin + '0';

        bitset<8> bits(bin);
        char cchar = bits.to_ulong();
        x = x + cchar;


    }
    out << x;
    out.close();


    int org = 8 * txt.length();
    int conv = ceil(rs.length() / 8);
    double compressionRate = (double)conv / org;
    cout << "String is compressed to " << compressionRate * 100 << "% of the total size." << endl;

}



int main()
{
    int answer;
    cout << "-------Text Compression Utility-------" << endl;
    cout << "1. Compress input folder" << endl;
    cout << "2. Decompress the compressed file" << endl;
    cout << "Type in your choice:";
    cin >> answer;
    while ((answer != 1) && (answer != 2))
    {
        cout << "Invalid input, try again;" << endl;
        cout << "Type in your choice:";
        cin >> answer;
    }
    if (answer == 1)
    {
        string str = "";
        char c = EOF;
        ifstream input;
        input.open("input.txt");
        string line;
        while (!input.eof())
        {
            getline(input, line);
            str = str + line;
        }
       // str = str + c;
            buildHuffman(str);
    }
    if (answer == 2)
        Decompress();
    cout << "Thank you for using our program!" << endl;
    return 0;
}