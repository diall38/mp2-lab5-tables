#pragma once

#include <iostream>
#include <string>
#include <vector>

template <class T>
class List {
public:
	class node {
	public:
		T data;
		node* next;
		node(T value = 0, node* next_ = NULL) : next(next_), data(value) {}
	};
	node* first;
	size_t size;
	
	class iterator {
	protected:
		node* ptr;
	public:
		iterator() : ptr(NULL) {}
		iterator(node* ptr_) : ptr(ptr_) {}
		iterator(const iterator& iter) : ptr(iter.ptr) {}

		iterator& operator++ ()
		{
			ptr = ptr->next;
			return *this;
		}

		iterator operator++ (int)
		{
			iterator iter(*this);
			++(*this);
			return iter;
		}

		node*& operator*() { return ptr; }
		node* operator->() { return ptr; }
		bool operator== (iterator iter) const { return ptr == iter.ptr; }
		bool operator!= (iterator iter) const { return !(*this == iter); }
	};

	List() : first(NULL), size(0) {}

	List(const List<T>& lst) : first(NULL), size(0) {
		node* pcurr = first;

		//the order of monomials in a polynomial is determined during creation, 
		//the insertion of a monom does not violate the order
		for (iterator it_curr = lst.begin(); it_curr != lst.end(); ++it_curr)
			pcurr = insert(pcurr, it_curr->data);			//return pointer to current monomial									
	}

	virtual ~List() {
		clear();
	}

	void clear() {
		while (first) {
			node* tmp = first;
			first = tmp->next;
			delete tmp;
			size--;
		}
	}

	void print() {
		iterator iter = begin();
		while (iter != end()) {
			std::cout << iter->data << " ";
			iter++;
		}
		std::cout << std::endl;
	}

	node* get_first() { return first; }

	iterator search(T& value) {
		iterator it = begin();
		while (it != end()) {
			if (it->data == value)
				break;
			it++;
		}
		return it;
	}

	void erase(node* pos) {
		if (pos == NULL) {
			if (!size) { throw "List is empty"; }
			node* tmp = first;
			first = tmp->next;
			delete tmp;
		}
		else {
			node* p = pos->next;
			pos->next = p->next;
			delete p;
		}
		size--;
	}

	iterator begin() const {
		iterator it(first);
		return it;
	}

	iterator end() const {
		iterator it(NULL);
		return it;
	}

	bool empty() { return size == 0; }
	size_t get_size() { return size; }

	node* insert(node* pos, const T& value) {
		node* tmp;
		if (pos == NULL) {
			tmp = new node(value, first);
			first = tmp;
		}
		else {
			tmp = new node(value, pos->next);
			pos->next = tmp;
		}
		size++;
		return tmp;
	}

	//friend std::ostream &operator<<(std::ostream& out, const T& t) {
	//	iterator iter = t.begin();
	//	while (iter != t.end()) {
	//		out << iter->data << " ";
	//		iter++;
	//	}
	//	out << std::endl;
	//	return out;
	//}
};


#define EXP 9

class polynomial {
public:
	class monomial {
	public:							//degree - three-digit number
		size_t degree;				//monomial degree in the range from 0 to 9 for each variable (x, y or z) 
		double multiplier;			//coefficient in front of the monomial  

		monomial() : degree(0), multiplier(0) {}
		monomial(size_t t_degree, double t_mult) : degree(t_degree), multiplier(t_mult) {}
		monomial(const monomial& t_monom) : degree(t_monom.degree), multiplier(t_monom.multiplier) {}
		
		monomial& operator*=(const monomial& m) {
			//the digit of a three-digit number corresponds to the degree of one of the variables x,y,z
			if (degree / 100 + m.degree / 100 > EXP || degree / 10 % 10 + m.degree / 10 % 10 > EXP \
				|| degree % 10 + m.degree % 10 > EXP) 
				throw "too big degree of monomial";
			else {
				multiplier *= m.multiplier;
				degree += m.degree;
				return (*this);
			}
		}
		monomial operator*(const monomial& m) {
			monomial tmp = (*this);
			tmp *= m;
			return tmp;
		}

		bool operator==(const monomial& m) const {
			//take into account the error of "double" type
			return (abs(multiplier - m.multiplier) <= 1e-10 && degree == m.degree);
		}

		bool operator!=(const monomial& m) const {
			return !((*this) == m);
		}

		void print() const {
			if (multiplier < 0) std::cout << " - ";
			//do not display 1 as a coefficient, only if degree == 0 (const)
			if (abs(abs(multiplier) - 1) > 1e-10 || \
				abs(abs(multiplier) - 1) < 1e-10 && degree == 0) std::cout << abs(multiplier);
			int deg_x = degree / 100, deg_y = degree / 10 % 10, deg_z = degree % 10;

			if (deg_x > 1) std::cout << "x^" << deg_x;
			else if (deg_x == 1) std::cout << "x";

			if (deg_y > 1) std::cout << "y^" << deg_y;
			else if (deg_y == 1) std::cout << "y";

			if (deg_z > 1) std::cout << "z^" << deg_z;
			else if (deg_z == 1) std::cout << "z";
		}

		friend std::ostream& operator<<(std::ostream& out, const monomial& t) {
			if (t.multiplier < 0) out << " - ";

			if (abs(abs(t.multiplier) - 1) > 1e-10 || \
				abs(abs(t.multiplier) - 1) < 1e-10 && t.degree == 0) out << abs(t.multiplier);
			int deg_x = t.degree / 100, deg_y = t.degree / 10 % 10, deg_z = t.degree % 10;

			if (deg_x > 1) out << "x^" << deg_x;
			else if (deg_x == 1) out << "x";

			if (deg_y > 1) out << "y^" << deg_y;
			else if (deg_y == 1) out << "y";

			if (deg_z > 1) out << "z^" << deg_z;
			else if (deg_z == 1) out << "z";
			return out;
		}
	};
public:
	// create polynomial as List of monomials
	List<monomial> polynom;
public:
	polynomial() {}
	polynomial(const polynomial& p) : polynom(p.polynom) {}
	polynomial(std::string s) {
		//polynomial is created by string, all monomials in the polynomial are sorted
		translator(s); 
		//print();
	}
	virtual ~polynomial() {
		clear();
	}

	void print() const {
		auto it = polynom.begin();
		(*it)->data.print();
		++it;
		while (it != polynom.end()) {
			if ((*it)->data.multiplier > 1e-10) 
				std::cout << " + ";
			(*it)->data.print();
			++it;
		}
		std::cout << '\n';
	}

	friend std::ostream& operator<<(std::ostream& out, const polynomial& p) {
		auto it = p.polynom.begin();
		out << (*it)->data;
		++it;
		while (it != p.polynom.end()) {
			if ((*it)->data.multiplier > 1e-10)
				out << " + ";
			out << (*it)->data;
			++it;
		}
		return out;
	}

	void insert(const monomial& m) {
		if (abs(m.multiplier) < 1e-10) return;

		auto it = polynom.begin();
		if (it == polynom.end()) {
			polynom.insert(NULL, m);   
			return;
		}

		List<polynomial::monomial>::iterator ans = NULL;
		auto prev_it = it;

		while (it != polynom.end()) {

			if ((*it)->data.degree == m.degree) {
				(*it)->data.multiplier += m.multiplier;

				if (abs((*it)->data.multiplier) < 1e-10) {  //if multiplier == 0, delete this monomial
					if (it == prev_it && it == polynom.begin()) //if monomial on the first position
						polynom.erase(NULL);
					else polynom.erase((*prev_it));
				}
				return;
			}
			else {
				if ((*it)->data.degree < m.degree) 
					ans = it;
				else {
					polynom.insert(*ans, m);
					return;
				}
			}
			prev_it = it;
			++it;
		}

		if (it == polynom.end())
			polynom.insert(*ans, m);
	}

	void translator(std::string input) {
		bool first = true;
		polynomial::monomial* m = new polynomial::monomial();

		while (!input.empty()) {
			if (first) {
				if (input[0] >= '0' && input[0] <= '9') {
					std::string::size_type sz;
					double num = std::stod(input, &sz);
					input.erase(0, sz);
					m->multiplier = num;
				}
				first = false;
				continue;
			}

			// beginning of a new monomial
			if (input[0] == '+' || input[0] == '-') {     
				//add a new monomial if we see the operator "+" or "-"
				insert(*m);	
				m->degree = 0;

				if (input.size() == 1) {
					throw "incorrect input, check your input polynomial";
					return;
				}

				if (input[1] == 'x' || input[1] == 'y' || input[1] == 'z') {  //if there is no number after the sign,
					if (input[0] == '-') m->multiplier = -1;				  //we assume that the multiplier is equal to one
					else m->multiplier = 1;
					input.erase(0, 1);
					continue;
				}

				else {
					if (input[1] >= '0' && input[1] <= '9') {
						std::string::size_type sz;
						double num = std::stod(input, &sz);
						input.erase(0, sz);
						m->multiplier = num;
					}
					else {
						throw "incorrect input, check your input polynomial";
						return;
					}
				}
			}
			else
				if (input[0] == 'x' || input[0] == 'y' || input[0] == 'z') {
					if (m->multiplier == 0) m->multiplier = 1;

					char tmp = input[0];
					int deg = 1;
					if (input[1] == '^') {   //get the degree
						input.erase(0, 2);

						std::string::size_type sz;
						deg *= std::stoi(input, &sz);
						input.erase(0, 1);
						if (deg < 0 || deg > EXP) {
							throw "degree is incorrect";
							return;
						}
					}
					else input.erase(0, 1);

					m->degree += tmp == 'x' ? deg * 100 : tmp == 'y' ? deg * 10 : deg;
				}
				else {
					throw "incorrect input, check your input polynomial";
					return;
				}
		}
		insert(*m);
	}

	polynomial& operator+=(const polynomial&);

	polynomial operator+(const polynomial&);

	polynomial& operator-=(const polynomial&);

	polynomial operator-(const polynomial&);

	polynomial& operator+=(const monomial&);

	polynomial& operator*=(const polynomial&);

	polynomial operator*(const polynomial&);

	polynomial& operator*=(const double&);

	polynomial operator*(const double&);

	polynomial& operator=(const polynomial&);

	bool operator==(const polynomial&) const;

	bool operator!=(const polynomial&) const;

	bool search(const monomial&);

	void clear();

	bool empty();

	size_t get_count();
};

polynomial&
polynomial::operator+=(const polynomial& p)
{
	polynomial ans;
	auto it = polynom.begin(), it_p = p.polynom.begin();

	while (it != polynom.end() && it_p != p.polynom.end()) {
		if (it->data.degree == it_p->data.degree) {
			double mult = it->data.multiplier + it_p->data.multiplier;
			if (abs(mult) > 1e-10) {
				polynomial::monomial tmp_m(it->data.degree, mult);
				ans.insert(tmp_m);
			}
			++it, ++it_p;
		}
		else {
			if (it->data.degree < it_p->data.degree) {
				ans.insert((*it)->data);
				++it;
			}
			else {
				ans.insert((*it_p)->data);
				++it_p;
			}
		}
	}

	if (it == polynom.end())
		while (it_p != p.polynom.end()) {
			ans.insert((*it_p)->data);
			++it_p;
		}
	if (it_p == p.polynom.end()) 
		while (it != polynom.end()) {
			ans.insert((*it)->data);
			++it;
		}
	*this = ans;
	return (*this);
}

polynomial
polynomial::operator+(const polynomial& p)
{
	polynomial tmp(*this);
	tmp += p;
	return tmp;
}

polynomial&
polynomial::operator*=(const double& num)
{
	auto it = polynom.begin();
	while (it != polynom.end()) {
		it->data.multiplier *= num;
		++it;
	}
	return (*this);
}

polynomial&
polynomial::operator-=(const polynomial& p)
{
	polynomial tmp_p = p;
	tmp_p *= (const double)(-1);
	(*this) += tmp_p;
	return (*this);
}

polynomial
polynomial::operator*(const double& num)
{
	polynomial tmp(*this);
	tmp *= num;
	return tmp;
}

polynomial
polynomial::operator-(const polynomial& p)
{
	polynomial tmp(*this);
	tmp -= p;
	return tmp;
}

polynomial&
polynomial::operator+=(const monomial& m)
{
	insert(m);
	return (*this);
}

bool
polynomial::operator==(const polynomial& p) const
{
	auto it = polynom.begin(), it_p = p.polynom.begin();

	while (it != polynom.end() && it_p != p.polynom.end()) {
		if (it->data != it_p->data)
			return false;
		++it; ++it_p;
	}

	if (it != polynom.end() || it_p != p.polynom.end()) return false;
	return true;
}

bool
polynomial::operator!=(const polynomial& p) const {
	return !((*this) == p);
}

polynomial&
polynomial::operator*=(const polynomial& p) {
	polynomial tmp;
	auto it1 = polynom.begin(), it2 = p.polynom.begin();
	while (it1 != polynom.end()) {
		it2 = p.polynom.begin();
		while (it2 != p.polynom.end()) {
			tmp += (*it1)->data * (*it2)->data;
			++it2;
		}
		++it1;
	}
	*this = tmp;
	return *this;
}

polynomial
polynomial::operator*(const polynomial& p) {
	polynomial tmp;
	tmp = *this;
	tmp *= p;
	return tmp;
}

bool polynomial::search(const monomial& m)
{
	auto it = polynom.begin();
	while (it != polynom.end()) {
		if ((*it)->data == m)
			return true;
		else ++it;
	}

	return false;
}

void polynomial::clear()
{
	size_t count = polynom.size;
	while (count) {
		polynom.erase(NULL);
		count--;
	}
}

polynomial& polynomial::operator=(const polynomial& p) {
	clear();
	auto it = p.polynom.begin();
	while (it != p.polynom.end()) {
		insert((*it)->data);
		++it;
	}
	return *this;
}

bool polynomial::empty() {
	return polynom.size == 0;
}

size_t polynomial::get_count() {
	return polynom.size;
}

