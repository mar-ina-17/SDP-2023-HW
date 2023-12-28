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
    double salary;
    vector<Node *> subordinates;

public:
    Node(const string nname)
    {
        this->name = nname;
    }
    Node(const Node *n)
    {
        this->name = n->getName();
        this->subordinates = n->getSubordinates();
    }

    const string &getName() const
    {
        return this->name;
    }
    const double &getSalary() const
    {
        return this->salary;
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
    string findBossOfSubordinates(const string subName)
    {
        unsigned int size = this->subordinates.size();
        for (unsigned int i = 0; i < size; i++)
        {
            if (this->subordinates[i]->getName() == subName)
            {
                return this->name;
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
        if (name != "CEO_to" && found)
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
        if (name != "CEO_to")
        {
            if (found)
            {
                Node *copyFound = new Node(found);
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
    void mergeHierarchies(Node *mergedHierarchy, const Node &hierarchy1, const Node &hierarchy2)
    {
        for (size_t i = 0; i < hierarchy2.getNumSubordinates(); ++i)
        {
            bool found = false;
            for (size_t j = 0; j < mergedHierarchy->getNumSubordinates(); ++j)
            {
                if (mergedHierarchy->getSubordinates()[j]->getName() == hierarchy2.getSubordinates()[i]->getName())
                {
                    Node *mergedSubordinate = mergedHierarchy->getSubordinates2()[j];

                    mergeHierarchies(mergedSubordinate, *mergedSubordinate, *hierarchy2.getSubordinates()[i]);
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                Node *copy = new Node(hierarchy2.getSubordinates()[i]);
                mergedHierarchy->getSubordinates2().push_back(copy);
            }
        }

        for (size_t i = 0; i < hierarchy2.getNumSubordinates(); ++i)
        {
            bool found = false;
            for (size_t j = 0; j < mergedHierarchy->getNumSubordinates(); ++j)
            {
                if (mergedHierarchy->getSubordinates()[j]->getName() == hierarchy2.getSubordinates()[i]->getName())
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                Node *copy = new Node(hierarchy2.getSubordinates()[i]);
                mergedHierarchy->getSubordinates2().push_back(copy);
            }
        }
    }

public:
    Tree(const string bossName)
    {
        this->boss = new Node(bossName);
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
        if (key != "CEO_to")
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
        cout << "Size of hierarchy is: " << getTreeSize(boss);
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

    void merge(Node *boss2)
    {
        if (boss != boss2)
        {
            mergeHierarchies(boss, *boss, *boss2);
        }
    }
};

void readFromFile(string &fileContent, string fn)
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
int main()
{
    string data, data2;
    readFromFile(data, "tree_data.txt");
    readFromFile(data2, "tree_data2.txt");

    Tree *tennis = new Tree("CEO_to");
    Tree *tennis2 = new Tree("CEO_to");
    tennis->buildTree(data);
    tennis2->buildTree(data2);
    tennis->printTree();
    tennis2->printTree();

    // 1 Да можете да проверите дали даден служител е част от дадена йерархия;
    //  tennis->doesEmployeeExist("Marina");
    //  tennis->doesEmployeeExist("CEO_to");
    //  tennis->doesEmployeeExist("Djokovic");

    // 2 За даден служител трябва да можете да кажете броя на преките му подчинени;
    //  tennis->directSubordinatesNum("CEO_to");
    //  tennis->directSubordinatesNum("Djokovic");
    //  tennis->directSubordinatesNum("marina");

    // 3 За даден служител трябва да можете да кажете името на прекия му ръководител;
    //  tennis->findBossOf("Djokovic");
    //  tennis->findBossOf("Dimitrov");
    //  tennis->findBossOf("CEO_to");
    //  tennis->findBossOf("marina");

    // 4 За дадена йерархия трябва да можете да кажете броя на всички служители в нея;
    // tennis->getSizeOfHierarchy();

    // 6 По подадена йерархия и име на служител трябва да уволните (премахнете) служителя, като всички негови подчинени стават съответно подчинени на ръководителя му. Разбира се, няма как да се уволни CEO-то;
    //  tennis->fireEmployeeByName("Alcaraz");
    //  tennis->fireEmployeeByName("marina");
    //  tennis->fireEmployeeByName("CEO_to");

    // 7 По подадена йерархия, име на служител и име на ръководител трябва да назначите служителя като подчинен на този ръководител.
    // tennis->employ("Nadal", "Marina");
    // tennis->employ("Dimitrov", "Nadal");
    // tennis->employ("Nadal", "CEO_to");

    // 8
    // tennis->numberOfEmployees("Dimitrov");
    // tennis->numberOfEmployees("Djokovic");
    // tennis->numberOfEmployees("CEO_to");
    // tennis->numberOfEmployees("marina");
    // tennis->printTree();

    // 9 Трябва да намирате броя на всички ръководители, които са претоварени - имат повече от N подчинени (преки или не). N е параметър на операцията;
    // tennis->overloads(0); -> 13
    // tennis->overloads(2); -> 5
    // tennis->overloads(10); ->1
    // tennis->overloads(15); -> 0

    // 10 За една фирма трябва да можете да кажете от колко човека се състои най-дългата верига от отношения ръководител-подчинен;
    // tennis->longest();

    // 11 Трябва да може да смятате заплатата на даден служител, като тя се определя по следната формула: 500 * <брой преки подчинени> + 50 * <брой непреки подчинени>;
    // tennis->calculateSalary("Dimitrov");
    // tennis->calculateSalary("CEO_to");
    // tennis->calculateSalary("Djokovic");
    // tennis->calculateSalary("Raonic");
    // tennis->calculateSalary("Marina");

    tennis->merge(tennis2->getBoss());
    tennis->printTree();
    return 0;
}
