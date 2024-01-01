#ifndef CMD_H
#define CMD_H
#pragma once

#include <sstream>
#include <functional>

#include "tree.h"

class Command
{
private:
    string input;
    vector<string> treeNames;
    vector<string> command;
    vector<Tree *> trees;

    vector<string> splitString(const string &input)
    {
        vector<string> result;
        istringstream stream(input);
        string word;

        while (stream >> word)
        {
            result.push_back(word);
        }

        return result;
    }
    string toLowerCase(const string &str)
    {
        string result = str;
        for (char &c : result)
        {
            c = tolower(c);
        }
        return result;
    }

    void addInputToVector()
    {
        cout << "Run command: ";
        getline(cin, input);
        command = splitString(input);
    }

    void readFromFile(string &fileContent, string &fn)
    {
        ifstream inputFile(fn);

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

    void createTree(string &fileName)
    {
        string data;
        readFromFile(data, fileName);

        Tree *tree = new Tree("CEO_to");
        tree->buildTree(data);
        trees.push_back(tree);
        tree->printTree();
    }

    int findPositionOfTree(const string &name)
    {
        for (int i = 0; i < treeNames.size(); ++i)
        {
            if (treeNames[i] == name)
            {
                return i;
            }
        }

        cout << "No object " << name << " found" << endl;
        return -1;
    }

    Tree *getTreeFromPos(const string &treeName)
    {
        int pos = this->findPositionOfTree(treeName);
        if (pos > -1)
        {
            return this->trees[pos];
        }
        return nullptr;
    }

    void performIfNotNull(Tree *tree, const function<void(Tree *)> &operation)
    {
        if (tree)
        {
            operation(tree);
        }
    }

    void load(const string &a1, string &a2)
    {
        this->treeNames.push_back(a1);
        this->createTree(a2);
    }

    void save(const string &a1, const string &a2)
    {
        performIfNotNull(getTreeFromPos(a1), [&](Tree *tree)
                         { tree->saveTreeToFile(a2); });
    }

    void find(const string &a1, const string &a2)
    {
        performIfNotNull(getTreeFromPos(a1), [&](Tree *tree)
                         { tree->doesEmployeeExist(a2); });
    }

    void numOfSub(const string &a1, const string &a2)
    {
        performIfNotNull(getTreeFromPos(a1), [&](Tree *tree)
                         { tree->directSubordinatesNum(a2); });
    }

    void manager(const string &a1, const string &a2)
    {
        performIfNotNull(getTreeFromPos(a1), [&](Tree *tree)
                         { tree->findBossOf(a2); });
    }

    void numEmployees(const string &a1)
    {
        performIfNotNull(getTreeFromPos(a1), [&](Tree *tree)
                         { tree->getSizeOfHierarchy(); });
    }

    void overloaded(const string &a1)
    {
        performIfNotNull(getTreeFromPos(a1), [&](Tree *tree)
                         { tree->overloads(20); });
    }

    void join(const string &a1, const string &a2, const string &a3)
    {
        Tree *tree1 = getTreeFromPos(a1);
        Tree *tree2 = getTreeFromPos(a2);

        performIfNotNull(tree1, [&](Tree *t1)
                         { performIfNotNull(tree2, [&](Tree *t2)
                                            {
            Tree *mergedTree = mergedTree->merge(t1->getBoss(), t2->getBoss());
            if (mergedTree)
            {
                mergedTree->printTree();
                this->treeNames.push_back(a3);
                this->trees.push_back(mergedTree);
            }
            else
            {
                cout << "Error: Cannot merge the same hierarchy." << endl;
            } }); });
    }

    void fire(const string &a1, const string &a2)
    {
        performIfNotNull(getTreeFromPos(a1), [&](Tree *tree)
                         { tree->fireEmployeeByName(a2); });
    }

    void hire(const string &a1, const string &a2, const string &a3)
    {
        performIfNotNull(getTreeFromPos(a1), [&](Tree *tree)
                         { tree->employ(a3, a2); });
    }

    void salary(const string &a1, const string &a2)
    {
        performIfNotNull(getTreeFromPos(a1), [&](Tree *tree)
                         { tree->calculateSalary(a2); });
    }

public:
    Command()
    {
        this->input = "";
    }

    ~Command()
    {
        this->command.clear();
        this->treeNames.clear();
        for (size_t i = 0; i < this->trees.size(); i++)
        {
            delete this->trees[i];
        }
        this->trees.clear();
    }

    void inputCommand()
    {
        this->addInputToVector();
        this->runProgram();
        this->inputCommand();
    }

    void displayHelp()
    {
        cout << " .............. H E L P ...............\n\n";

        cout << "!!!! IMPORTANT !!!!\n";
        cout << "1. Your command should be structured as follows: [command] [args]\n";
        cout << "2. If the file you want to use is not placed in the same dir as the .h files, make sure to pass the file PATH not only the file name. Example:\n";
        cout << "File is in : root (.) of project - then pass ../fileName.txt\n";
        cout << "File is in : root/src of project - then pass ../src/fileName.txt\n\n";

        cout << "load <object_name> <file_name> - loads data for a hierarchy from a file with the given name and creates a tree associated with the object_name.\n";
        cout << "save <object_name> <file_name> - saves information about the hierarchy of the specified object to a file with the given name.\n";
        cout << "find <object_name> <employee_name> - checks if an employee with the specified name exists in the specified object.\n";
        cout << "num_subordinates <object_name> <employee_name> - displays the number of direct subordinates for the given employee in the specified object.\n";
        cout << "manager <object_name> <employee_name> - displays the name of the manager for the given employee in the specified object.\n";
        cout << "num_employees <object_name> - displays the number of employees in the specified object.\n";
        cout << "overloaded <object_name> - displays the number of employees in the specified object for which the count of subordinates (direct or indirect) is greater than 20.\n";
        cout << "join <object_name_1> <object_name_2> <result_object_name> - merges the two given objects into a new object with the name <result_object_name>.\n";
        cout << "fire <object_name> <employee_name> - removes the employee from the specified object.\n";
        cout << "hire <object_name> <employee_name> <manager_name> - appoints the employee to the specified object as a subordinate to the specified manager.\n";
        cout << "salary <object_name> <employee_name> - displays the salary of the specified employee in the specified object.\n";
        cout << "exit - terminates the execution of the program.\n";
        cout << "help - displays help.\n";
    }

    void runProgram()
    {
        if (command.empty())
        {
            cout << "ERROR! Empty command.\n";
            return;
        }

        string d = this->toLowerCase(command[0]);

        if (d == "exit")
        {
            exit(0);
        }
        else if (d == "load" && command.size() == 3)
        {
            load(command[1], command[2]);
        }
        else if (d == "save" && command.size() == 3)
        {
            save(command[1], command[2]);
        }
        else if (d == "find" && command.size() == 3)
        {
            find(command[1], command[2]);
        }
        else if (d == "num_subordinates" && command.size() == 3)
        {
            numOfSub(command[1], command[2]);
        }
        else if (d == "manager" && command.size() == 3)
        {
            manager(command[1], command[2]);
        }
        else if (d == "num_employees" && command.size() == 2)
        {
            numEmployees(command[1]);
        }
        else if (d == "overloaded" && command.size() == 2)
        {
            overloaded(command[1]);
        }
        else if (d == "join" && command.size() == 4)
        {
            join(command[1], command[2], command[3]);
        }
        else if (d == "fire" && command.size() == 3)
        {
            fire(command[1], command[2]);
        }
        else if (d == "hire" && command.size() == 4)
        {
            hire(command[1], command[2], command[3]);
        }
        else if (d == "salary" && command.size() == 3)
        {
            salary(command[1], command[2]);
        }
        else if (d == "help" && command.size() == 1)
        {
            displayHelp();
        }
        else
        {
            cout << "ERROR! Unknown command: " << d << endl;
        }

        cout << "--------------------------------------------------------\n";
    }
};

#endif