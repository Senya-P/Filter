#ifndef node_hpp_
#define node_hpp_


#include <string>
#include <memory>
#include <iostream>
#include <vector>

using row_type = std::vector<std::string>;
enum comp_op { eq, ne, gt, lt, ge, le };
enum logic_op { and_, or_ };

class a_node {
public:
	virtual ~a_node() {}
	a_node(const std::string& v) {
		value_ = v;
	}
	const std::string& get_value() const {
		return value_;
	}
	virtual const std::string& get_cell(const row_type& row) const = 0;
	virtual bool evaluate(const row_type& row) const = 0;
private:
	std::string value_;
};
class op_node : public a_node {
public:
	~op_node() {}
	op_node(const std::string& v, size_t arity, int priority) : a_node(v), priority_(priority) {
		arity_ = arity; 
	}
	const std::string& get_cell(const row_type& row) const override {
		throw std::runtime_error("No value to return.");
	}
	size_t get_arity() const {
		return arity_;
	}
	int get_priority() const {
		return priority_;
	}
	std::unique_ptr<a_node> left, right;
private:
	size_t arity_;
	int priority_; // higher number -> higher priority
};
class val_node : public a_node {
public:
	~val_node() {}
	val_node(const std::string& v) : a_node(v) {}
	bool evaluate(const row_type& row) const override {
		throw std::runtime_error("Nothing to evaluate.");
	}
};

class col_value : public val_node {
public:
	col_value(const std::string& v, size_t idx) : val_node(v) {
		idx_ = idx;
	}
	const std::string& get_cell(const row_type& row) const override {
		return row[idx_];
	}
private:
	size_t idx_;
};

class const_value : public val_node {
public:
	const_value(const std::string& v) : val_node(v) {}
	const std::string& get_cell(const row_type& row) const override {
		return get_value();
	}
};

static inline bool compare(const std::string& first, const std::string& second, comp_op op) {
	switch (op) {
		case eq: return first == second;
		case ne: return first != second;
		case lt: return first < second;
		case gt: return first > second;
		case le: return first <= second;
		case ge: return first >= second;
	}
	throw std::logic_error("Unknown operator " + op);
}

class comparison : public op_node {
public:
	comparison(const std::string& v, comp_op op, int priority) : op_node(v, 2, priority) {
		op_ = op;
	}
	bool evaluate(const row_type& row) const override {
		return compare(left->get_cell(row), right->get_cell(row), op_);
	}
private:
	comp_op op_;
};

class is_empty : public op_node {
public:
	is_empty(const std::string& v, int priority) : op_node(v, 1, priority) {
		left = NULL; // unused
	}
	bool evaluate(const row_type& row) const override {
		return (right->get_cell(row).empty());
	}
};


class boolean : public op_node {
public:
	boolean(const std::string& v, logic_op op, int priority) : op_node(v, 2, priority) {
		op_ = op;
	}
	bool evaluate(const row_type& row) const override {
		bool l = left->evaluate(row);
		bool r = right->evaluate(row);
		if (op_ == and_) {
			return l && r;
		}
		else { // --or
			return l || r;
		}
	}
private:
	logic_op op_;
};

#endif