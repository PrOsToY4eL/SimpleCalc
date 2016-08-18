#pragma once
#include <memory>
#include "Token.h"
struct Node
{
	std::shared_ptr<Node> left;
	std::shared_ptr<Node> right;
	std::shared_ptr<Node> parent;
	std::shared_ptr<Token> value;
	explicit Node() = default;
	explicit Node(const std::shared_ptr<Token> &val, const std::shared_ptr<Node> &l, const std::shared_ptr<Node> &r, const std::shared_ptr<Node> &p)
		: value{ std::move(val) }
		, left{ std::move(l) }
		, right{ std::move(r) }
		, parent{ std::move(p) }
	{}
	~Node() = default;
};

class AST
{
private:
	std::shared_ptr<Node> root;
public:
	void addNode(std::shared_ptr<Node> &node);
	explicit AST() = default;
	~AST() = default;
};
