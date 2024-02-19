#ifndef TABLE_HPP
#define TABLE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using p = std::pair<std::string, std::size_t>;
using row_type = std::vector<std::string>;

class table {
public:
	row_type const& operator[](int index) const;
	std::size_t n_col() const;
	std::size_t n_row() const;
	std::size_t get_column_index(const std::string& column) const;
	void add_column(std::string& s, const std::size_t i);
	void add_row(const row_type& row);
	void sort_header();
	row_type get_header() const;
	void print(std::ostream& os = std::cout, const char delimiter = ',');
	void print_tsv(std::ostream& os = std::cout);
	void print_csv(std::ostream& os = std::cout);
	void read_tsv(std::istream& is = std::cin);
	void read_csv(std::istream& is = std::cin);
	void read(std::istream& is = std::cin, const char delimiter = ',');
private:
	std::vector<p> col_names;
	std::vector<row_type> data_;
};

#endif