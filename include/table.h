#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
#include <algorithm>
#include <cmath>
#include "polynomial.h"
#include <fstream>


#define GET_COMPLEXITY
#ifdef GET_COMPLEXITY
	size_t ops_find = 0;
	size_t ops_insert = 0;
	size_t ops_erase = 0;

	void get_complexity_insert() {
		std::ofstream out;
		out.open("log.txt", std::ios::app);
		out << std::endl;
		out << "Number of commands, executed the last time insert function was called:\n";
		out << ops_insert << std::endl;
		out.close();
	}

	void get_complexity_find() {
		std::ofstream out;
		out.open("log.txt", std::ios::app);
		out << std::endl;
		out << "Number of commands, executed the last time the search function was called:\n";
		out << ops_find << std::endl;
		out.close();
	}

	void get_complexity_erase() {
		std::ofstream out;
		out.open("log.txt", std::ios::app);
		out << std::endl;
		out << "Number of commands, executed the last time erase function was called:\n";
		out << ops_erase << std::endl;
		out.close();
	}

#endif

//====================== abstract class _table_interface and class cell =========================
template <class T>
class _table_interface {
public:
	class cell {
	public:
		size_t key;
		T value;

		cell() {}

		cell(size_t _key, T _value) :key(_key), value(_value){}
		
		cell(const std::pair<size_t, T>& p) :key(p.first), value(p.second) {}

		cell(cell&& c) :key(std::move(c.key)), value(std::move(c.value)) {}

		cell(const cell& c) :key(c.key), value(c.value) {}

		virtual ~cell(){}

		cell& operator=(const cell& c) {
			key = c.key;
			value = c.value;
			return *this;
		}

		bool operator==(const cell& c) {
			return (key == c.key && value == c.value);
		}
		bool operator!=(const cell& c) {
			return !(*this == c);
		}
	};

	virtual void clear() = 0;

	virtual bool erase(size_t _key) = 0;
	
	virtual bool insert(size_t _key, const T& _val) = 0;

	virtual _table_interface<T>::cell* find(size_t key) = 0;

};

template <class T>
using t_cell = typename _table_interface<T>::cell;


//===================================== class unordered_table ===================================
template <class T>
class unordered_table: public _table_interface<T> {
public:
	std::vector<t_cell<T>> table;
	size_t size;


	unordered_table() : size(0) {}

	unordered_table(const std::initializer_list<_table_interface<T>::cell>& l) : table(l), size(l.size()) {}
	
	unordered_table(const unordered_table<T>&& t) : table(std::move(t.table)), size(std::move(t.size)) {}

	unordered_table(const unordered_table<T>& t) : table(t.table), size(t.size) {}

	virtual ~unordered_table() {
		clear();
	}

	bool insert(size_t key, const T& val) override {
		if (find(key) == nullptr) {
			table.push_back(t_cell<T>(key, val));
			size++;
#ifdef GET_COMPLEXITY
			ops_insert = 1;
			std::ofstream out;
			out.open("log.txt", std::ios::app);
			out << "\nUNORDERED TABLE";
			out.close();
			get_complexity_insert();
#endif
			return true;
		}
		else return false;
	}

	bool erase(size_t key) override {
		bool deleted = false;
#ifdef GET_COMPLEXITY
		ops_erase = 0;
#endif

		for (auto& x : table) {
#ifdef GET_COMPLEXITY
			++ops_erase;
#endif
			if (x.key == key) {
				// also work correct for size == 1
				x = table[size - 1];
				table.pop_back();
				size--;
				deleted = true;

#ifdef GET_COMPLEXITY
				ops_erase++;
				std::ofstream out;
				out.open("log.txt", std::ios::app);
				out << "\nUNORDERED TABLE";
				out.close();
				get_complexity_erase();
#endif
				return true;
			}
		}

		if (!deleted) return false;
	}

	t_cell<T>* find(size_t key) override {
#ifdef GET_COMPLEXITY
		ops_find = 0;
#endif

		for (auto& x : table) {
#ifdef GET_COMPLEXITY
			++ops_find;
#endif
			if (x.key == key) {
#ifdef GET_COMPLEXITY
				std::ofstream out;
				out.open("log.txt", std::ios::app);
				out << "\nUNORDERED TABLE";
				out.close();
				get_complexity_find();
#endif
				return &x;
			}
		}
#ifdef GET_COMPLEXITY
		std::ofstream out;
		out.open("log.txt", std::ios::app);
		out << "\nUNORDERED TABLE";
		out.close();
		get_complexity_find();
#endif
		return nullptr;
	}

	const size_t get_size() const noexcept { return size; }
	
	bool empty() const noexcept { return size == 0; }

	void clear() { table.clear(); }

	T& operator[](size_t _key) {
		auto ptr = find(_key);

		if (ptr != nullptr)
			return (*ptr).value;
		else throw "Incorrect access by key to a table cell\n";
	}
};


//===================================== class ordered_table =====================================
template <class T>
class ordered_table : public unordered_table<T> {
public:
	std::vector<t_cell<T>> table;
	size_t size;

	ordered_table() : size(0) {}

	ordered_table(const std::initializer_list<t_cell<T>>& l) :table(l), size(l.size()) {
		std::sort(table.begin(), table.end(), [](const t_cell<T>& first, const t_cell<T>& second) {
			return first.key < second.key;
		});
	}

	ordered_table(const unordered_table<T>& t) :table(t.table), size(table.size()) {
		std::sort(table.begin(), table.end(), [](const t_cell<T>& first, const t_cell<T>& second) {
			return first.key < second.key;
		});
	}

	ordered_table(unordered_table<T>&& t) :table(std::move(t.table)), size(std::move(table.size())) {
		std::sort(table.begin(), table.end(), [](const t_cell<T>& first, const t_cell<T>& second) {
			return first.key < second.key;
		});
	}

	ordered_table(const ordered_table<T>& t) :table(t.table), size(table.size()) {}
	ordered_table(ordered_table<T>&& t) :table(std::move(t.table)), size(std::move(table.size())) {}

	virtual ~ordered_table() {
		clear();
	}

	void clear() { table.clear(); }

	const size_t get_size() const noexcept { return size; }

	bool empty() const noexcept { return size == 0; }

	t_cell<T>* find(size_t key) override {
		//binary_search
		auto it = std::lower_bound(table.begin(), table.end(), t_cell<T>(key, T()),
			[](const t_cell<T>& first, const t_cell<T>& second) {
			return first.key < second.key;
		});

#ifdef GET_COMPLEXITY
		if (table.size() == 1)
			ops_find = 1;
		else if (table.size() > 1)
			ops_find = log2(table.size());

		std::ofstream out;
		out.open("log.txt", std::ios::app);
		out << "\nORDERED TABLE";
		out.close();
		get_complexity_find();
#endif

		if (it == table.end() || it->key != key)
			return nullptr;
		else return &(*it);
	}

	bool insert(size_t _key, const T& _value) override {
		//binary_search
		auto it = std::lower_bound(table.begin(), table.end(), t_cell<T>(_key, T()),
			[](const t_cell<T>& first, const t_cell<T>& second) {
			return first.key < second.key;
		});

		if (it != table.end() && it->key == _key)
			return false;

		size_t pos = it - table.begin();

		table.emplace_back(t_cell<T>({ _key, _value }));
		size++;

		for (int i = size - 1; i > pos; --i) 
			std::swap(table[i], table[i - 1]);

#ifdef GET_COMPLEXITY
		//find + emplace_back
		if (table.size() == 1)
			ops_insert = 2;
		else if (table.size() > 1)
			ops_insert = log2(table.size()) + 1;

		//swap
		ops_insert += (size - 1 > pos) ? size - 1 - pos : 0;
		std::ofstream out;
		out.open("log.txt", std::ios::app);
		out << "\nORDERED TABLE";
		out.close();
		get_complexity_insert();
#endif
		return true;
	}

	bool erase(size_t key) override {
		auto it = find(key);

		if (it != nullptr) {
			int pos = it - &table[0];

			for (int i = pos; i < size - 1; ++i)
				std::swap(table[i], table[i + 1]);

			table.pop_back();

#ifdef GET_COMPLEXITY
			//find + pop_back
			ops_erase = ops_find + 1;
			//swap
			ops_erase += (size - 1 > pos) ? size - 1 - pos : 0;

			std::ofstream out;
			out.open("log.txt", std::ios::app);
			out << "\nORDERED TABLE";
			out.close();
			get_complexity_erase();
#endif
		}
		else return false;

		size--;
		return true;
	}

	T& operator[](size_t _key) {
		auto ptr = find(_key);

		if (ptr != nullptr)
			return (*ptr).value;
		else throw "Incorrect access by key to a table cell\n";
	}
};


//======================================= class hash_table ======================================
#define NUM_BUCKETS 1000
#define PRIME 1009

template <class T>
class hash_table: public _table_interface<T> {
public:
	std::vector<List<t_cell<T>>> table;
	size_t size;

private:
	size_t a, b;

private:
	size_t hash_function(size_t _key) {
		return ((a * _key + b) % PRIME) % NUM_BUCKETS;
	}

public:

	hash_table() :size(0) {
		srand(time(NULL));
		a = rand() % PRIME;
		b = rand() % PRIME;

		table.resize(NUM_BUCKETS);
	}

	hash_table(const hash_table<T>& t) :table(t.table), size(t.size), a(t.a), b(t.b) {}

	hash_table(hash_table<T>&& t) :table(std::move(t.table)), size(std::move(t.size)), a(t.a), b(t.b) {}

	hash_table(const std::initializer_list<t_cell<T>>& l) {
		table.resize(NUM_BUCKETS);
		srand(time(NULL));
		a = rand() % PRIME;
		b = rand() % PRIME;

		size = 0;
		for (auto& val : l)
			insert(val.key, val.value);
	}

	virtual ~hash_table() {
		clear();
	}

	const size_t get_size() const noexcept { return size; }

	bool empty() const noexcept { return size == 0; }

	void clear() override { 
		for (size_t i = 0; i < NUM_BUCKETS; ++i) 
			table[i].clear();
		
		table.clear();
	}

	t_cell<T>* find(size_t _key) override {
		size_t hash = hash_function(_key);
#ifdef GET_COMPLEXITY
		ops_find = 1;
#endif
		for (auto it = table[hash].begin(); it != table[hash].end(); ++it) {
#ifdef GET_COMPLEXITY
			++ops_find;
#endif
			if (it->data.key == _key) {
#ifdef GET_COMPLEXITY
				std::ofstream out;
				out.open("log.txt", std::ios::app);
				out << "\nHASH TABLE";
				out.close();
				get_complexity_find();
#endif
				return &(it->data);
			}
		}
#ifdef GET_COMPLEXITY
		std::ofstream out;
		out.open("log.txt", std::ios::app);
		out << "\nHASH TABLE";
		out.close();
		get_complexity_find();
#endif
		return nullptr;
	}

	bool insert(size_t _key, const T& _val) override {
		auto it = find(_key);

		if (it != nullptr)
			return false;
		
		size_t hash = hash_function(_key);

		t_cell<T> new_node(_key, _val);
		table[hash].insert(0, new_node);
		size++;

#ifdef GET_COMPLEXITY
		ops_insert = ops_find + 1;
		std::ofstream out;
		out.open("log.txt", std::ios::app);
		out << "\nHASH TABLE";
		out.close();
		get_complexity_insert();
#endif
		return true;
	}

	bool erase(size_t _key) override {
		typename List<t_cell<T>>::iterator t = NULL;
		bool f = 0;

		size_t hash = hash_function(_key);
#ifdef GET_COMPLEXITY
		ops_erase = 1;
#endif

		for (auto it = table[hash].begin(); it != table[hash].end(); ++it) {
#ifdef GET_COMPLEXITY
			++ops_erase;
#endif
			if (it->data.key == _key) {
				f = 1;
				break;
			}
			else t = it->next;
		}

		if (f == 0) return false;

		table[hash].erase(*t);
		size--;

#ifdef GET_COMPLEXITY
		++ops_erase;
		std::ofstream out;
		out.open("log.txt", std::ios::app);
		out << "\nHASH TABLE";
		out.close();
		get_complexity_erase();
#endif

		return true;
	}

	T& operator[](size_t _key) {
		auto ptr = find(_key);

		if (ptr != nullptr)
			return (*ptr).value;
		else throw "Incorrect access by key to a table cell\n";
	}
};