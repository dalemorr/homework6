#include <exception>
#include <vector>
#include <string>
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
}

struct Node {
    int value;
    int g_cost;
    int h_cost;
    int f_cost;

    Node* parent;
    std::vector<Node*>* adjacentNodes = new std::vector<Node*> {};

    static inline auto* nodes = new std::vector<Node*> {};

    Node (int value) {
        this->value = value;

        addNodeToNodes(this);
    }

    ~Node()=default;

    int getValue() {
        return value;
    }

    int getGCost() const {
        return g_cost;
    }

    void setGCost(int gCost) {
        g_cost = gCost;
    }

    int getHCost() const {
        return h_cost;
    }

    void setHCost(int hCost) {
        h_cost = hCost;
    }

    int getFCost() const {
        return f_cost;
    }

    void setFCost(int fCost) {
        f_cost = fCost;
    }

    Node *getParent() const {
        return parent;
    }

    void setParent(Node* parent) {
        Node::parent = parent;
    }

    void addNodeToNodes(Node* node) {
        nodes->emplace_back(node);
    }

    void addAdjacentNode(Node* node) {
        for (auto & adjacentNode : *adjacentNodes) {
            if (adjacentNode==node || adjacentNode->getValue() == node->getValue()) {
                return;
            }
        }

        adjacentNodes->emplace_back(node);
    }

    void addAdjacentByValue(int value){
        if (getNode(value) == nullptr) {
            addAdjacentNode(new Node(value));
        } else {
            addAdjacentNode(getNode(value));
        }
    }

    std::vector<Node*>* getAdjacentNodes() {
        return adjacentNodes;
    }

    static std::string printNodes(std::vector<Node*>* nodes) {
        std::string x = "";
        for (Node* node : *nodes) {
            x += node->getValue();
            x += " ";
        }

        return x;
    }

    static Node* getNode(int value) {
        for (int i = 0; i < nodes->size(); i++) {
            if (nodes->at(i)->getValue() == value) {
                return nodes->at(i);
            }
        }

        return nullptr;
    }

    static std::vector<Node*>* getNodes() {
        return nodes;
    }

    static void clearNodes() {
        nodes = new std::vector<Node*> {};
    }

    static std::string NodesToString() {
        std::string val;
        val += "{";
        for (int i = 0; i < nodes->size(); i++) {
            val += std::to_string(nodes->at(i)->getValue());
            if (i != nodes->size()-1) {
                val += " ";
            }
        }
        val += "}";
        return val;
    }
};
