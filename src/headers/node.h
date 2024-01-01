
#ifndef NODE_H
#define NODE_H
#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Node
{
private:
    string name;
    double salary;
    vector<Node *> subordinates;

public:
    Node(const string &nname)
    {
        this->name = nname;
        this->salary = 0;
    }

    Node(const Node &n)
    {
        this->name = n.getName();
        this->salary = n.getSalary();
        for (size_t i = 0; i < n.getNumSubordinates(); ++i)
        {
            this->subordinates.push_back(new Node(*(n.getSubordinates()[i])));
        }
    }

    ~Node()
    {
        for (int i = 0; i < subordinates.size(); ++i)
        {
            if (subordinates[i] != nullptr)
            {
                delete subordinates[i];
                subordinates[i] = nullptr;
            }
        }
    }

    const string &getName() const
    {
        return this->name;
    }
    const double &getSalary() const
    {
        return this->salary;
    }
    void setSalary(const double &s)
    {
        if (s >= 0)
        {
            this->salary = s;
        }
    }

    const size_t getNumSubordinates() const
    {
        return this->subordinates.size();
    }
    const vector<Node *> &getSubordinates() const
    {
        return this->subordinates;
    }
    vector<Node *> &getSubordinates2()
    {
        return this->subordinates;
    }
    void pushToSubordinates(Node *sub)
    {
        if (sub)
        {
            this->subordinates.push_back(sub);
        }
    }

    Node *findInSubordinates(const string subName)
    {
        size_t size = this->subordinates.size();
        for (size_t i = 0; i < size; i++)
        {
            if (this->subordinates[i]->getName() == subName)
            {
                return this->subordinates[i];
            }
        }
        return nullptr;
    }
    string findBossOfSubordinates(const string &subName)
    {
        if (!subName.empty())
        {
            size_t size = this->subordinates.size();
            for (size_t i = 0; i < size; i++)
            {
                if (this->subordinates[i]->getName() == subName)
                {
                    return this->name;
                }
            }
        }
        return "";
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
#endif