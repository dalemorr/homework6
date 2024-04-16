#include <fstream>
#include <iostream>

struct Node {
    int val;
    Node** children;
};

struct Tree {
    Node* head;
};

std::string get_file_name();

Tree parse_data(std::ifstream& data_file);

int main() {
    std::cout << "Enter a file name: ";
    std::string data_file_name = get_file_name();

    std::ifstream data_file (data_file_name);
    Tree data;
    if (data_file.is_open()) {
        data = parse_data(data_file);
    } else {
        std::cout << "There was an issue opening the file" << std::endl;
        return 1;
    }

    return 0;
}

std::string get_file_name() {
    std::string file_name;
    std::cin >> file_name;
    return file_name;
}

Tree parse_data(std::ifstream& data_file) {
    Tree data = Tree{nullptr};

    int num;
    while (data_file >> num) {
        if (num == -1) {
            std::cout << std::endl;
        } else {
            std::cout << num << " ";
        }
    }

    return data;
}
