#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <map>
#include <string>

#include "expr.hpp"


int main(int argc, char** argv) {
	try {
		if (argc < 6) {
			std::cout << "Invalid number of arguments." << std::endl;
		}
		std::ifstream input(argv[argc - 2]);
		if (input.fail()) {
			std::cout << "File " << argv[argc - 2] << "does not exist." << std::endl;
			return 1;
		}
		std::ofstream output(argv[argc - 1]);

		std::string line;
		std::getline(input, line);
		if (input.fail()) {
			std::cout << "Can't read from file." << std::endl;
			return 1;
		}
		std::vector<std::string> condition;
		for (int i = 1; i < argc - 2; i++) {
			condition.push_back(argv[i]);
		}
		expression expr_tree;
		expr_tree.set_delimiter(',');
		expr_tree.write_header(line, output);
		for (auto const& c : condition) {
			expr_tree.insert(c);
		}
		expr_tree.build_tree();
		expr_tree.evaluate(input, output);

	}

	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unknown exception." << std::endl;
	}
	return 0;
}