#include "includes.h"
#include "../src/headers/node.h"

TEST_CASE("Node Constructors and destructor")
{
    SECTION("Node is constructed with the given name and default salary")
    {
        string name = "Grga Pitic";
        Node node(name);

        REQUIRE(node.getName() == name);
        REQUIRE(node.getSalary() == 0);
    }

    SECTION("Node is copied with the same attributes")
    {
        Node original("Grga Pitic");
        original.setSalary(1000);

        Node *sub1 = new Node("Ida");
        Node *sub2 = new Node("Dadan");
        original.pushToSubordinates(sub1);
        original.pushToSubordinates(sub2);

        Node copy(original);

        REQUIRE(copy.getName() == original.getName());
        REQUIRE(copy.getSalary() == original.getSalary());
        REQUIRE(copy.getNumSubordinates() == original.getNumSubordinates());

        const vector<Node *> &originalSubordinates = original.getSubordinates();
        const vector<Node *> &copySubordinates = copy.getSubordinates();

        REQUIRE(copySubordinates.size() == originalSubordinates.size());

        for (size_t i = 0; i < originalSubordinates.size(); ++i)
        {
            REQUIRE(copySubordinates[i]->getName() == originalSubordinates[i]->getName());
        }
    }

    SECTION("Destructing a Node with Subordinates")
    {
        Node *sub1 = new Node("Ida");
        Node *sub2 = new Node("Dadan");

        Node *boss = new Node("Grga Pitic");
        boss->pushToSubordinates(sub1);
        boss->pushToSubordinates(sub2);

        REQUIRE(boss->getNumSubordinates() == 2);

        delete boss;
    }

    SECTION("Destructing a Node without Subordinates")
    {
        Node *leafNode = new Node("Leaf");

        delete leafNode;
    }
}

TEST_CASE("Getters and setters")
{
    SECTION("Node getName() returns the correct name")
    {
        string name = "Grga Pitic";
        Node node(name);

        REQUIRE(node.getName() == name);
    }

    SECTION("Node getSalary() returns the correct salary")
    {
        double salary = 1000.0;
        Node node("Ida");
        node.setSalary(salary);

        REQUIRE(node.getSalary() == salary);
    }

    SECTION("Node setSalary sets the salary correctly")
    {
        Node node("Grga Pitic");

        double salary = 1500.0;
        node.setSalary(salary);

        REQUIRE(node.getSalary() == Approx(salary));
    }

    SECTION("Node setSalary does not allow negative values")
    {
        Node node("Ida");

        double initialSalary = 1000.0;
        node.setSalary(initialSalary);

        double invalidSalary = -500.0;
        node.setSalary(invalidSalary);

        REQUIRE(node.getSalary() == Approx(initialSalary));
    }

    SECTION("Getting the number of subordinates")
    {
        Node node("Grga Pitic");

        Node *subordinate1 = new Node("Ida");
        Node *subordinate2 = new Node("Dadan");

        node.pushToSubordinates(subordinate1);
        node.pushToSubordinates(subordinate2);

        REQUIRE(node.getNumSubordinates() == 2);

        const vector<Node *> &subordinates = node.getSubordinates();
        REQUIRE(subordinates.size() == 2);
        REQUIRE(subordinates[0]->getName() == "Ida");
        REQUIRE(subordinates[1]->getName() == "Dadan");
    }

    SECTION("Getting const subordinates")
    {
        Node node("Grga Pitic");
        REQUIRE(node.getSubordinates().empty());

        Node *subordinate1 = new Node("Ida");
        Node *subordinate2 = new Node("Dadan");

        node.pushToSubordinates(subordinate1);
        node.pushToSubordinates(subordinate2);

        const vector<Node *> &subordinates = node.getSubordinates();

        REQUIRE(subordinates.size() == 2);
        REQUIRE(subordinates[0]->getName() == "Ida");
        REQUIRE(subordinates[1]->getName() == "Dadan");
    }

    SECTION("Getting non-const subordinates")
    {
        Node node("Grga Pitic");
        REQUIRE(node.getSubordinates2().empty());

        Node *subordinate1 = new Node("Ida");
        Node *subordinate2 = new Node("Dadan");

        node.pushToSubordinates(subordinate1);
        node.pushToSubordinates(subordinate2);

        vector<Node *> &subordinates = node.getSubordinates2();

        REQUIRE(subordinates.size() == 2);
        REQUIRE(subordinates[0]->getName() == "Ida");
        REQUIRE(subordinates[1]->getName() == "Dadan");
    }
}

TEST_CASE("Node pushToSubordinates")
{
    SECTION("Pushing a Subordinate to an Empty Node")
    {
        Node boss("Grga Pitic");
        Node *subordinate = new Node("Ida");

        boss.pushToSubordinates(subordinate);

        REQUIRE(boss.getNumSubordinates() == 1);
        REQUIRE(boss.getSubordinates()[0] == subordinate);
    }

    SECTION("Pushing Multiple Subordinates to a Node")
    {
        Node boss("Grga Pitic");
        Node *sub1 = new Node("Ida");
        Node *sub2 = new Node("Dadan");

        boss.pushToSubordinates(sub1);
        boss.pushToSubordinates(sub2);

        REQUIRE(boss.getNumSubordinates() == 2);
        REQUIRE(boss.getSubordinates()[0] == sub1);
        REQUIRE(boss.getSubordinates()[1] == sub2);
    }

    SECTION("Pushing Subordinates to a Node with Existing Subordinates")
    {
        Node boss("Grga Pitic");
        Node *sub1 = new Node("Ida");
        Node *sub2 = new Node("Dadan");

        boss.pushToSubordinates(sub1);

        boss.pushToSubordinates(sub2);

        REQUIRE(boss.getNumSubordinates() == 2);
        REQUIRE(boss.getSubordinates()[0] == sub1);
        REQUIRE(boss.getSubordinates()[1] == sub2);
    }
}

TEST_CASE("Node findInSubordinates")
{
    SECTION("Finding in Empty Subordinates")
    {
        Node boss("Grga Pitic");
        REQUIRE(boss.findInSubordinates("Ida") == nullptr);
    }

    SECTION("Finding Existing Subordinate by Name")
    {
        Node boss("Grga Pitic");
        Node *subordinate = new Node("Ida");
        boss.pushToSubordinates(subordinate);

        Node *foundSubordinate = boss.findInSubordinates("Ida");

        REQUIRE(foundSubordinate != nullptr);
        REQUIRE(foundSubordinate == subordinate);
    }

    SECTION("Finding Nonexistent Subordinate by Name")
    {
        Node boss("Grga Pitic");
        Node *subordinate = new Node("Ida");
        boss.pushToSubordinates(subordinate);

        Node *foundSubordinate = boss.findInSubordinates("Dadan");

        REQUIRE(foundSubordinate == nullptr);
    }

    SECTION("Finding in Multiple Subordinates")
    {
        Node boss("Grga Pitic");
        Node *sub1 = new Node("Ida");
        Node *sub2 = new Node("Dadan");
        Node *sub3 = new Node("Zare");

        boss.pushToSubordinates(sub1);
        boss.pushToSubordinates(sub2);
        boss.pushToSubordinates(sub3);

        Node *foundSubordinate = boss.findInSubordinates("Dadan");

        REQUIRE(foundSubordinate != nullptr);
        REQUIRE(foundSubordinate == sub2);
    }
}