#include "table.hpp"


row_type const& table::operator[](int index) const
{
	return data_[index];
}
std::size_t table::n_col() const {
	return col_names.size();
}
std::size_t table::n_row() const {
	return data_.size();
}
std::size_t table::get_column_index(const std::string& column) const {
	for (auto& col : col_names) {
		if (col.first == column) {
			return col.second;
		}
	}
	return -1;
}
void table::add_column(std::string& s, const std::size_t i) {
	col_names.emplace_back(move(s), i);
}
void table::add_row(const row_type& row) {
	data_.push_back(row);
}
void table::sort_header() {
	std::sort(col_names.begin(), col_names.end());
}
row_type table::get_header() const {
	row_type header;
	for (auto& pair : col_names) {
		header.push_back(pair.first);
	}
	return header;
}
void table::print_csv(std::ostream& os) {
	print(os, ',');
}
void table::print_tsv(std::ostream& os) {
	print(os, '\t');
}
void table::print(std::ostream& os, const char delimiter) {
	size_t idx = 0;
	for (const p& s : col_names) {
		if (s.first.find(delimiter) != std::string::npos)
			os << '\"' + s.first + '\"';
		else
			os << s.first;
		if (idx != n_col() - 1)
			os << delimiter;
		idx++;
	}
	os << std::endl;
	idx = 0;
	for (const row_type& r : data_) {
		for (const p& s : col_names) {
			if (r[s.second].find(delimiter) != std::string::npos)
				os << "\"" + r[s.second] + "\"";
			else
				os << r[s.second];
			if (idx != n_col() - 1)
				os << delimiter;
			idx++;
		}
		os << std::endl;
		idx = 0;
	}
}
void table::read_tsv(std::istream& is) {
	read(is, '\t');
}
void table::read_csv(std::istream& is) {
	read(is, ',');
}
void table::read(std::istream& is, const char delimiter) {
	std::string line;
	std::getline(is, line);
	if (is.fail()) {
		std::cout << "Can't read from file."<< std::endl;
		return;
	}
	bool quoted = false;
	size_t quotes = 0;
	size_t columns = 0;
	std::string value = "";
	bool header = true;
	do {
		row_type row;
		quoted = false;
		quotes = 0;
		value = "";
		for (size_t i = 0; i < line.size(); i++) {
			if (!header && (row.size() == n_col()))
			{
				value = "";
				break;
			}
			if (line[i] == '"') {
				if (!quoted) {
					quoted = true;
				}
				quotes++;
			}
			else if (line[i] == delimiter && (quotes % 2 == 0)) {
				if (quotes == 2) {
					value = value.substr(1, value.size() - 2);
				}
				header ? add_column(value, columns) : row.push_back(value);
				value = "";
				quoted = false;
				quotes = 0;
				columns++;
				continue;
			}
			value += line[i];
		}
		if (value != "") {
			if (quotes == 2) {
				value = value.substr(1, value.size() - 2);
			}
			header ? add_column(value, columns) : row.push_back(value);
			value = "";
		}
		if (!header) {
			while (row.size() < n_col()) {
				row.push_back("");
			}
			add_row(row);
		}
		header = false;
	} while (std::getline(is, line));
}