#ifndef expr_hpp_
#define expr_hpp_


#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <map>
#include <string>

#include "node.hpp"

class expression {
public:
	void insert(std::string value);
	void build_tree();
	void write_header(const std::string& line, std::ostream& os);
	void evaluate(std::istream& is, std::ostream& os);
	size_t get_idx(const std::string& v) const;
	const std::string& get_value(size_t idx) const;
	void set_delimiter(char del);
private:
	std::unique_ptr<op_node> root;
	std::vector<std::unique_ptr<op_node>> nodes;
	std::stack<std::unique_ptr<val_node>> values;
	row_type row;
	row_type header;
	std::map<const std::string, size_t> map;
	char delimiter;
	int priority_summand = 0;
	int max_priority = 3;
	void create_parent_nodes(int priority);
	bool evaluate_row(const row_type& row) const;
	void read_row(const std::string& line);
	void write_row(std::ostream& os, row_type& row) const;
};


/*
- delete stack and use only priorities to build the tree??
- set default priorities inside class and not in constructor, add setter for new priority
*/
#endif