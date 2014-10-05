#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Node;
static void print_nodes (vector<Node *> &);

struct Node {
private:
	int val;
	int ttl;
	bool visited;
	vector<Node *> adjacent_nodes;
public:
	Node (int val) { this->val = val; }
	void add_adjacent_node (Node *node) { adjacent_nodes.push_back (node); }
	int reachable_nodes (int ttl);
	string description (void) { return to_string (this->val); }
	static void clear_nodes (vector<Node *> &nodes) {
		for (Node *node : nodes) {
			node->ttl = 0;
			node->visited = false;
		}
	}
};

int
Node::reachable_nodes (int ttl) {
	int result = 0;
	
	queue<Node *> q;
	this->ttl = ttl;
	this->visited = true;
	q.push (this);

	while (!q.empty ()) {
		Node *node = q.front (); q.pop ();

		++result;
	
		if (node->ttl > 0)
			for (Node *other_node : node->adjacent_nodes) {
				if (!other_node->visited) {
					other_node->visited = true;
					other_node->ttl = node->ttl - 1;
					q.push (other_node);
				}
			}
	}
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
			if (!left_node) {
				left_node = new Node (left_val);
				(*result)[left_val] = left_node;}

			right_node = (*result)[right_val];
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

int
solve (int start_node_num, int ttl, unordered_map<int,Node *> &map) {
	Node *start_node = map[start_node_num];
	vector<Node *> nodes;
	for (pair<int,Node *>p : map) {
		nodes.push_back (p.second);
	}
	Node::clear_nodes (nodes);
	return map.size() - start_node->reachable_nodes (ttl);
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
			int result = solve (next_case.first, next_case.second, *map);
			print_results (++case_num, result, next_case.first, next_case.second);
		}
		for (pair<int, Node *> p: *map)
			delete p.second;
		delete map;
	}
}
