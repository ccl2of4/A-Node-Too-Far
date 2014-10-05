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
	static void reset_nodes (vector<Node *> &nodes);
public:
	Node (int val) : val(val), ttl(0), visited(false) {}
	void join (Node *other_node) { this->adjacent_nodes.push_back (other_node); other_node->adjacent_nodes.push_back (this); }
	int reachable_nodes (int ttl);
	string description (void) { return to_string (this->val); }
};

void
Node::reset_nodes (vector<Node *> &nodes) {
	for (Node *node : nodes) {
		node->ttl = 0;
		node->visited = false;}
}

int
Node::reachable_nodes (int ttl) {
	int result = 0;
	vector<Node *> affected_nodes;
	
	queue<Node *> q;
	this->ttl = ttl;
	this->visited = true;
	q.push (this);
	affected_nodes.push_back (this);

	while (!q.empty ()) {
		Node *node = q.front (); q.pop ();
		++result;
		if (node->ttl > 0)
			for (Node *other_node : node->adjacent_nodes) {
				if (!other_node->visited) {
					other_node->visited = true;
					other_node->ttl = node->ttl - 1;
					q.push (other_node);
					affected_nodes.push_back (other_node);
				}
			}
	}

	reset_nodes (affected_nodes);
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
			Node *right_node; Node *left_node;

			r >> left_val; r >> right_val;

			left_node = (*result)[left_val];
			if (!left_node) {
				left_node = new Node (left_val);
				(*result)[left_val] = left_node;}

			right_node = (*result)[right_val];
			if (!right_node) {
				right_node = new Node (right_val);
				(*result)[right_val] = right_node;}

			left_node->join (right_node);
		}
	}
	return result;
}

static pair<int, int>
read_case (istream &r) {
	int start_node_num, ttl;
	r >> start_node_num; r >> ttl;

	if (start_node_num || ttl)
		return make_pair (start_node_num, ttl);

	return make_pair (0, 0);
}

static int
solve_case (int start_node_num, int ttl, unordered_map<int,Node *> &map) {
	Node *start_node = map[start_node_num];
	return map.size () - start_node->reachable_nodes (ttl);
}

static void
print_case_results (int case_num, int nodes_not_reachable, int start_node, int ttl) {
	cout << "Case " << to_string (case_num) << ": " << to_string (nodes_not_reachable) << " nodes not reachable from node " << to_string (start_node) << " with TTL = " << to_string (ttl) << "." << endl;
}

int
main (void) {
	int case_num = 0;

	while (true) {
		unordered_map<int,Node *> *map = create_map (cin);
		if (!map) break;

		while (true) {
			pair<int,int> case_vals = read_case (cin);
			if (case_vals == make_pair (0,0)) break;

			int result = solve_case (case_vals.first, case_vals.second, *map);
			print_case_results (++case_num, result, case_vals.first, case_vals.second);

		}

		for (pair<int, Node *> p: *map)
			delete p.second;
		delete map;
	}
}
