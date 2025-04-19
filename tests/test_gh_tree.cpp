#include <gtest/gtest.h>
#include "graph.h"
#include "gh_tree.h"

TEST(GomoryHuTreeTest, TwoNode) {
    Graph g(2);
    g.add_edge(0, 1, 42);
    GomoryHuTree tree(g);
    int cut = tree.query(0, 1);
    EXPECT_EQ(cut, -1);
}

TEST(GomoryHuTreeTest, Disconnected) {
    Graph g(3);
    GomoryHuTree tree(g);
    EXPECT_EQ(tree.query(0, 2), -1);
}

TEST(GomoryHuTreeTest, CompleteGraphSmoke) {
    int n = 4;
    Graph g(n);
    for (int i = 0; i < n; ++i)
      for (int j = i + 1; j < n; ++j)
        g.add_edge(i, j, i + j);
    GomoryHuTree tree(g);
    EXPECT_NO_THROW({
      int x = tree.query(1, 3);
      (void)x;
    });
}