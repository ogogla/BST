#include <iostream>
#include <vector>

class Node {
public:
    int value;
    Node *left;
    Node *right;

    Node(int val): value(val), left(nullptr), right(nullptr) {
    };

    virtual ~Node() {
    };
};

class BST {
private:
    Node *root;
    std::vector<Node *> v;

    void delete_vse(Node *cur) {
        if (!cur) return;
        delete_vse(cur->left);
        delete_vse(cur->right);
        delete cur;
    }

    Node *insertNode(Node *current, int val) {
        if (!current) {
            return new Node(val);
        }
        if (val < current->value) {
            current->left = insertNode(current->left, val);
        } else {
            current->right = insertNode(current->right, val);
        }
        return current;
    }

    void pre_order(Node *cur) {
        if (!cur) return;
        v.push_back(cur);
        pre_order(cur->left);
        pre_order(cur->right);
    }

    void in_order(Node *cur) {
        if (!cur) return;
        in_order(cur->left);
        v.push_back(cur);
        in_order(cur->right);
    }

    void post_order(Node *cur) {
        if (!cur) return;
        post_order(cur->left);
        post_order(cur->right);
        v.push_back(cur);
    }

    Node *max_order(Node *cur) {
        if (!cur) return nullptr;
        if (!cur->right) return cur;
        return max_order(cur->right);
    }

    Node *min_order(Node *cur) {
        if (!cur) return nullptr;
        if (!cur->left) return cur;
        return min_order(cur->left);
    }

    Node *search_order(Node *cur, int val) {
        if (!cur) return nullptr;
        if (val < cur->value) return search_order(cur->left, val);
        if (val > cur->value) return search_order(cur->right, val);
        return cur;
    }

    void pop_order(Node * &cur, Node *parent, int val) {
        if (!cur) return;
        if (val < cur->value) {
            pop_order(cur->left, cur, val);
        } else if (val > cur->value) {
            pop_order(cur->right, cur, val);
        } else {
            if (!cur->left && !cur->right) {
                delete cur;
                cur = nullptr;
            } else if (cur->left && cur->right) {
                Node *minRight = min_order(cur->right);
                cur->value = minRight->value;
                pop_order(cur->right, cur, minRight->value);
            } else {
                Node *child = cur->left ? cur->left : cur->right;
                Node *temp = cur;
                cur = child;
                delete temp;
            }
        }
    }

    void inject(Node *in, Node * &out) {
        if (!in) return;
        out = insertNode(out, in->value);
        inject(in->left, out);
        inject(in->right, out);
        delete in;
    }

public:
    void insert(int val) {
        root = insertNode(root, val);
    }

    void display() {
        if (root == nullptr) {
            std::cout << "List is empty!\n";
            return;
        }
        std::cout << "Choose order:\n1) pre_order\n2) in_order\n3) post_order\n";
        int x;
        std::cin >> x;
        if (x == 1) pre();
        if (x == 2) in();
        if (x == 3) post();
        for (auto u: v) {
            std::cout << u->value << " ";
        }
        std::cout << std::endl;
    }

    void pre() {
        v.clear();
        pre_order(root);
    }

    void in() {
        v.clear();
        in_order(root);
    }

    void post() {
        v.clear();
        post_order(root);
    }

    void max() {
        Node *maxx = max_order(root);
        if (maxx) std::cout << maxx->value;
        else std::cout << "BST is empty!";
    }

    void min() {
        Node *minn = min_order(root);
        if (minn) std::cout << minn->value;
        else std::cout << "BST is empty!";
    }

    bool search(int val) {
        return search_order(root, val) != nullptr;
    }

    void pop(int val) {
        if (!root) {
            std::cout << "BST is empty!";
            return;
        }
        pop_order(root, nullptr, val);
    }

    BST(): root(nullptr) {
    };

    virtual ~BST() {
        delete_vse(root);
    }
};

void showMenu() {
    std::cout << "0. Exit\n";
    std::cout << "1. Display\n";
    std::cout << "2. Insert\n";
    std::cout << "3. Search\n";
    std::cout << "4. Delete\n";
    std::cout << "5. Find Min\n";
    std::cout << "6. Find Max\n";
}

int main() {
    BST tree;
    int choice, value;
    do {
        showMenu();
        std::cout << "Enter choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                tree.display();
                break;
            case 2:
                std::cout << "Enter value to insert: ";
                std::cin >> value;
                tree.insert(value);
                break;
            case 3:
                std::cout << "Enter value to search: ";
                std::cin >> value;
                std::cout << (tree.search(value) ? "Found" : "Not Found") << std::endl;
                break;
            case 4:
                std::cout << "Enter value to delete: ";
                std::cin >> value;
                tree.pop(value);
                break;
            case 5:
                tree.min();
                std::cout << std::endl;
                break;
            case 6:
                tree.max();
                std::cout << std::endl;
                break;
        }
    } while (choice != 0);
    return 0;
}
