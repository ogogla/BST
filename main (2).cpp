#include <iostream>
#include <vector>
#include <climits>

class Node {
public:
    int value;
    Node *left;
    Node *right;

    Node(int val): value(val), left(nullptr), right(nullptr) {};

    virtual ~Node() {};
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

    // 1) Поиск k-того наименьшего элемента
    Node* kthSmallest(Node* root, int& k) {
        if (!root) return nullptr;
        Node* left = kthSmallest(root->left, k);
        if (left) return left;
        k--;
        if (k == 0) return root;
        return kthSmallest(root->right, k);
    }

    // 2) Нахождение ближайшего меньшего и большего элемента
    void findPreSuc(Node* root, Node*& pre, Node*& suc, int val) {
        if (!root) return;
        if (root->value == val) {
            if (root->left) {
                pre = max_order(root->left);
            }
            if (root->right) {
                suc = min_order(root->right);
            }
            return;
        }
        if (root->value > val) {
            suc = root;
            findPreSuc(root->left, pre, suc, val);
        } else {
            pre = root;
            findPreSuc(root->right, pre, suc, val);
        }
    }

    // 3) Проверка, является ли BST
    bool isBSTUtil(Node* root, int min, int max) {
        if (!root) return true;
        if (root->value < min || root->value > max) return false;
        return isBSTUtil(root->left, min, root->value - 1) && isBSTUtil(root->right, root->value + 1, max);
    }

    // 4) Поиск ближайшего общего предка (LCA)
    Node* findLCA(Node* root, int n1, int n2) {
        if (!root) return nullptr;
        if (root->value > n1 && root->value > n2) return findLCA(root->left, n1, n2);
        if (root->value < n1 && root->value < n2) return findLCA(root->right, n1, n2);
        return root;
    }

    // 5) Преобразование BST в двусвязный список
    void BSTToDLL(Node* root, Node*& head, Node*& prev) {
        if (!root) return;
        BSTToDLL(root->left, head, prev);
        if (!prev) {
            head = root;
        } else {
            prev->right = root;
            root->left = prev;
        }
        prev = root;
        BSTToDLL(root->right, head, prev);
    }

    // 6) Удаление всех узлов с ключами в диапазоне от L до R
    Node* deleteRange(Node* root, int L, int R) {
        if (!root) return nullptr;
        root->left = deleteRange(root->left, L, R);
        root->right = deleteRange(root->right, L, R);
        if (root->value >= L && root->value <= R) {
            if (!root->left) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                Node* temp = root->left;
                delete root;
                return temp;
            } else {
                Node* temp = min_order(root->right);
                root->value = temp->value;
                root->right = deleteRange(root->right, temp->value, temp->value);
            }
        }
        return root;
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

    // 1) Поиск k-того наименьшего элемента
    int kthSmallest(int k) {
        Node* res = kthSmallest(root, k);
        return res ? res->value : -1;
    }

    // 2) Нахождение ближайшего меньшего и большего элемента
    void findPreSuc(int val) {
        Node* pre = nullptr;
        Node* suc = nullptr;
        findPreSuc(root, pre, suc, val);
        std::cout << "Predecessor: " << (pre ? pre->value : -1) << ", Successor: " << (suc ? suc->value : -1) << std::endl;
    }

    // 3) Проверка, является ли BST
    bool isBST() {
        return isBSTUtil(root, INT_MIN, INT_MAX);
    }

    // 4) Поиск ближайшего общего предка (LCA)
    int findLCA(int n1, int n2) {
        Node* lca = findLCA(root, n1, n2);
        return lca ? lca->value : -1;
    }

    // 5) Преобразование BST в двусвязный список
    void BSTToDLL() {
        Node* head = nullptr;
        Node* prev = nullptr;
        BSTToDLL(root, head, prev);
        while (head) {
            std::cout << head->value << " ";
            head = head->right;
        }
        std::cout << std::endl;
    }

    // 6) Удаление всех узлов с ключами в диапазоне от L до R
    void deleteRange(int L, int R) {
        root = deleteRange(root, L, R);
    }

    BST(): root(nullptr) {};

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
    std::cout << "7. Find k-th smallest element\n";
    std::cout << "8. Find predecessor and successor\n";
    std::cout << "9. Check if BST\n";
    std::cout << "10. Find LCA\n";
    std::cout << "11. Convert BST to DLL\n";
    std::cout << "12. Delete nodes in range\n";
}

int main() {
    BST tree;
    int choice, value, k, L, R, n1, n2;
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
            case 7:
                std::cout << "Enter k: ";
                std::cin >> k;
                std::cout << "k-th smallest element: " << tree.kthSmallest(k) << std::endl;
                break;
            case 8:
                std::cout << "Enter value: ";
                std::cin >> value;
                tree.findPreSuc(value);
                break;
            case 9:
                std::cout << (tree.isBST() ? "Is BST" : "Is not BST") << std::endl;
                break;
            case 10:
                std::cout << "Enter two values: ";
                std::cin >> n1 >> n2;
                std::cout << "LCA: " << tree.findLCA(n1, n2) << std::endl;
                break;
            case 11:
                tree.BSTToDLL();
                break;
            case 12:
                std::cout << "Enter range [L, R]: ";
                std::cin >> L >> R;
                tree.deleteRange(L, R);
                break;
        }
    } while (choice != 0);
    return 0;
}