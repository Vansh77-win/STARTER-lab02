#include "intbst.h"

#include <iostream>
#include <sstream>
#include <string>

namespace {

int failures = 0;

void check(bool condition, const std::string& description) {
    if (!condition) {
        std::cerr << "FAIL: " << description << '\n';
        ++failures;
    }
}

std::string captureOutput(void (IntBST::*printMethod)() const, const IntBST& tree) {
    std::ostringstream output;
    std::streambuf* original = std::cout.rdbuf(output.rdbuf());
    (tree.*printMethod)();
    std::cout.rdbuf(original);
    return output.str();
}

void testConstructionAndInsertion() {
    IntBST tree;
    check(captureOutput(&IntBST::printInOrder, tree).empty(),
          "empty in-order traversal has no output");
    check(captureOutput(&IntBST::printPostOrder, tree).empty(),
          "empty post-order traversal has no output");
    check(tree.count() == 0, "new tree has no nodes");
    check(tree.sum() == 0, "new tree has sum zero");
    check(!tree.contains(10), "new tree does not contain values");
    check(tree.insert(10), "insert into empty tree");
    check(!tree.insert(10), "duplicate insertion is rejected");
    check(tree.insert(5), "insert left child");
    check(tree.insert(15), "insert right child");
    check(tree.count() == 3, "count after insertion");
    check(tree.sum() == 30, "sum after insertion");
}

void testTraversalsAndQueries() {
    IntBST tree;
    const int values[] = {64, 128, 8, 512, 256, 32, 16, 4};
    for (int value : values) {
        check(tree.insert(value), "insert test-tree value");
    }

    check(captureOutput(&IntBST::printPreOrder, tree) ==
              "64 8 4 32 16 128 512 256 \n",
          "pre-order traversal");
    check(captureOutput(&IntBST::printInOrder, tree) ==
              "4 8 16 32 64 128 256 512 ",
          "in-order traversal");
    check(captureOutput(&IntBST::printPostOrder, tree) ==
              "4 16 32 8 256 512 128 64 ",
          "post-order traversal");

    check(tree.sum() == 1020, "sum of test tree");
    check(tree.count() == 8, "count of test tree");
    check(tree.contains(64), "contains root");
    check(tree.contains(4), "contains smallest value");
    check(!tree.contains(17), "does not contain missing value");

    check(tree.getPredecessor(64) == 32, "predecessor of root");
    check(tree.getPredecessor(512) == 256, "predecessor with left subtree");
    check(tree.getPredecessor(128) == 64,
          "predecessor from an ancestor when target has no left subtree");
    check(tree.getPredecessor(16) == 8,
          "predecessor follows ancestors through left branches");
    check(tree.getPredecessor(4) == 0, "smallest value has no predecessor");
    check(tree.getPredecessor(17) == 0, "missing value has no predecessor");
    check(tree.getSuccessor(64) == 128, "successor of root");
    check(tree.getSuccessor(4) == 8, "successor with ancestor");
    check(tree.getSuccessor(512) == 0, "largest value has no successor");
    check(tree.getSuccessor(17) == 0, "missing value has no successor");
}

void testRemovalAndDestruction() {
    {
        IntBST tree;
        const int values[] = {64, 128, 8, 512, 256, 32, 16, 4};
        for (int value : values) {
            tree.insert(value);
        }

        check(tree.remove(4), "remove leaf");
        check(tree.remove(64), "remove node with two children");
        check(tree.remove(128), "remove node with one child");
        check(!tree.remove(128), "removing a missing value returns false");
        check(captureOutput(&IntBST::printInOrder, tree) ==
                  "8 16 32 256 512 ",
              "tree remains ordered after removals");
        check(tree.count() == 5, "count after removals");
        check(!tree.contains(4) && !tree.contains(64) && !tree.contains(128),
              "removed values are absent");
    }

    {
        IntBST tree;
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        check(tree.remove(10), "remove root with two children");
        check(captureOutput(&IntBST::printInOrder, tree) == "5 15 ",
              "root removal preserves both subtrees");
    }

    {
        IntBST tree;
        tree.insert(10);
        tree.insert(5);
        tree.insert(2);
        check(tree.remove(5), "remove non-root node with one child");
        check(tree.contains(2), "child is reconnected after non-root removal");
        check(tree.count() == 2, "count after non-root removal");
    }

    {
        IntBST tree;
        check(tree.insert(10), "insert singleton for removal");
        check(tree.remove(10), "remove root from singleton tree");
        check(!tree.remove(10), "removing absent value returns false");
        check(tree.count() == 0, "tree is empty after root removal");
    }

    {
        IntBST tree;
        check(tree.insert(10), "insert root for one-child removal");
        check(tree.insert(5), "insert root's child");
        check(tree.remove(10), "remove root with one child");
        check(tree.count() == 1, "one child remains after root removal");
        check(tree.contains(5), "remaining child becomes root");
    }
}

}  // namespace

int main() {
    testConstructionAndInsertion();
    testTraversalsAndQueries();
    testRemovalAndDestruction();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return 1;
    }

    std::cout << "All IntBST tests passed\n";
    return 0;
}
