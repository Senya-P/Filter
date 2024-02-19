#include "expr.hpp"

void expression::insert(std::string value) {
	if (value == "(") {
		priority_summand += 2;
	}
	else if (value == ")") {
		priority_summand -= 2;
		if (priority_summand < 0) {
			throw std::logic_error("Wrong number of closing brackets.");
		}
	}
	else if (value == "--or") {
		auto ptr = std::make_unique<boolean>(value, or_, 1 + priority_summand);
		nodes.push_back(std::move(ptr));
	}
	else if (value == "--and") {
		auto ptr = std::make_unique<boolean>(value, and_, 2 + priority_summand);
		nodes.push_back(std::move(ptr));
	}
	else if (value == "--eq") {
		auto ptr = std::make_unique<comparison>(value, eq, 3 + priority_summand);
		nodes.push_back(std::move(ptr));
	}
	else if (value == "--ne") {
		auto ptr = std::make_unique<comparison>(value, ne, 3 + priority_summand);
		nodes.push_back(std::move(ptr));
	}
	else if (value == "--gt") {
		auto ptr = std::make_unique<comparison>(value, gt, 3 + priority_summand);
		nodes.push_back(std::move(ptr));
	}
	else if (value == "--lt") {
		auto ptr = std::make_unique<comparison>(value, lt, 3 + priority_summand);
		nodes.push_back(std::move(ptr));
	}
	else if (value == "--ge") {
		auto ptr = std::make_unique<comparison>(value, ge, 3 + priority_summand);
		nodes.push_back(std::move(ptr));
	}
	else if (value == "--le") {
		auto ptr = std::make_unique<comparison>(value, le, 3 + priority_summand);
		nodes.push_back(std::move(ptr));
	}
	else if (value == "--empty") {
		auto ptr = std::make_unique<is_empty>(value, 3 + priority_summand);
		nodes.push_back(std::move(ptr));
	}
	else if (value[0] == '[')
	{
		const std::string& v = value.substr(1, value.size() - 2);
		size_t idx = get_idx(v);
		auto ptr = std::make_unique<col_value>(v, idx);
		values.push(std::move(ptr));
	}
	else {
		auto ptr = std::make_unique<const_value>(value);
		values.push(std::move(ptr));
	}
	if (nodes.size() > 0) {
		int priority = nodes.back()->get_priority();
		if (priority > max_priority) {
			max_priority = priority;
		}
		size_t arity = nodes.back()->get_arity();
		if (values.size() == arity) {
			nodes.back()->right = std::move(values.top());
			values.pop();
			if (arity == 2) {
				nodes.back()->left = std::move(values.top());
				values.pop();
			}
		}
	}
}
void expression::build_tree() {
	for (int i = max_priority; i > 0; i--) {
		create_parent_nodes(i);
	}
	if (nodes.size() != 1) {
		throw std::logic_error("Invalid format.");
	}
	root = std::move(nodes.back());
	nodes.pop_back();
}

void expression::create_parent_nodes(int priority) {
	for (size_t i = 0; i < nodes.size(); ) {
		if (nodes[i]->get_priority() == priority && nodes[i]->left == NULL && nodes[i]->right == NULL) { // priorities for comparison operators are not yet used, but are added to simplify further extensions
			nodes[i]->left = std::move(nodes[i - 1]);
			nodes[i]->right = std::move(nodes[i + 1]);
			nodes.erase(nodes.begin() + i + 1);
			nodes.erase(nodes.begin() + i - 1);
		}
		else {
			i++;
		}
	}
}
void expression::write_header(const std::string& line, std::ostream& os) {
	os << line << std::endl;
	std::string value = "";
	size_t idx = 0;
	for (size_t i = 0; i < line.size(); i++) {
		if (line[i] == delimiter) {
			header.push_back(value);
			map.emplace(value, idx);
			idx++;
			value = "";
		}
		else {
			value += line[i];
		}
	}
	if (value != "") {
		header.push_back(value);
		map.emplace(value, idx);
	}
}
void expression::evaluate(std::istream& is, std::ostream& os) {
	std::string line;
	while (std::getline(is, line)) {
		read_row(std::move(line));
		if (evaluate_row(row)) {
			write_row(os, row);
		}
		row.clear();
	}
}
bool expression::evaluate_row(const row_type& row) const {
	if (root->evaluate(row))
		return true;
	return false;
}
void expression::read_row(const std::string& line) {
	std::string value = "";
	for (size_t i = 0; i < line.size(); i++) {
		if (line[i] == delimiter) {
			row.push_back(value);
			value = "";
		}
		else {
			value += line[i];
		}
	}
	if (value != "") {
		row.push_back(value);
	}
	while (row.size() < header.size()) {
		row.push_back("");
	}
}
void expression::write_row(std::ostream& os, row_type& row) const {
	size_t idx = 0;
	for (const std::string& s : row) {
		os << s;
		if (idx != row.size() - 1) { // writes delimiter after empty cells too
			os << delimiter;
		}
		idx++;
	}
	os << std::endl;
}

void expression::set_delimiter(char del) {
	delimiter = del;
}
size_t expression::get_idx(const std::string& v) const {
	return map.find(v)->second;
}
const std::string& expression::get_value(size_t idx) const {
	return header[idx];
}
