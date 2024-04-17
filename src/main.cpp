#include <exception>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

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

struct Maze {
    static std::vector<Node*>* A_Star(Node* start, Node* goal) {
        auto* openList = new std::priority_queue<Node*>;
        auto* closedList = new std::vector<Node*>;

        for (Node* node : *Node::getNodes()){
            node->setGCost(2147483647);
            node->setHCost(0);
            node->setFCost(node->getGCost() + node->getHCost());
        }

        openList->push(start);

        start->setGCost(0);
        start->setHCost(heuristic(start, goal));
        start->setFCost(start->getGCost() + start->getHCost());

        while (!openList->empty()) {
            Node* current_node = openList->top();
            openList->pop();

            if (current_node == goal){
                return reconstruct_path(start, goal);
            }

            closedList->push_back(current_node);
            Node::printNodes(current_node->getAdjacentNodes());

            for (Node* neighbor: *current_node->getAdjacentNodes()) {
                bool neighborIsNode = false;
                for (Node *node: *closedList) {
                    if (neighbor == node) {
                        neighborIsNode = true;
                        break;
                    }
                }

                if (neighborIsNode) {
                    continue;
                }

                int tentative_g_cost = current_node->getGCost() + 1;

                auto tempqueue = *openList;
                while (!tempqueue.empty()) {
                    if (tempqueue.top() != neighbor) {
                        tempqueue.pop();
                    }
                }

                if (tempqueue.empty()) {
                    openList->push(neighbor);
                }

                if (tentative_g_cost >= neighbor->getGCost()){
                    continue;
                }

                neighbor->setParent(current_node);
                neighbor->setGCost(tentative_g_cost);
                neighbor->setHCost(heuristic(neighbor, goal));
                neighbor->setFCost(neighbor->getGCost() + neighbor->getHCost());
            }
        }

        if (openList->empty()) {
            return nullptr;
        }
    }

    static std::vector<Node*>* reconstruct_path(Node* start, Node* goal) {
        auto* revPath = new std::vector<Node*> {};
        Node* currentNode = goal;
        while(currentNode!=start){
            revPath->emplace_back(currentNode);
            currentNode = currentNode->getParent();
        }
        revPath->emplace_back(start);

        auto* path = new std::vector<Node*> {};

        for(int i = revPath->size()-1; i > -1; i--){
            path->emplace_back(revPath->at(i));
        }

        return path;
    }

    static int heuristic(Node* start, Node* goal) {
        return 0;
    }

    static void ingestNodesFromFile(std::string filePath) {
        std::string line;
        std::ifstream file;
        file.open(filePath);
        if (file.is_open()) {
            while (getline(file, line)) {
                std::vector<std::string> splitLine = splitString(line, ' ');
                if (splitLine.size() == 0) {
                    continue;
                }

                if(Node::getNode(std::stoi(splitLine.at(0))) == nullptr){
                    new Node(std::stoi(splitLine.at(0)));
                }

                for(int i = 1; i < splitLine.size() && std::stoi(splitLine.at(i)) != -1; i++){
                    Node::getNode(std::stoi(splitLine.at(0)))->addAdjacentByValue(std::stoi(splitLine.at(i)));
                }
            }
        }
    }

    static std::vector<std::string> splitString(std::string str, char dl) {
        std::string word;
        int num = 0;

        str = str + dl;
        int l = str.size();

        std::vector<std::string> substr_list;
        for (int i = 0; i < l; i++) {
            if (str[i] != dl) {
                word = word + str[i];
            } else {
                if ((int)word.size() != 0) {
                    substr_list.push_back(word);
                }
                word = "";
            }
        }
        
        return substr_list;
    }
};

int main() {
    std::string File = "data/maze.txt";

    Maze::ingestNodesFromFile(File);
    std::vector<Node*>* path = Maze::A_Star(Node::getNode(1), Node::getNode(100));

    std::cout << "File: " << File << std::endl;
    std::cout << "Path:" << std::endl;

    for (int i = 0; i < path->size(); i++) {
        std::cout << path->at(i)->getValue();
        if(i != path->size()-1) {
            std::cout << "->";
        }
    }
    std::cout << std::endl;

    std::cout << "Path Cost: " << path->at(path->size()-1)->getFCost() << std::endl;

    Node::clearNodes();

    //test-maze

    Node::clearNodes();

    std::string File2 = "data/test-maze.txt";

    Maze::ingestNodesFromFile(File2);

    std::vector<Node*>* path2 = Maze::A_Star(Node::getNode(1), Node::getNode(6));

    std::cout << "\nFile:";

    std::cout << "\n" <<  File2 << ":\n";

    std::cout << "Path:\n";

    for(int i = 0; i < path2->size(); i++){
        std::cout << path2->at(i)->getValue();
        if(i != path2->size()-1)
            std::cout << "->";
    }
    std::cout << "\nPath Cost: " << path2->at(path2->size()-1)->getFCost();

    std::cout << '\n';

    // test-maze2

    Node::clearNodes();

    std::string File3 = "data/test-maze2.txt";

    Maze::ingestNodesFromFile(File3);

    std::vector<Node*>* path3 = Maze::A_Star(Node::getNode(1), Node::getNode(7));

    std::cout << "\nFile:";

    std::cout << "\n" << File3 << ":\n";

    std::cout << "Path:\n";

    for(int i = 0; i < path3->size(); i++){
        std::cout << path3->at(i)->getValue();
        if(i != path3->size()-1)
            std::cout << "->";
    }
    std::cout << "\nPath Cost: " << path3->at(path3->size()-1)->getFCost();

    std::cout << '\n';
}
