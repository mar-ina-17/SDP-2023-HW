
#ifndef TREE_H
#define TREE_H
#pragma once

#include <fstream>
#include <sstream>

#include "node.h"

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
    Node *searchBoss(Node *rootNode, const string &childName)
    {
        if (rootNode == nullptr)
        {
            return nullptr;
        }

        for (size_t i = 0; i < rootNode->getSubordinates().size(); ++i)
        {
            Node *child = rootNode->getSubordinates()[i];
            if (child->getName() == childName)
            {
                return rootNode;
            }
        }

        for (size_t i = 0; i < rootNode->getSubordinates().size(); ++i)
        {
            Node *child = rootNode->getSubordinates()[i];
            Node *parent = searchBoss(child, childName);
            if (parent != nullptr)
            {
                return parent;
            }
        }

        return nullptr;
    }
    bool removeNode(Node *parent, const string &nodeName)
    {
        if (parent == nullptr)
        {
            return false;
        }

        for (size_t i = 0; i < parent->getSubordinates().size(); ++i)
        {
            if (parent->getSubordinates()[i]->getName() == nodeName)
            {
                parent->getSubordinates2().erase(parent->getSubordinates().begin() + i);
                return true;
            }
        }

        for (size_t i = 0; i < parent->getSubordinates().size(); ++i)
        {
            if (removeNode(parent->getSubordinates()[i], nodeName))
            {
                return true;
            }
        }

        return false;
    }

    const int numberOfEmployeesUnder(const string &key)
    {
        Node *found = this->search(key);
        if (found)
        {
            return this->getTreeSize(found) - 1;
        }
        else
        {
            return -1;
        }
    }
    const size_t getTreeSize(Node *root)
    {
        if (root == nullptr)
        {
            return 0;
        }

        size_t size = 1;

        for (size_t i = 0; i < root->getSubordinates().size(); ++i)
        {
            Node *child = root->getSubordinates()[i];
            size += getTreeSize(child);
        }

        return size;
    }

    bool fireEmployee(const string &name)
    {
        Node *found = searchRec(this->boss, name);
        if (name != this->boss->getName() && found)
        {
            vector<Node *> subs = found->getSubordinates();

            Node *bossOfFound = searchBoss(this->boss, name);
            for (size_t i = 0; i < subs.size(); i++)
            {
                bossOfFound->pushToSubordinates(subs[i]);
            }

            return removeNode(this->boss, name);
        }
        return false;
    }
    bool employPerson(const string &bossName, const string &name)
    {
        Node *found = searchRec(this->boss, name);
        Node *findBoss = searchRec(this->boss, bossName);
        if (name != this->boss->getName())
        {
            if (found)
            {
                Node *copyFound = new Node(*found);
                this->fireEmployee(name);
                findBoss->pushToSubordinates(copyFound);
                return true;
            }
            else
            {
                Node *newEmployee = new Node(name);
                findBoss->pushToSubordinates(newEmployee);
                return true;
            }
        }
        return false;
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
    void numberOfOverloads(const size_t n, size_t &count, Node *curr)
    {
        if (curr == nullptr)
        {
            return;
        }
        int num = numberOfEmployeesUnder(curr->getName());
        count = num >= n ? count + 1 : count;

        for (size_t i = 0; i < curr->getSubordinates().size(); ++i)
        {
            Node *child = curr->getSubordinates()[i];
            numberOfOverloads(n, count, child);
        }
    }
    void longestSequeceEmployer(size_t &max, Node *curr)
    {
        if (curr == nullptr)
        {
            return;
        }
        int num = curr->getNumSubordinates();
        max = num >= max ? num : max;

        for (size_t i = 0; i < curr->getSubordinates().size(); ++i)
        {
            Node *child = curr->getSubordinates()[i];
            longestSequeceEmployer(max, child);
        }
    }
    double calcSalary(const string &name)
    {
        Node *find = searchRec(this->boss, name);
        if (find)
        {
            int direct = find->getNumSubordinates(),
                nonDirect = numberOfEmployeesUnder(name) - direct;
            return 500 * direct + 50 * nonDirect;
        }
        else
        {
            cout << "ERROR! Employee " << name << " COULDN'T be found!\n";
            return -1;
        }
        return 0;
    }
    void mergeHierarchies(Node *mergedBoss, const Node &hierarchy1, const Node &hierarchy2)
    {
        for (size_t i = 0; i < hierarchy2.getNumSubordinates(); ++i) // purvo obhojda
        {
            bool found = false;

            for (size_t j = 0; j < mergedBoss->getNumSubordinates(); ++j) // ako ima zapisi v izhodnoto durvo shte proveri dali veche ne sushtestvuva tekushtiq ot h2
            {
                if (mergedBoss->getSubordinates2()[j]->getName() == hierarchy2.getSubordinates()[i]->getName())
                {
                    Node *mergedSubordinate = mergedBoss->getSubordinates2()[j];

                    // shte se vikne rekursivno za da produlji s dobavqneto na sub-ovete na h1 i h2
                    mergeHierarchies(mergedSubordinate, *hierarchy1.getSubordinates()[i], *hierarchy2.getSubordinates()[i]);

                    found = true;
                    break;
                }
            }

            // ako ne e nameren zapis ili kato cqlo nqma zapisi v izhodnoto durvo, prosto shte se dobavi tekushtiq ot h2
            if (!found)
            {
                Node *copy = new Node(*hierarchy2.getSubordinates()[i]);
                mergedBoss->getSubordinates2().push_back(copy);
            }
        }

        // dobavq subovete ot h2
        for (size_t i = 0; i < hierarchy2.getNumSubordinates(); ++i)
        {
            bool found = false;

            // proverqvame dali sub-a ot h2 ne susht v izhodnoto durvo
            for (size_t j = 0; j < mergedBoss->getNumSubordinates(); ++j)
            {
                if (mergedBoss->getSubordinates()[j]->getName() == hierarchy2.getSubordinates()[i]->getName())
                {
                    found = true;
                    break;
                }
            }

            // ako ne e nameren zapis ili kato cqlo nqma zapisi v izhodnoto durvo, prosto shte se dobavi tekushtiq ot h2
            if (!found)
            {
                Node *copy = new Node(*hierarchy2.getSubordinates()[i]);
                mergedBoss->getSubordinates2().push_back(copy);
            }
        }
    }

public:
    Tree(const string bossName)
    {
        this->boss = new Node(bossName);
    }

    ~Tree()
    {
        delete boss;
        boss = nullptr;
    }

    Node *getBoss() const
    {
        return this->boss;
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
        cout << "---------- END PRINT ----------\n";
    }

    void insert(const string &value)
    {
        boss = insertRec(boss, value);
    }

    Node *search(const string &key)
    {
        if (key.empty())
        {
            cout << "ERROR! Search not possible - key is empty!\n";
            return nullptr;
        }
        return searchRec(boss, key);
    }

    // checks is employee is part of the hierarchy
    void doesEmployeeExist(const string &key)
    {
        Node *n = search(key);
        if (n)
        {
            cout << "Employee " << n->getName() << " exists in hierarchy.\n";
        }
        else
        {
            cout << "Employee " << key << " DOESN'T exist in hierarchy.\n";
        }
    }

    // retrieve number of direct subordinates
    void directSubordinatesNum(const string &key)
    {
        Node *n = search(key);
        if (n)
        {
            cout << key << " has " << n->getNumSubordinates() << " direct subordinates.\n";
        }
        else
        {
            cout << "ERROR! Cannot display direct subordinates for " << key << endl;
        }
    }

    // find boss of employee
    void findBossOf(const string &key)
    {
        if (key != this->boss->getName())
        {
            Node *n = searchBoss(boss, key);

            if (n)
            {
                cout << " Boss of " << key << " is " << n->getName() << "\n";
            }
            else
            {
                cout << "ERROR! " << key << " does not have a boss or doesn't exists.\n";
            }
        }
        else
        {
            cout << "ERROR! Cannot display boss for " << key << endl;
        }
    }

    // get size of whole hierarchy
    void getSizeOfHierarchy()
    {
        cout << "Size of hierarchy is: " << getTreeSize(boss) << endl;
    }

    // fire employee
    void fireEmployeeByName(const string &name)
    {
        if (this->fireEmployee(name))
        {
            cout << "Employee " << name << " was fired." << endl;
        }
        else
        {
            cout << "Employee " << name << " COULDN'T be fired." << endl;
        }
    }

    // employ person
    void employ(const string &boss, const string &name)
    {
        if (this->employPerson(boss, name))
        {
            cout << name << " is successfully employed under " << boss << endl;
        }
        else
        {
            cout << "ERROR! Employment of " << name << " under " << boss << " was unsuccessful!" << endl;
        }
    }

    // count overloads
    void overloads(const size_t &n)
    {
        size_t num = 0;
        numberOfOverloads(n, num, this->boss);
        cout << "Number of overloads over " << n << " is " << num << endl;
    }

    // find longest sequence
    void longest()
    {
        size_t num = 0;
        longestSequeceEmployer(num, this->boss);
        cout << "Longest sequence of relationship employer-employee is " << num << endl;
    }

    // calculate salary
    void calculateSalary(const string &name)
    {
        double salary = calcSalary(name);
        if (salary > -1)
        {
            cout << "Salary for " << name << " is " << salary << endl;
        }
    }

    Tree *merge(Node *boss1, Node *boss2)
    {
        if (boss1 != boss2)
        {
            Tree *mergedTree = new Tree("CEO_to");

            mergedTree->mergeHierarchies(mergedTree->getBoss(), *boss1, *boss2);
            return mergedTree;
        }
        return nullptr;
    }

    void saveTreeToFile(Node *currentNode, ofstream &outputFile)
    {
        if (currentNode)
        {
            const vector<Node *> &subordinates = currentNode->getSubordinates();
            for (size_t i = 0; i < subordinates.size(); ++i)
            {
                if (!subordinates[i]->getName().empty())
                {
                    outputFile << currentNode->getName() << "-" << subordinates[i]->getName() << endl;
                }
            }

            for (size_t i = 0; i < subordinates.size(); ++i)
            {
                saveTreeToFile(subordinates[i], outputFile);
            }
        }
    }

    void saveTreeToFile(const string &filename)
    {
        ofstream outputFile(filename);

        if (!outputFile.is_open())
        {
            cerr << "Error opening the file: " << filename << endl;
            return;
        }
        saveTreeToFile(this->boss, outputFile);

        outputFile.close();
    }
};

#endif