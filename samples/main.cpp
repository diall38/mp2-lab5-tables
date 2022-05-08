#pragma once

#include <iostream>
#include <string>
#include "polynomial.h"
#include "table.h"

int main() {
	std::cout << "\n	Hi!!! This program will allow you to perform operations on polynomials.\n" << \
		" When entering a polynomial, please follow some rules : \n";
	std::cout << " 1) Enter a polynomial without spaces\n" << \
		" 2) Only 3 variables are allowed in the polynomial: x, y and z\n" << \
		" 3) Monomials are written in any order\n" << \
		" 4) Degree of each variable in a monomial from 0 to 9\n" << \
		" 5) After writing the polynom, press enter\n\n";
	std::cout << "Select the action you want to perform on the polynomials\n" << \
		" 1. Add two polynomials\n" << \
		" 2. Subtract two polynomials\n" << \
		" 3. Multiply two polynomials\n" << \
		" 4. Multiply a polynomial by a number\n" << \
		" 5. Find the number of monomials in a polynomial\n" << \
		" 6. Insert (new) polynomial with key to ordered, unordered and hash table\n" << \
		" 7. Find (new) polynomial by the key in ordered, unordered and hash table\n" << \
		" 8. Erase (new) polynomial by the key in ordered, unordered and hash table\n" << \
		" 9. Insert resulting polynomial with key to ordered, unordered and hash table\n" << \
		" 10. Find resulting polynomial by the key in ordered, unordered and hash table\n" << \
		" 11. Erase resulting polynomial by the key in ordered, unordered and hash table\n" << \
		" 12. Exit\n";

	int item;
	std::cin >> item;

	polynomial last;

	unordered_table<polynomial> t1;
	ordered_table<polynomial> t2;
	hash_table<polynomial> t3;

	std::ofstream out;
	out.open("log.txt");

	while (item != 12) {
		if (item == 1 || item == 2 || item == 3) {
			std::cout << " Enter two polynomials:\n";

			std::string s1, s2;
			std::cin >> s1 >> s2;
			try {
				polynomial p1(s1), p2(s2);

				if (item == 1) {
					p1 += p2;
				}
				else {
					if (item == 2) p1 -= p2;
					else p1 *= p2;
				}
				p1.print();
				last = p1;
			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
				std::cout << " Please, try again:\n";
			}
		}
		if (item == 4 || item == 5) {
			try {
				std::cout << "Enter polynomial\n";
				std::string s;
				std::cin >> s;
				polynomial p(s);

				if (item == 4) {
					std::cout << "Ented number(double)\n";
					double m;
					std::cin >> m;

					p *= m;
					p.print();
					last = p;
				}
				else
					std::cout << p.get_count() << '\n';

			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
				std::cout << " Please, try again:\n";
			}
		}
		if (item >= 6 && item <= 11) {
			try {
				std::string s;
				polynomial p;
				if (item <= 8) {
					std::cout << "Enter polynomial\n";
					std::cin >> s;
					p = s;
				}
				std::cout << "Enter the name of the polynomial as a key\n";
				size_t key;
				std::cin >> key;

				if (item == 6) {
					std::ofstream out;
					out.open("log.txt", std::ios::app);
					out << "\n**************** Insertion of ";
					out << p;
					out << " ****************\n";
					out.close();

					t1.insert(key, p);
					t2.insert(key, p);
					t3.insert(key, p);

					std::cout << "Your polynomial has been added to the tables\n";
				}
				if (item == 9) {
					std::ofstream out;
					out.open("log.txt", std::ios::app);
					out << "\n**************** Insertion of ";
					out << last;
					out << " ****************\n";
					out.close();

					t1.insert(key, last);
					t2.insert(key, last);
					t3.insert(key, last);

					std::cout << "Your polynomial has been added to the tables\n";
				}

				if (item == 7 || item == 10) {
					std::ofstream out;
					out.open("log.txt", std::ios::app);
					out << "\n**************** Find of polynomial with key: " << key;
					out << " ****************\n";
					out.close();

					auto tmp1 = t1.find(key);
					auto tmp2 = t2.find(key);
					auto tmp3 = t3.find(key);

					if (tmp1 == nullptr) 
						std::cout << "The polynomial with the entered key is not in the table\n";
					else {
						std::cout << "Found polynomial\n";
						tmp1->value.print();
					}
				}

				if (item == 8 || item == 11) {
					std::ofstream out;
					out.open("log.txt", std::ios::app);
					out << "\n**************** Removal of polynomial with key: " << key;
					out << " ****************\n";
					out.close();

					t1.erase(key);
					t2.erase(key);
					t3.erase(key);

					std::cout << "Your polynomial has been removed from the tables\n";
				}

			}
			catch (const std::exception& e) {
				std::cout << e.what() << std::endl;
				std::cout << " Please, try again:\n";
			}
		}
		std::cout << "Enter the item\n";
		std::cin >> item;
	}

	out.close();
	return 0;
}
