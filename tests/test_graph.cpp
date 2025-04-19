#include <gtest/gtest.h>
#include "graph.h"

TEST(GraphTest, Construction) {
    Graph g(3);
    EXPECT_EQ(g.n, 3);
    for (int i = 0; i <= 3; ++i)
        for (int j = 0; j <= 3; ++j)
            EXPECT_EQ(g.adj[i][j], -1);
}

TEST(GraphTest, AddEdgeSymmetric) {
    Graph g(4);
    g.add_edge(1, 2, 7);
    EXPECT_EQ(g.adj[1][2], 7);
    EXPECT_EQ(g.adj[2][1], 7);

    EXPECT_EQ(g.adj[1][3], -1);
    EXPECT_EQ(g.adj[0][4], -1);
}

TEST(GraphTest, MultipleEdgesOverwrite) {
    Graph g(2);
    g.add_edge(0, 1, 5);
    EXPECT_EQ(g.adj[0][1], 5);
    g.add_edge(0, 1, 9);
    EXPECT_EQ(g.adj[0][1], 9);
}