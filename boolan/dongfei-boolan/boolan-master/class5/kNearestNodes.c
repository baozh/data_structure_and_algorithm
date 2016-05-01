struct Node {
    double x;
    double y;
};

struct NodeWithDistance {
    Node node;
    double distance;
    NodeWithDistance(Node newNode) {
        node = newNode;
        distance = sqrt((pow(node.x, 2) + pow(node.y, 2)));
    }
};

struct Cmp {
    bool operator ()(const NodeWithDistance &a, const NodeWithDistance &b) {
        if (a.distance > b.distance)
            return false;
        else
            return true;
    }
};

vector<Node> kNearestNodes(vector<Node> nodes, int k) {
    vector<Node> results;

    priority_queue<NodeWithDistance, vector<NodeWithDistance>, Cmp> heap;

    for (int i = 0; i < nodes.size(); i++) 
    {
        NodeWithDistance newNodeWithDistance = NodeWithDistance(nodes[i]);
        if (heap.size() < k)
            heap.push(newNodeWithDistance);
        else 
        {
            if (heap.top().distance > newNodeWithDistance.distance) 
            {
                heap.pop();
                heap.push(newNodeWithDistance);
            }
        }
    }

    while (!heap.empty()) {
        results.push_back(heap.top().node);
        heap.pop();
    }

    return results;
}
