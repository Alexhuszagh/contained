//  :copyright: (c) 2018 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <contained/forward_list.h>
#include <gtest/gtest.h>

// TESTS
// -----

// FORWARD LIST FACET

TEST(forward_list_facet, element_access)
{
    contained::forward_list<int> l = {1};
    auto& facet = l.facet();
    const auto& cfacet = l.facet();

    EXPECT_EQ(cfacet.front(), 1);
    facet.front() = 3;
    EXPECT_EQ(cfacet.front(), 3);
}


TEST(forward_list_facet, iterators)
{
    contained::forward_list<int> l;
    auto& f = l.facet();

    EXPECT_EQ(f.begin(), f.end());
    l = {3};
    EXPECT_NE(f.begin(), f.end());
    EXPECT_EQ(*f.begin(), 3);
}


TEST(forward_list_facet, capacity)
{
    contained::forward_list<int> l1;
    contained::forward_list<int> l2 = {1};
    auto& f1 = l1.facet();
    auto& f2 = l2.facet();

    EXPECT_TRUE(f1.empty());
    EXPECT_FALSE(f2.empty());
    EXPECT_GE(f1.max_size(), 0);
}

// TODO: operations
// splice after
// merge
// sort
// reverse

// FORWARD LIST


TEST(forward_list, element_access)
{
    // TODO:
}


TEST(forward_list, iterators)
{
    contained::forward_list<int> l;
    EXPECT_EQ(l.begin(), l.end());
    l = {3};
    EXPECT_NE(l.begin(), l.end());
    EXPECT_EQ(*l.begin(), 3);
}


TEST(forward_list, capacity)
{
    contained::forward_list<int> l1;
    contained::forward_list<int> l2 = {1};
    EXPECT_TRUE(l1.empty());
    EXPECT_FALSE(l2.empty());
    EXPECT_GE(l1.max_size(), 0);
}

// TODO: operations
// splice after
// merge
// sort
// reverse
