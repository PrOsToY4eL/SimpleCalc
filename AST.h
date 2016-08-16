#pragma once
#include <memory>
template <typename ValueType>
struct Node;

template <typename ValueType>
class AST
{
private:
	std::unique_ptr<Node<ValueType>> root;
public:
	template <typename ValueType>
	struct Node
	{
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;
		std::unique_ptr<Node> parent;
		ValueType  value;
		explicit Node()
			: left{ nullptr }
			, right{ nullptr }
			, parent{ nullptr }
		{}
		explicit Node(const ValueType &val)
			: Node{}
			, value{ val }
		{}
		~Node() = default;
	};
	void addNode(std::unique_ptr<Node<ValueType>> &node);
	AST() = default;
	~AST() = default;
};
