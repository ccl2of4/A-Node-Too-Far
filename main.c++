#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

struct Node;
static void print_nodes (vector<Node *> &);

struct Node {
private:
	int val;
	vector<Node *> adjacent_nodes;
	void reachable_nodes (int ttl, vector<Node *> &working_vector);
public:
	Node (int val) { this->val = val; }
	void add_adjacent_node (Node *node) { adjacent_nodes.push_back (node); }
	vector<Node *> *create_reachable_nodes (int ttl);
	string description (void) { return to_string (this->val); }
};

void
Node::reachable_nodes (int ttl, vector<Node *> &working_vector) {
	if (find (working_vector.begin (), working_vector.end (), this) == working_vector.end ())
		working_vector.push_back (this);
	
	if (ttl-- > 0)
		for (Node *node : this->adjacent_nodes)
			node->reachable_nodes (ttl, working_vector);
}

vector<Node *> *
Node::create_reachable_nodes (int ttl) {
	vector<Node *> *result = new vector<Node *> ();
	this->reachable_nodes (ttl, *result);
	return result;
}

static void
print_nodes (vector<Node *> &vector) {
	for (Node *node : vector) {
		cout << "\t" << node->description () << endl;
	}
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

			left_node->add_adjacent_node (right_node);
			right_node->add_adjacent_node (left_node);

		}
	}
	return result;
}

static pair<int, int>
read_case (istream &r) {
	int start_node_num, ttl;
	r >> start_node_num; r >> ttl;

	if (!start_node_num && !ttl) {
		return make_pair (0, 0);
	}
	return make_pair (start_node_num, ttl);
}

static vector<Node *> *
solve (int start_node_num, int ttl, unordered_map<int,Node *> &map) {
	Node *start_node = map[start_node_num];
	vector<Node *> *result = start_node->create_reachable_nodes (ttl);
	return result;
}

static void
print_results (int case_num, int nodes_not_reachable, int start_node, int ttl) {
	cout << "Case " << to_string (case_num) << ": " << to_string (nodes_not_reachable) << " nodes not reachable from node " << to_string (start_node) << " with TTL = " << to_string (ttl) << "." << endl;
}

int
main (void) {
	int case_num = 0;
	while (true) {
		unordered_map<int,Node *> *map = create_map (cin);
		if (!map) break;

		while (true) {
			pair<int, int> next_case = read_case (cin);
			if (next_case == make_pair(0,0)) break;
			vector<Node *> *result_nodes = solve (next_case.first, next_case.second, *map);
			int nodes_not_reachable = map->size () - result_nodes->size ();
			print_results (++case_num, nodes_not_reachable, next_case.first, next_case.second);
			delete result_nodes;
		}

		delete map;
	}
}
