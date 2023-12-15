#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
using namespace std;
class Node
{
private:
    string name;
    vector<Node *> subordinates;

public:
    Node(const string nname)
    {
        this->name = nname;
    }

    const string &getName() const
    {
        return this->name;
    }
    const vector<Node *> &getSubordinates() const
    {
        return this->subordinates;
    }

    void pushToSubordinates(Node *sub)
    {
        this->subordinates.push_back(sub);
    }

    Node *findInSubordinates(const string subName)
    {
        unsigned int size = this->subordinates.size();
        for (unsigned int i = 0; i < size; i++)
        {
            if (this->subordinates[i]->getName() == subName)
            {
                return this->subordinates[i];
            }
        }
        return nullptr;
    }

    void printNodeAndChildren()
    {
        cout << "Parent: " << this->name << endl;
        for (size_t i = 0; i < this->subordinates.size(); i++)
        {
            cout << "Child " << i << ":" << this->subordinates[i]->getName() << endl;
        }
    }
};

class Tree
{
private:
    Node *boss;
    Node *insertRec(Node *node, const string &value)
    {
        if (node == nullptr)
        {
            return new Node(value);
        }

        Node *child = new Node(value);
        node->pushToSubordinates(child);

        return node;
    }
    Node *searchRec(Node *node, const string &key)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (key == node->getName())
        {
            return node;
        }

        for (size_t i = 0; i < node->getSubordinates().size(); ++i)
        {
            Node *foundNode = searchRec(node->getSubordinates()[i], key);
            if (foundNode != nullptr)
            {
                return foundNode;
            }
        }

        return nullptr;
    }

    void buildTreeRecursive(Node *currentNode, const string &fileContent, size_t &start)
    {
        while (start < fileContent.size())
        {
            size_t pos = fileContent.find('-', start);

            string parent = fileContent.substr(start, pos - start);
            string child;

            if (pos != string::npos)
            {
                child = fileContent.substr(pos + 1, fileContent.find('\n', pos) - (pos + 1));
            }
            else
            {
                child = fileContent.substr(pos + 1);
            }

            Node *parentNode = search(parent);
            if (!parentNode)
            {
                parentNode = new Node(parent);
                currentNode->pushToSubordinates(parentNode);
            }

            Node *childNode = new Node(child);
            parentNode->pushToSubordinates(childNode);

            start = fileContent.find('\n', start);
            if (start != string::npos)
            {
                start++;
            }
            else
            {
                break;
            }

            buildTreeRecursive(parentNode, fileContent, start);
        }
    }

public:
    Tree(const string bossName)
    {
        this->boss = new Node(bossName);
    }

    void buildTree(const string &fileContent)
    {
        size_t start = 0;
        buildTreeRecursive(this->boss, fileContent, start);
    }

    void printTree(Node *node, int depth = 0)
    {
        cout << string(depth * 2, ' ');
        node->printNodeAndChildren();

        for (size_t i = 0; i < node->getSubordinates().size(); ++i)
        {
            printTree(node->getSubordinates()[i], depth + 1);
        }
    }

    void printTree()
    {
        cout << "Tree structure:" << endl;
        printTree(this->boss);
    }

    void insert(const string &value)
    {
        boss = insertRec(boss, value);
    }

    Node *search(const string &key)
    {
        return searchRec(boss, key);
    }
};

void readFromFile(string &fileContent)
{
    ifstream inputFile("tree_data.txt");

    if (!inputFile.is_open())
    {
        cerr << "Error opening file." << endl;
        return;
    }

    string line;

    while (getline(inputFile, line))
    {
        fileContent += line + "\n";
    }

    inputFile.close();
}
int main()
{
    string data;
    readFromFile(data);

    Tree *tennis = new Tree("CEO_to");
    tennis->buildTree(data);
    tennis->printTree();

    return 0;
}
