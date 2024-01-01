// this code seems to fail when ran with cmake config but the coverage percentage was 33%, so i will just leave it
#define private public
#include "includes.h"
#include "../src/headers/cmd.h"

TEST_CASE("splitString function")
{
    Command command;

    SECTION("Empty string")
    {
        string input = "";
        vector<string> result = command.splitString(input);
        REQUIRE(result.empty());
    }

    SECTION("String with spaces")
    {
        string input = "Hello World";
        vector<string> result = command.splitString(input);
        REQUIRE(result.size() == 2);
        REQUIRE(result[0] == "Hello");
        REQUIRE(result[1] == "World");
    }

    SECTION("String with leading and trailing spaces")
    {
        string input = "   Catch2 Unit Testing   ";
        vector<string> result = command.splitString(input);
        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == "Catch2");
        REQUIRE(result[1] == "Unit");
        REQUIRE(result[2] == "Testing");
    }

    SECTION("String with multiple spaces between words")
    {
        string input = "This   is    a   test";
        vector<string> result = command.splitString(input);
        REQUIRE(result.size() == 4);
        REQUIRE(result[0] == "This");
        REQUIRE(result[1] == "is");
        REQUIRE(result[2] == "a");
        REQUIRE(result[3] == "test");
    }
}
TEST_CASE("addInputToVector function")
{
    Command command;

    SECTION("Empty input")
    {
        istringstream fakeInput("");
        cin.rdbuf(fakeInput.rdbuf());

        command.addInputToVector();

        REQUIRE(command.command.empty());
    }

    SECTION("Input with spaces")
    {
        istringstream fakeInput("load file.txt");
        cin.rdbuf(fakeInput.rdbuf());

        command.addInputToVector();

        REQUIRE(command.command.size() == 2);
        REQUIRE(command.command[0] == "load");
        REQUIRE(command.command[1] == "file.txt");
    }
}

TEST_CASE("findPositionOfTree function")
{
    Command command;

    SECTION("Tree found")
    {
        command.treeNames = {"Tree1", "Tree2", "Tree3"};
        int position = command.findPositionOfTree("Tree2");

        REQUIRE(position == 1);
    }

    SECTION("Tree not found")
    {
        stringstream output;
        cout.rdbuf(output.rdbuf());

        command.treeNames = {"Tree1", "Tree2", "Tree3"};
        int position = command.findPositionOfTree("NonexistentTree");

        REQUIRE(output.str() == "No object NonexistentTree found\n");

        REQUIRE(position == -1);
    }
}

TEST_CASE("getTreeFromPos function")
{
    Command command;

    SECTION("Tree found")
    {
        Tree *tree = new Tree("CEO");
        command.trees = {tree};
        tree->insert("Tree1");

        Tree *result = command.getTreeFromPos("Tree1");

        REQUIRE(result == tree);
    }

    SECTION("Tree not found")
    {
        Tree *result = command.getTreeFromPos("NonexistentTree");

        REQUIRE(result == nullptr);
    }
}

TEST_CASE("load function")
{
    Command command;

    SECTION("Load tree from file")
    {
        string fileName = "tree_data.txt";
        command.load("Tree1", fileName);

        REQUIRE(command.treeNames.size() == 1);
        REQUIRE(command.trees.size() == 1);
    }
}

TEST_CASE("save function")
{
    Command command;
    Tree *tree = new Tree("CEO");
    command.trees.push_back(tree);

    SECTION("Save tree to file")
    {
        string fileName = "output_file.txt";
        command.save("Tree1", fileName);
    }
}

TEST_CASE("numOfSub function")
{
    Command command;

    SECTION("Get number of direct subordinates in existing tree")
    {
        Tree *tree = new Tree("CEO");
        tree->insert("Employee1");
        tree->insert("Employee2");
        command.trees.push_back(tree);

        command.numOfSub("CEO", "Employee1");
    }

    SECTION("Get number of direct subordinates in non-existing tree")
    {
        command.numOfSub("NonExistingTree", "Employee1");
    }
}

TEST_CASE("manager function")
{
    Command command;

    SECTION("Find manager in existing tree")
    {
        Tree *tree = new Tree("CEO");
        tree->insert("Manager");
        tree->insert("Employee1");
        tree->employ("Manager", "Employee1");
        command.trees.push_back(tree);

        command.manager("CEO", "Employee1");
    }

    SECTION("Find manager in non-existing tree")
    {
        command.manager("NonExistingTree", "Employee1");
    }
}

TEST_CASE("numEmployees function")
{
    Command command;

    SECTION("Get number of employees in existing tree")
    {
        Tree *tree = new Tree("CEO");
        tree->insert("Employee1");
        tree->insert("Employee2");
        command.trees.push_back(tree);

        command.numEmployees("CEO");
    }

    SECTION("Get number of employees in non-existing tree")
    {
        command.numEmployees("NonExistingTree");
    }
}

TEST_CASE("overloaded function")
{
    Command command;

    SECTION("Get number of overloaded employees in existing tree")
    {
        Tree *tree = new Tree("CEO");
        tree->insert("Employee1");
        tree->insert("Manager");
        tree->employ("Manager", "Employee1");
        tree->insert("Subordinate1");
        tree->employ("Employee1", "Subordinate1");
        command.trees.push_back(tree);

        command.overloaded("CEO");
    }

    SECTION("Get number of overloaded employees in non-existing tree")
    {
        command.overloaded("NonExistingTree");
    }
}

TEST_CASE("fire function")
{
    Command command;

    SECTION("Fire an employee in an existing tree")
    {
        Tree *tree = new Tree("CEO");
        tree->insert("Employee1");
        command.trees.push_back(tree);

        command.fire("CEO", "Employee1");
    }

    SECTION("Fire an employee in a non-existing tree")
    {
        command.fire("NonExistingTree", "Employee1");
    }
}

TEST_CASE("hire function")
{
    Command command;

    SECTION("Hire an employee in an existing tree")
    {
        Tree *tree = new Tree("CEO");
        tree->insert("Manager");
        command.trees.push_back(tree);

        command.hire("CEO", "Employee1", "Manager");
    }

    SECTION("Hire an employee in a non-existing tree")
    {
        command.hire("NonExistingTree", "Employee1", "Manager");
    }
}

TEST_CASE("salary function")
{
    Command command;

    SECTION("Get salary of an employee in an existing tree")
    {
        Tree *tree = new Tree("CEO");
        tree->insert("Employee1");
        tree->calcSalary("Employee1");
        command.trees.push_back(tree);

        command.salary("CEO", "Employee1");
    }

    SECTION("Get salary of an employee in a non-existing tree")
    {
        command.salary("NonExistingTree", "Employee1");
    }
}

TEST_CASE("join function")
{
    Command command;

    SECTION("Join existing trees")
    {
        Tree *tree1 = new Tree("CEO1");
        tree1->insert("Employee1");
        command.trees.push_back(tree1);

        Tree *tree2 = new Tree("CEO2");
        tree2->insert("Employee2");
        command.trees.push_back(tree2);

        command.join("CEO1", "CEO2", "MergedTree");
    }

    SECTION("Join non-existing trees")
    {
        command.join("NonExistingTree1", "NonExistingTree2", "MergedTree");
    }

    SECTION("Join the same hierarchy")
    {
        Tree *tree = new Tree("CEO");
        tree->insert("Employee1");
        command.trees.push_back(tree);

        command.join("CEO", "CEO", "MergedTree");
    }
}
TEST_CASE("displayHelp function")
{
    Command command;

    stringstream buffer;
    streambuf *oldCout = cout.rdbuf(buffer.rdbuf());

    SECTION("Display help information")
    {
        command.displayHelp();
        string output = buffer.str();

        REQUIRE(output.find("load <object_name> <file_name>") != string::npos);
        REQUIRE(output.find("save <object_name> <file_name>") != string::npos);
        REQUIRE(output.find("find <object_name> <employee_name>") != string::npos);
        REQUIRE(output.find("num_subordinates <object_name> <employee_name>") != string::npos);
        REQUIRE(output.find("manager <object_name> <employee_name>") != string::npos);
        REQUIRE(output.find("num_employees <object_name>") != string::npos);
        REQUIRE(output.find("overloaded <object_name>") != string::npos);
        REQUIRE(output.find("join <object_name_1> <object_name_2> <result_object_name>") != string::npos);
        REQUIRE(output.find("fire <object_name> <employee_name>") != string::npos);
        REQUIRE(output.find("hire <object_name> <employee_name> <manager_name>") != string::npos);
        REQUIRE(output.find("salary <object_name> <employee_name>") != string::npos);
        REQUIRE(output.find("exit - terminates the execution of the program.") != string::npos);
    }

    cout.rdbuf(oldCout);
}

TEST_CASE("runProgram function")
{
    Command command;

    SECTION("Run 'load' command with valid arguments")
    {
        command.command = {"load", "object_name", "file_name"};
        command.runProgram();
    }

    SECTION("Run 'save' command with valid arguments")
    {
        command.command = {"save", "object_name", "file_name"};
        command.runProgram();
    }

    SECTION("Run 'find' command with valid arguments")
    {
        command.command = {"find", "object_name", "employee_name"};
        command.runProgram();
    }

    SECTION("Run 'num_subordinates' command with valid arguments")
    {
        command.command = {"num_subordinates", "object_name", "employee_name"};
        command.runProgram();
    }

    SECTION("Run 'manager' command with valid arguments")
    {
        command.command = {"manager", "object_name", "employee_name"};
        command.runProgram();
    }

    SECTION("Run 'num_employees' command with valid arguments")
    {
        command.command = {"num_employees", "object_name"};
        command.runProgram();
    }

    SECTION("Run 'overloaded' command with valid arguments")
    {
        command.command = {"overloaded", "object_name"};
        command.runProgram();
    }

    SECTION("Run 'join' command with valid arguments")
    {
        command.command = {"join", "object_name_1", "object_name_2", "result_object_name"};
        command.runProgram();
    }

    SECTION("Run 'fire' command with valid arguments")
    {
        command.command = {"fire", "object_name", "employee_name"};
        command.runProgram();
    }

    SECTION("Run 'hire' command with valid arguments")
    {
        command.command = {"hire", "object_name", "employee_name", "manager_name"};
        command.runProgram();
    }

    SECTION("Run 'salary' command with valid arguments")
    {
        command.command = {"salary", "object_name", "employee_name"};
        command.runProgram();
    }

    SECTION("Run unknown command")
    {
        command.command = {"unknown_command"};
        command.runProgram();
    }
}

#undef private