#pragma once

#include "table.h"
#include "polynomial.h"
#include <gtest/gtest.h>

TEST(_table, can_create_cell) {
	ASSERT_NO_THROW(t_cell<polynomial> c(9, polynomial("-3+x^3yz+z^2")));

	ASSERT_NO_THROW(t_cell<polynomial> c(std::pair(2, polynomial("xy-z+5.1z^2"))));

	t_cell<polynomial> c(0, polynomial("x+1"));
	ASSERT_NO_THROW(t_cell<polynomial> c1(c));

	ASSERT_NO_THROW(t_cell<polynomial> c1(t_cell<polynomial>(1, polynomial("y-z^4"))));
}

TEST(unordered_table, can_create_unordered_table) {
	ASSERT_NO_THROW(unordered_table<polynomial> t);

	t_cell<polynomial> c0(3, polynomial("-30.2+xyz")), c1(0, polynomial("7x^9-3yz^2")),
		c2(1, polynomial("-5.5xy-z"));
	ASSERT_NO_THROW(unordered_table<polynomial> t({c0, c1, c2}));

	unordered_table<polynomial> t({ c0, c1, c2 });
	ASSERT_NO_THROW(unordered_table<polynomial> t1(t));
}

TEST(unordered_table, can_insert) {
	t_cell<polynomial> c0(3, polynomial("-30.2+xyz")), c1(0, polynomial("7x^9-3yz^2")),
		c2(1, polynomial("-5.5xy-z"));
	unordered_table<polynomial> t0({ c0, c1, c2 }), t1({ c0 });

	ASSERT_NO_THROW(t1.insert(c1.key, c1.value));
	t1.insert(1, polynomial("-5.5xy-z"));
	
	EXPECT_EQ(t1.size, t0.size);
	for (int i = 0; i < t1.size; ++i) 
		EXPECT_EQ(t1.table[i] == t0.table[i], true);

	ASSERT_ANY_THROW(t0.insert(c2.key, polynomial("3x^2+x+1")));
}

TEST(unordered_table, can_find) {
	t_cell<polynomial> c0(3, polynomial("-0.2+x^3yz-5y")), c1(0, polynomial("x^4z-2yz^2+4")),
		c2(1, polynomial("-0.5y-z+x"));
	unordered_table<polynomial> t0({ c0, c1, c2 });

	ASSERT_NO_THROW(t0.find(c1.key));
	EXPECT_EQ(*(t0.find(c1.key)) == c1, true);

	EXPECT_EQ(t0.find(2), nullptr);

}

TEST(unordered_table, can_erase) {
	t_cell<polynomial> c0(3, polynomial("-30.2+xyz")), c1(0, polynomial("7x^9-3yz^2")),
		c2(1, polynomial("-5.5xy-z"));
	unordered_table<polynomial> t0({ c0, c1, c2 }), t1({ c1, c2 }), t2({c1});

	ASSERT_NO_THROW(t0.erase(c0.key));

	EXPECT_EQ(t1.size, t0.size);
	t0.erase(c2.key);
	EXPECT_EQ(t2.size, t0.size);

	ASSERT_ANY_THROW(t0.erase(c2.key));
}

TEST(unordered_table, can_get_size) {
	t_cell<polynomial> c0(3, polynomial("-30.2+xyz")), c1(0, polynomial("7x^9-3yz^2")),
		c2(1, polynomial("-5.5xy-z"));
	unordered_table<polynomial> t({ c0, c1, c2 });
	EXPECT_EQ(t.get_size(), 3);

	t.insert(2, polynomial("7.81z+3yx-0.2"));
	EXPECT_EQ(t.get_size(), 4);

	t.erase(3);
	EXPECT_EQ(t.get_size(), 3);

	t.erase(0);
	EXPECT_EQ(t.get_size(), 2);

	t.insert(0, polynomial("-5yx-3.2"));
	EXPECT_EQ(t.get_size(), 3);
}

TEST(unordered_table, can_check_empty) {
	unordered_table<polynomial> t;
	EXPECT_EQ(t.empty(), true);

	t.insert(1, polynomial("8zx^4y-7x"));
	EXPECT_EQ(t.empty(), false);
}

TEST(unordered_table, can_access_table_cells_by_key) {
	unordered_table<polynomial> t;
	ASSERT_ANY_THROW(t[4]);

	t.insert(1, polynomial("8zx^4y-7x"));
	t.insert(2, polynomial("4y+4.5x"));
	ASSERT_NO_THROW(t[1]);
	EXPECT_EQ(t[1], polynomial("8zx^4y-7x"));
}

TEST(ordered_table, can_create_ordered_table) {
	ASSERT_NO_THROW(ordered_table<polynomial> t);

	t_cell<polynomial> c0(3, polynomial("yz+xy+zx^7")), c1(0, polynomial("3x^4+2.2x^2y^3")),
		c2(1, polynomial("-2.43xy+7z"));
	ASSERT_NO_THROW(ordered_table<polynomial> t({ c0, c1, c2 }));

	ordered_table<polynomial> t({ c0, c1, c2 });
	unordered_table<polynomial> t1({ c2, c1, c0 });
	ASSERT_NO_THROW(ordered_table<polynomial> t2(t));
	ASSERT_NO_THROW(ordered_table<polynomial> t2(t1));
}

TEST(ordered_table, can_insert) {
	t_cell<polynomial> c0(3, polynomial("-30.2+xyz")), c1(0, polynomial("7x^9-3yz^2")),
		c2(1, polynomial("-5.5xy-z"));
	ordered_table<polynomial> t0({ c1, c2, c0 }), t1;

	ASSERT_NO_THROW(t1.insert(c0.key, c0.value));
	t1.insert(c1.key, c1.value);
	t1.insert(1, polynomial("-5.5xy-z"));

	EXPECT_EQ(t1.size, t0.size);
	for (int i = 0; i < t1.size; ++i)
		EXPECT_EQ(t1.table[i] == t0.table[i], true);

	ASSERT_ANY_THROW(t0.insert(c2.key, polynomial("3x^2+x+1")));
}

TEST(ordered_table, can_find) {
	t_cell<polynomial> c0(10, polynomial("-0.2+x^3yz-5y")), c1(38, polynomial("x^4z-2yz^2+4")),
		c2(0, polynomial("-0.5y-z+x"));
	ordered_table<polynomial> t0({ c0, c1, c2 });

	ASSERT_NO_THROW(t0.find(c1.key));
	EXPECT_EQ(*(t0.find(c1.key)) == c1, true);
	EXPECT_EQ(*(t0.find(c2.key)) == c2, true);

	EXPECT_EQ(t0.find(9), nullptr);
}

TEST(ordered_table, can_erase) {
	t_cell<polynomial> c0(17, polynomial("15y+y^3-x^2")), c1(18, polynomial("7xy^2-3y^3z^2")),
		c2(1, polynomial("-57.5xy+2zx^3")), c3(19, polynomial("x^7y-1"));
	ordered_table<polynomial> t0({ c3, c0, c1, c2 }), t1({ c1, c2, c0 }), t2({ c1, c0 }), t3;

	ASSERT_NO_THROW(t0.erase(c3.key));
	EXPECT_EQ(t1.size, t0.size);
	for (int i = 0; i < t1.size; ++i)
		EXPECT_EQ(t1.table[i] == t0.table[i], true);

	t0.erase(c2.key);
	EXPECT_EQ(t2.size, t0.size);
	for (int i = 0; i < t2.size; ++i)
		EXPECT_EQ(t2.table[i] == t0.table[i], true);
	
	t0.erase(c1.key);
	t0.erase(c0.key);
	EXPECT_EQ(t3.size, t0.size);

	ASSERT_ANY_THROW(t0.erase(c2.key));
}

TEST(hash_table, can_create_hash_table) {
	ASSERT_NO_THROW(hash_table<polynomial> t);

	t_cell<polynomial> c0(3, polynomial("xy+x^7")), c1(0, polynomial("3x^4+2.2x^2y^3")),
		c2(1, polynomial("-2.43xy+7z"));
	ASSERT_NO_THROW(hash_table<polynomial> t({ c0, c1, c2 }));

	hash_table<polynomial> t0({ c0, c1, c2 });

	ASSERT_NO_THROW(hash_table<polynomial> t1(t0));
	ASSERT_NO_THROW(hash_table<polynomial> t2(hash_table<polynomial>{c1, c2}));
}

TEST(hash_table, can_find) {
	t_cell<polynomial> c0(10, polynomial("-0.2z-5y")), c1(38, polynomial("z^2+4")),
		c2(0, polynomial("-x"));
	hash_table<polynomial> t0({ c0, c1, c2 });

	ASSERT_NO_THROW(t0.find(c1.key));
	EXPECT_EQ(*(t0.find(c1.key)) == c1, true);
	EXPECT_EQ(*(t0.find(c2.key)) == c2, true);

	EXPECT_EQ(t0.find(9), nullptr);
}

TEST(hash_table, can_insert) {
	t_cell<polynomial> c0(3, polynomial("-3xyz")), c1(0, polynomial("15x^2-3yz^2")),
		c2(1, polynomial("-5.5xy-z"));

	hash_table<polynomial> t0({ c1, c2, c0 }), t1;

	ASSERT_NO_THROW(t1.insert(c0.key, c0.value));
	t1.insert(c1.key, c1.value);
	t1.insert(1, polynomial("-5.5xy-z"));

	EXPECT_EQ(t0.size == t1.size, true);
	EXPECT_EQ(t1.find(c0.key) != nullptr, true);
	EXPECT_EQ(t1.find(c1.key) != nullptr, true);
	EXPECT_EQ(t1.find(c2.key) != nullptr, true);
	EXPECT_EQ(t1.find(10) == nullptr, true);

	ASSERT_ANY_THROW(t0.insert(c2.key, polynomial("3x^2+x+1")));
}


TEST(hash_table, can_erase) {
	t_cell<polynomial> c0(17, polynomial("15y+y^3-x^2")), c1(18, polynomial("7xy^2-3y^3z^2")),
		c2(1, polynomial("-57.5xy+2zx^3")), c3(19, polynomial("x^7y-1"));
	hash_table<polynomial> t0({ c3, c0, c1, c2 }), t1({ c1, c2, c0 }), t2({ c1, c0 }), t3;

	ASSERT_NO_THROW(t0.erase(c3.key));
	EXPECT_EQ(t0.size == t1.size, true);
	EXPECT_EQ(t0.find(c3.key) == nullptr, true);

	t0.erase(c2.key);
	EXPECT_EQ(t0.size == t2.size, true);
	EXPECT_EQ(t0.find(c2.key) == nullptr, true);

	t0.erase(c1.key);
	t0.erase(c0.key);
	EXPECT_EQ(t0.size == t3.size, true);
	EXPECT_EQ(t0.find(c1.key) == nullptr, true);
	EXPECT_EQ(t0.find(c0.key) == nullptr, true);

	ASSERT_ANY_THROW(t0.erase(c2.key));
}