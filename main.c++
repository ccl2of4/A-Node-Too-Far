#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Node {
private:
	int val;
	vector<Node> adjacent_nodes;
	void reachable_nodes (int ttl, vector<Node *> &working_vector);
public:
	Node (int val) { this->val = val; }
	void add_adjacent_node (Node &node) { adjacent_nodes.push_back (node); }
	vector<Node *> *create_reachable_nodes (int ttl);
};

void
Node::reachable_nodes (int ttl, vector<Node *> &working_vector) {

}

vector<Node *> *
Node::create_reachable_nodes (int ttl) {
	vector<Node *> *result = new vector<Node *> ();
	return result;
}

static unordered_map<int,Node *> *
create_map (istream &r) {
	unordered_map<int,Node *> *result = NULL;
	int num_connections;
	r >> num_connections;
	if (num_connections) {
		result = new unordered_map<int,Node *> ();
		for (int i = 0; i < num_connections; ++i) {

			int left_val, right_val;
			r >> left_val; r >> right_val;

			Node *right_node; Node *left_node;
			left_node = (*result)[left_val];
			right_node = (*result)[right_val];

			if (!left_node) {
				left_node = new Node (left_val);
				(*result)[left_val] = left_node;}
			if (!right_node) {
				right_node = new Node (right_val);
				(*result)[right_val] = right_node;}

			left_node->add_adjacent_node (*right_node);
			right_node->add_adjacent_node (*left_node);

		}
	}
	return result;
}

static vector<Node *> *
solve (istream &r, unordered_map<int,Node *> &map) {
	vector<Node *> *result = NULL;
	int start_node_num, ttl;

	r >> start_node_num; r >> ttl;

	if (start_node_num || ttl) {
		Node *start_node = map[start_node_num];
		result = start_node->create_reachable_nodes (ttl);
	}

	return result;
}

int
main (void) {
	while (true) {
		unordered_map<int,Node *> *map = create_map (cin);
		if (!map) break;

		while (true) {
			vector<Node *> result_nodes = solve (cin, *map);
			if (!result_nodes) break;
		}

		delete map;
	}
}
