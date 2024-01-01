#define private public
#include "includes.h"
#include "../src/headers/tree.h"

TEST_CASE("Tree Constructor and Destructor")
{
    SECTION("Creating Tree with Boss")
    {
        Tree tree("CEO");

        REQUIRE(tree.getBoss() != nullptr);
        REQUIRE(tree.getBoss()->getName() == "CEO");
    }

    SECTION("Destroying Tree")
    {
        Tree *tree = new Tree("CEO");

        REQUIRE(tree->getBoss() != nullptr);
        REQUIRE(tree->getBoss()->getName() == "CEO");

        delete tree;

        REQUIRE(tree->getBoss() == nullptr);
    }
}

TEST_CASE("Getting Boss Node and Building Tree")
{
    SECTION("Getting Boss Node")
    {
        Tree tree("CEO");

        Node *boss = tree.getBoss();
        REQUIRE(boss != nullptr);
        REQUIRE(boss->getName() == "CEO");
    }

    SECTION("Building Tree from File Content")
    {
        Tree tree("CEO");

        string fileContent = "CEO-Sub1\n";
        fileContent += "CEO-Sub2\n";
        fileContent += "Sub1-Sub3\n";
        fileContent += "Sub1-Sub4\n";
        fileContent += "Sub2-Sub5\n";

        tree.buildTree(fileContent);

        Node *boss = tree.getBoss();
        REQUIRE(boss != nullptr);
        REQUIRE(boss->getName() == "CEO");

        const vector<Node *> &subordinates = boss->getSubordinates();
        REQUIRE(subordinates.size() == 2);
        REQUIRE(subordinates[0]->getName() == "Sub1");
        REQUIRE(subordinates[1]->getName() == "Sub2");

        const vector<Node *> &subs1 = subordinates[0]->getSubordinates();
        REQUIRE(subs1.size() == 2);
        REQUIRE(subs1[0]->getName() == "Sub3");
        REQUIRE(subs1[1]->getName() == "Sub4");

        const vector<Node *> &subs2 = subordinates[1]->getSubordinates();
        REQUIRE(subs2.size() == 1);
        REQUIRE(subs2[0]->getName() == "Sub5");
    }
}

TEST_CASE("Insert Node")
{
    SECTION("Inserting Node Recursively")
    {
        Tree tree("CEO");

        tree.insertRec(tree.getBoss(), "CTO");
        tree.insertRec(tree.getBoss(), "CFO");
        tree.insertRec(tree.getBoss()->getSubordinates()[0], "Developer");
        tree.insertRec(tree.getBoss()->getSubordinates()[0], "QA");
        tree.insertRec(tree.getBoss()->getSubordinates()[1], "Accountant");

        Node *boss = tree.getBoss();

        REQUIRE(boss != nullptr);
        REQUIRE(boss->getName() == "CEO");

        const vector<Node *> &subordinates = boss->getSubordinates();
        REQUIRE(subordinates.size() == 2);
        REQUIRE(subordinates[0]->getName() == "CTO");
        REQUIRE(subordinates[1]->getName() == "CFO");

        const vector<Node *> &ctoSubordinates = subordinates[0]->getSubordinates();
        REQUIRE(ctoSubordinates.size() == 2);
        REQUIRE(ctoSubordinates[0]->getName() == "Developer");
        REQUIRE(ctoSubordinates[1]->getName() == "QA");

        const vector<Node *> &cfoSubordinates = subordinates[1]->getSubordinates();
        REQUIRE(cfoSubordinates.size() == 1);
        REQUIRE(cfoSubordinates[0]->getName() == "Accountant");
    }

    SECTION("Inserting Node into Tree")
    {
        Tree tree("CEO1");

        tree.insert("CTO");
        tree.insert("CFO");

        Node *boss = tree.getBoss();

        REQUIRE(boss != nullptr);
        REQUIRE(boss->getName() == "CEO1");

        const vector<Node *> &subordinates = boss->getSubordinates();
        REQUIRE(subordinates.size() == 2);
        REQUIRE(subordinates[0]->getName() == "CTO");
        REQUIRE(subordinates[1]->getName() == "CFO");
    }
}

TEST_CASE("Search for a Node in the Tree")
{
    Tree tree("Grga Pitic");

    tree.insert("Ida");
    tree.insert("Dadan");

    SECTION("Search for an existing node")
    {
        const string key = "Dadan";
        Node *result = tree.search(key);

        REQUIRE(result != nullptr);
        REQUIRE(result->getName() == key);
    }

    SECTION("Search for a non-existing node")
    {
        const string key = "Nonexistent";
        Node *result = tree.search(key);

        REQUIRE(result == nullptr);
    }

    SECTION("Search with empty key")
    {
        const string key = "";
        Node *result = tree.search(key);

        REQUIRE(result == nullptr);
    }
}

TEST_CASE("Check if Employee Exists in the Tree")
{
    Tree tree("Grga Pitic");

    tree.insert("Ida");
    tree.insert("Dadan");

    SECTION("Employee exists in the hierarchy")
    {
        const string key = "Dadan";
        CAPTURE(key);
        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.doesEmployeeExist(key);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Employee Dadan exists in hierarchy.\n");
    }

    SECTION("Employee doesn't exist in the hierarchy")
    {
        const string key = "Nonexistent";
        CAPTURE(key);
        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.doesEmployeeExist(key);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Employee Nonexistent DOESN'T exist in hierarchy.\n");
    }
}

TEST_CASE("Direct Subordinates Number")
{
    Tree tree("Grga Pitic");

    SECTION("Employee exists with direct subordinates")
    {
        const string bossName = "Grga Pitic";
        const string sub1Name = "Ida";
        const string sub2Name = "Dadan";

        tree.insert(sub1Name);
        tree.insert(sub2Name);

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.directSubordinatesNum(bossName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Grga Pitic has 2 direct subordinates.\n");
    }

    SECTION("Employee exists with no direct subordinates")
    {
        const string bossName = "Grga Pitic";
        const string subName = "Ida";

        tree.insert(subName);

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.directSubordinatesNum(bossName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Grga Pitic has 1 direct subordinates.\n");
    }

    SECTION("Employee doesn't exist")
    {
        const string bossName = "Grga Pitic";
        const string nonExistingName = "NonExisting";

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.directSubordinatesNum(nonExistingName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "ERROR! Cannot display direct subordinates for NonExisting\n");
    }
}

TEST_CASE("Find Boss of Employee")
{
    Tree tree("Grga Pitic");

    SECTION("Employee exists with boss")
    {
        const string bossName = "Grga Pitic";
        const string subName = "Ida";

        tree.insert(subName);

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.findBossOf(subName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == " Boss of Ida is Grga Pitic\n");
    }

    SECTION("Employee exists without boss")
    {
        const string bossName = "Grga Pitic";
        const string subName = "Ida";

        tree.insert(bossName);

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.findBossOf(subName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "ERROR! Ida does not have a boss or doesn't exists.\n");
    }

    SECTION("Employee doesn't exist")
    {
        const string nonExistingName = "NonExisting";

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.findBossOf(nonExistingName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "ERROR! NonExisting does not have a boss or doesn't exists.\n");
    }

    SECTION("Employee is the boss")
    {
        const string bossName = "Grga Pitic";

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.findBossOf(bossName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "ERROR! Cannot display boss for Grga Pitic\n");
    }
}

TEST_CASE("Get Size of Hierarchy")
{
    Tree tree("Grga Pitic");

    SECTION("Hierarchy is empty")
    {
        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.getSizeOfHierarchy();

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Size of hierarchy is: 1\n");
    }

    SECTION("Hierarchy has one level of subordinates")
    {
        const string bossName = "Grga Pitic";
        const string sub1Name = "Ida";
        const string sub2Name = "Dadan";

        tree.insert(sub1Name);
        tree.insert(sub2Name);

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.getSizeOfHierarchy();

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Size of hierarchy is: 3\n");
    }

    SECTION("Hierarchy has multiple levels of subordinates")
    {
        const string bossName = "Grga Pitic";
        const string sub1Name = "Ida";
        const string sub2Name = "Dadan";
        const string sub3Name = "Zare";
        const string sub4Name = "Mara";

        tree.insert(sub1Name);
        tree.insert(sub2Name);
        tree.insert(sub3Name);
        tree.insert(sub4Name);

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.getSizeOfHierarchy();

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Size of hierarchy is: 5\n");
    }
}

TEST_CASE("Fire Employee by Name")
{
    Tree tree("Grga Pitic");

    SECTION("Fire existing employee")
    {
        const string bossName = "Grga Pitic";
        const string sub1Name = "Ida";
        const string sub2Name = "Dadan";

        tree.insert(sub1Name);
        tree.insert(sub2Name);

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.fireEmployeeByName(sub1Name);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Employee Ida was fired.\n");
    }

    SECTION("Fire non-existing employee")
    {
        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.fireEmployeeByName("Nonexistent");

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Employee Nonexistent COULDN'T be fired.\n");
    }

    SECTION("Fire boss (should fail)")
    {
        const string bossName = "Grga Pitic";

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.fireEmployeeByName(bossName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Employee Grga Pitic COULDN'T be fired.\n");
    }
}

TEST_CASE("Employing a new person")
{
    Tree tree("Grga Pitic");

    SECTION("Successfully employing a new person")
    {
        const string bossName = "Grga Pitic";
        const string employeeName = "Ida";

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.employ(bossName, employeeName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Ida is successfully employed under Grga Pitic\n");

        const size_t hierarchySize = tree.getTreeSize(tree.getBoss());
        REQUIRE(hierarchySize == 2);

        const Node *employee = tree.search(employeeName);
        REQUIRE(employee != nullptr);
        REQUIRE(employee->getName() == employeeName);
        REQUIRE(employee->getSubordinates().empty());
    }

    SECTION("Error in employing a new person (already exists)")
    {
        const string bossName = "Grga Pitic";
        const string employeeName = "Ida";

        tree.employ(bossName, employeeName);

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.employ(bossName, employeeName);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Ida is successfully employed under Grga Pitic\n");

        const size_t hierarchySize = tree.getTreeSize(tree.getBoss());
        REQUIRE(hierarchySize == 2);
    }
}

TEST_CASE("Overloads Count")
{
    SECTION("Counting overloads for an empty hierarchy")
    {
        Tree tree("Grga Pitic");

        const size_t n = 5;
        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.overloads(n);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Number of overloads over 5 is 0\n");

        const size_t hierarchySize = tree.getTreeSize(tree.getBoss());
        REQUIRE(hierarchySize == 1);
    }

    SECTION("Counting overloads for a hierarchy with multiple nodes")
    {
        Tree tree("Grga Pitic");

        tree.employ("Grga Pitic", "Ida");
        tree.employ("Grga Pitic", "Dadan");
        tree.employ("Ida", "Subordinate1");
        tree.employ("Subordinate1", "Subordinate2");

        const size_t n = 2;
        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.overloads(n);

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Number of overloads over 2 is 2\n");

        const size_t hierarchySize = tree.getTreeSize(tree.getBoss());
        REQUIRE(hierarchySize == 5);
    }
}

TEST_CASE("Longest Sequence of Employer-Employee Relationship")
{
    SECTION("Longest sequence for an empty hierarchy")
    {
        Tree tree("Grga Pitic");

        size_t max = 0;
        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.longest();

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Longest sequence of relationship employer-employee is 0\n");

        const size_t hierarchySize = tree.getTreeSize(tree.getBoss());
        REQUIRE(hierarchySize == 1);
    }

    SECTION("Longest sequence for a hierarchy with multiple nodes")
    {
        Tree tree("Grga Pitic");

        tree.employ("Grga Pitic", "Ida");
        tree.employ("Grga Pitic", "Dadan");
        tree.employ("Ida", "Subordinate1");
        tree.employ("Subordinate1", "Subordinate2");

        size_t max = 0;
        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.longest();

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Longest sequence of relationship employer-employee is 2\n");

        const size_t hierarchySize = tree.getTreeSize(tree.getBoss());
        REQUIRE(hierarchySize == 5);
    }
}

TEST_CASE("Calculate Salary")
{
    SECTION("Calculate salary for an employee with direct and non-direct subordinates")
    {
        Tree tree("Grga Pitic");

        tree.employ("Grga Pitic", "Ida");
        tree.employ("Grga Pitic", "Dadan");
        tree.employ("Ida", "Subordinate1");
        tree.employ("Subordinate1", "Subordinate2");

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.calculateSalary("Grga Pitic");

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Salary for Grga Pitic is 1100\n");

        const size_t hierarchySize = tree.getTreeSize(tree.getBoss());
        REQUIRE(hierarchySize == 5);
    }

    SECTION("Calculate salary for an employee with no subordinates")
    {
        Tree tree("Grga Pitic");

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.calculateSalary("Grga Pitic");

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "Salary for Grga Pitic is 0\n");

        const size_t hierarchySize = tree.getTreeSize(tree.getBoss());
        REQUIRE(hierarchySize == 1);
    }

    SECTION("Calculate salary for an employee that doesn't exist")
    {
        Tree tree("Grga Pitic");

        stringstream buffer;
        streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

        tree.calculateSalary("NonexistentEmployee");

        cout.rdbuf(oldCout);
        REQUIRE(buffer.str() == "ERROR! Employee NonexistentEmployee COULDN'T be found!\n");

        const size_t hierarchySize = tree.getTreeSize(tree.getBoss());
        REQUIRE(hierarchySize == 1);
    }
}

#undef private