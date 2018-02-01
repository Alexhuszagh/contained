//  :copyright: (c) 2018 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <contained/string_view.h>
#include <gtest/gtest.h>
#include <utility>

namespace con = contained;

// DATA
// ----

// TODO: change to con::string
static const std::string STR = {0, 84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};
static const std::string NONNULL = {84, 104, 105, 115, 32, 105, 115, 32, 100, 97, 116, 97, 10};

// TESTS
// -----


TEST(string_view, constructors)
{
    // TODO: change to con::string_view str(STR);
    con::string_view str(STR.data(), STR.size());
    con::string_view other;
    EXPECT_EQ(str.size(), 14);

    str = con::string_view(STR.data());
    EXPECT_EQ(str.size(), 0);

    str = con::string_view(STR.data(), STR.size());
    EXPECT_EQ(str.size(), 14);
}


TEST(string_view, swap)
{
    // TODO: change to con::string_view str(STR);
    con::string_view str(STR.data(), STR.size());
    con::string_view other;
    EXPECT_EQ(str.size(), 14);
    EXPECT_EQ(other.size(), 0);

    swap(str, other);
    EXPECT_EQ(str.size(), 0);
    EXPECT_EQ(other.size(), 14);

    str.swap(other);
    EXPECT_EQ(str.size(), 14);
    EXPECT_EQ(other.size(), 0);
}


TEST(string_view, relational)
{
    // TODO: change to con::string_view str(STR);
    con::string_view str(STR.data(), STR.size());
    con::string_view other;

    EXPECT_TRUE(str == str);
    EXPECT_FALSE(str == other);
    EXPECT_FALSE(str != str);
    EXPECT_TRUE(str != other);
    EXPECT_FALSE(str < str);
    EXPECT_TRUE(str <= str);
    EXPECT_FALSE(str > str);
    EXPECT_TRUE(str >= str);
}


TEST(string_view, iterator)
{
    // TODO: change to con::string_view str(STR);
    con::string_view str(STR.data(), STR.size());
    std::string reversed(STR.rbegin(), STR.rend());
    EXPECT_EQ(std::string(str.begin(), str.end()), STR);
    EXPECT_EQ(std::string(str.rbegin(), str.rend()), reversed);
}


TEST(string_view, capacity)
{
    // TODO: change to con::string_view str(STR);
    con::string_view str(STR.data(), STR.size());
    con::string_view other;

    EXPECT_EQ(str.size(), 14);
    EXPECT_EQ(other.size(), 0);

    EXPECT_EQ(str.length(), 14);
    EXPECT_EQ(other.length(), 0);

    EXPECT_FALSE(str.empty());
    EXPECT_TRUE(other.empty());
}


TEST(string_view, element)
{
    // TODO: change to con::string_view str(STR);
    con::string_view str(STR.data(), STR.size());

    EXPECT_EQ(str[0], '\0');
    EXPECT_EQ(str.at(0), '\0');
    EXPECT_EQ(str[1], 'T');
    EXPECT_EQ(str.at(1), 'T');
    EXPECT_EQ(str.front(), '\0');
    EXPECT_EQ(str.back(), '\n');
}


TEST(string_view, operations)
{
    // TODO: change to con::string_view str(STR);
    con::string_view str(STR.data(), STR.size());
    con::string_view null;
    // TODO: change to con::string_view str(NONNULL);
    con::string_view non_null(NONNULL.data(), NONNULL.size());

// TODO: restore to con::string
    // find
    EXPECT_EQ(str.find(con::string_view("is")), 3);
    EXPECT_EQ(str.find("is"), 3);
//    EXPECT_EQ(str.find(std::string("is")), 3);
    EXPECT_EQ(str.find("is", 0, 2), 3);
    EXPECT_EQ(str.find('i'), 3);

    // find_first_of
    EXPECT_EQ(str.find_first_of(con::string_view("hsi")), 2);
    EXPECT_EQ(str.find_first_of("hsi"), 2);
//    EXPECT_EQ(str.find_first_of(std::string("hsi")), 2);
    EXPECT_EQ(str.find_first_of("hsi", 0, 3), 2);
    EXPECT_EQ(str.find_first_of('h'), 2);

    EXPECT_EQ(str.find_first_of(con::string_view("x")), con::string_view::npos);
    EXPECT_EQ(str.find_first_of("x"), con::string_view::npos);
//    EXPECT_EQ(str.find_first_of(std::string("x")), con::string_view::npos);
    EXPECT_EQ(str.find_first_of("x", 0, 1), con::string_view::npos);
    EXPECT_EQ(str.find_first_of('x'), con::string_view::npos);

    // find_first_not_of
    EXPECT_EQ(non_null.find_first_not_of(con::string_view("Tish")), 4);
    EXPECT_EQ(non_null.find_first_not_of("Tish"), 4);
//    EXPECT_EQ(non_null.find_first_not_of(std::string("Tish")), 4);
    EXPECT_EQ(non_null.find_first_not_of("Tish", 0, 4), 4);
    EXPECT_EQ(non_null.find_first_not_of('T'), 1);

    EXPECT_EQ(non_null.find_first_not_of(con::string_view("Thisdta \n")), con::string_view::npos);
    EXPECT_EQ(non_null.find_first_not_of("Thisdta \n"), con::string_view::npos);
//    EXPECT_EQ(non_null.find_first_not_of(std::string("Thisdta \n")), con::string_view::npos);
    EXPECT_EQ(non_null.find_first_not_of("Thisdta \n", 0, 9), con::string_view::npos);

    // rfind
    EXPECT_EQ(str.rfind(con::string_view("is")), 6);
    EXPECT_EQ(str.rfind("is"), 6);
//    EXPECT_EQ(str.rfind(std::string("is")), 6);
    EXPECT_EQ(str.rfind("is", 0, 2), con::string_view::npos);
    EXPECT_EQ(str.rfind('i'), 6);

    EXPECT_EQ(str.rfind(con::string_view("isx")), con::string_view::npos);
    EXPECT_EQ(str.rfind("isx"), con::string_view::npos);
//    EXPECT_EQ(str.rfind(std::string("isx")), con::string_view::npos);
    EXPECT_EQ(str.rfind("isx", 0, 3), con::string_view::npos);
    EXPECT_EQ(str.rfind('x'), con::string_view::npos);

    EXPECT_EQ(null.rfind('i'), con::string_view::npos);

    // find_last_of
    EXPECT_EQ(str.find_last_of(con::string_view("hsi")), 7);
    EXPECT_EQ(str.find_last_of("hsi"), 7);
//    EXPECT_EQ(str.find_last_of(std::string("hsi")), 7);
    EXPECT_EQ(str.find_last_of("hsi", 0, 3), con::string_view::npos);
    EXPECT_EQ(str.find_last_of('h'), 2);

    EXPECT_EQ(str.find_last_of(con::string_view("x")), con::string_view::npos);
    EXPECT_EQ(str.find_last_of("x"), con::string_view::npos);
//    EXPECT_EQ(str.find_last_of(std::string("x")), con::string_view::npos);
    EXPECT_EQ(str.find_last_of("x", 0, 1), con::string_view::npos);
    EXPECT_EQ(str.find_last_of('x'), con::string_view::npos);

    // find_last_not_of
    EXPECT_EQ(non_null.find_last_not_of(con::string_view("dat\n")), 7);
    EXPECT_EQ(non_null.find_last_not_of("dat\n"), 7);
//    EXPECT_EQ(non_null.find_last_not_of(std::string("dat\n")), 7);
    EXPECT_EQ(non_null.find_last_not_of("dat\n", 0, 4), 0);
    EXPECT_EQ(non_null.find_last_not_of('\n'), 11);

    EXPECT_EQ(non_null.find_last_not_of(con::string_view("Thisdta \n")), con::string_view::npos);
    EXPECT_EQ(non_null.find_last_not_of("Thisdta \n"), con::string_view::npos);
//    EXPECT_EQ(non_null.find_last_not_of(std::string("Thisdta \n")), con::string_view::npos);
    EXPECT_EQ(non_null.find_last_not_of("Thisdta \n", 0, 9), con::string_view::npos);
}
