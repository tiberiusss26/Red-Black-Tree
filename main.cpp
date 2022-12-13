#include <fstream>

using namespace std;

ifstream fin("grader_test7.in");
ofstream fout("abce.out");

enum Color {
    RED, BLACK
};

struct Node {
    int key;
    Node *left;
    Node *right;
    Node *parent;
    bool color; // 0 = red, 1 = black;

    explicit Node(int key) {
        this->key = key;
        left = right = parent = nullptr;
        color = RED;
    }
};

class RBT {
    Node *root;
public:
    explicit RBT(Node *root) : root(root) {}

    RBT() { root = nullptr; }

    virtual ~RBT() = default;

    static void rbTransplant(Node *&root, Node *&u, Node *&v) {
        if (u->parent == nullptr) {
            root = v;
            root->parent = nullptr;
            v->parent = nullptr;
        } else {
            if (u == u->parent->left) {
                if (v != nullptr)
                    u->parent->left = v;
                else
                    u->parent->left = nullptr;
            } else {
                if (v != nullptr)
                    u->parent->right = v;
                else
                    u->parent->right = nullptr;
            }
            if (v != nullptr)
                v->parent = u->parent;
        }
    }

    void rbDelete(Node *&root1, Node *&z) {
        Node *y = z;
        Node *x;
        bool yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            rbTransplant(root1, z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            rbTransplant(root1, z, z->left);
        } else {
            y = treeMinimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y;
            } else {
                rbTransplant(root1, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            y->left = z->left;
            rbTransplant(root1, z, y);
            if (y->left != nullptr) {
                y->left->parent = y;
            }
            y->color = z->color;
            if (y->right == nullptr)
                x = nullptr;
        }
        if (yOriginalColor == BLACK) {
            rbDeleteFixup(root1, x);
        }
    }

    static void rbDeleteFixup(Node *root2, Node *x) {
        if (x != nullptr && x->color == BLACK) {
            while (x != root2 && x->color == BLACK) {
                if (x == x->parent->left) {
                    Node *w = x->parent->right;
                    if (w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        leftRotate(root2, x->parent);
                        w = x->parent->right;
                    }
                    if (w->left->color == BLACK && w->right->color == BLACK) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if (w->right->color == BLACK) {
                            w->left->color = BLACK;
                            w->color = RED;
                            rightRotate(root2, w);
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        w->right->color = BLACK;
                        leftRotate(root2, x->parent);
                        x = root2;
                    }
                } else {
                    Node *w = x->parent->left;
                    if (w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        rightRotate(root2, x->parent);
                        w = x->parent->left;
                    }
                    if (w->right->color == BLACK && w->left->color == BLACK) {
                        w->color = RED;
                        x = x->parent;
                    } else {
                        if (w->left->color == BLACK) {
                            w->right->color = BLACK;
                            w->color = RED;
                            leftRotate(root2, w);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        w->parent->color = BLACK;
                        rightRotate(root2, x->parent);
                        x = root2;
                    }
                }
            }
        }
        if (x != nullptr)
            x->color = BLACK;
    }

    static void leftRotate(Node *&root, Node *&x) {
        Node *y = x->right;
        x->right = y->left;

        if (x->right != nullptr)
            x->right->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    static void rightRotate(Node *&root, Node *&x) {
        Node *y = x->left;

        x->left = y->right;

        if (x->left != nullptr)
            x->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;


        y->right = x;
        x->parent = y;
    }

    Node *bstInsert(Node *root, Node *x) {
        if (root == nullptr)
            return x;
        if (x->key < root->key) {
            root->left = bstInsert(root->left, x);
            root->left->parent = root;
        } else if (x->key > root->key) {
            root->right = bstInsert(root->right, x);
            root->right->parent = root;
        }
        return root;
    }

    void insert(const int &newKey) {
        Node *z = new Node(newKey);

        root = bstInsert(root, z);
//
        insertFixup(root, z);
    }

    static void insertFixup(Node *&root, Node *&z) {
        Node *parent = nullptr;
        Node *grandParent = nullptr;

        while ((z != root) && (z->color != BLACK) && (z->parent->color == RED)) {
            parent = z->parent;
            grandParent = z->parent->parent;

            if (parent == grandParent->left) {
                Node *y = grandParent->right;  // unchiul

                if (y != nullptr && y->color == RED) {
                    grandParent->color = RED;
                    parent->color = BLACK;
                    y->color = BLACK;
                    z = grandParent;
                } else {
                    if (z == parent->right) {
                        leftRotate(root, parent);
                        z = parent;
                        parent = z->parent;
                    }
                    rightRotate(root, grandParent);
                    swap(parent->color, grandParent->color);
                    z = parent;
                }
            } else {
                Node *y = grandParent->left;
                if ((y != nullptr) && (y->color == RED)) {
                    grandParent->color = RED;
                    parent->color = BLACK;
                    y->color = BLACK;
                    z = grandParent;
                } else {
                    if (z == parent->left) {
                        rightRotate(root, parent);
                        z = parent;
                        parent = z->parent;
                    }
                    leftRotate(root, grandParent);
                    swap(parent->color, grandParent->color);
                    z = parent;
                }
            }
        }

        root->color = BLACK;
    }

    Node *treeMinimum(Node *x) {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

    int op4(int x) {
        Node *y = root;

        while (y->key > x)
            y = y->left;

        while (y->right != nullptr && x > y->right->key)
            y = y->right;

        return y->key;
    }

    int op5(int x) {
        Node *y = root;
        while (y->key < x) {
            y = y->right;
        }
        while (y->left != nullptr && x < y->left->key)
            y = y->left;

        return y->key;
    }

    Node *getRoot() const {
        return root;
    }

    void printInorder(Node *rooot) {
        if (rooot == nullptr)
            return;
        printInorder(rooot->left);
        fout << rooot->key << " ";
        printInorder(rooot->right);
    }

    Node *findNode(int val, int op) {
        if (op == 3) {
            if (root == nullptr) {
                fout << "0\n";
                return nullptr;
            }
        }
        Node *x = root;
        while (x != nullptr) {
            if (val < x->key) {
                if (x->left == nullptr) {
                    if (op == 3)
                        fout << "0\n";
                    return nullptr;
                } else
                    x = x->left;
            } else if (val == x->key) {
                if (op == 3)
                    fout << "1\n";
                break;
            } else {
                if (x->right == nullptr) {
                    if (op == 3)
                        fout << "0\n";
                    return nullptr;
                } else
                    x = x->right;
            }
        }
        return x;
    }

    void setRoot(Node *root) {
        RBT::root = root;
    }
};

int main() {
    int n, op, x, y;
    RBT tree;

    fin >> n;
    for (int i = 0; i < n; ++i) {
        fin >> op;
        if (op == 1) {
            fin >> x;
            tree.insert(x);
        } else if (op == 2) {
            fin >> x;
            if (tree.findNode(x, 2) != nullptr) {
                Node *root = tree.getRoot();
                Node *del = tree.findNode(x, 2);
                tree.rbDelete(root, del);

                tree.setRoot(root);

            }
        } else if (op == 3) {
            fin >> x;
            tree.findNode(x, 3);
        } else if (op == 4) {
            fin >> x;
            fout << tree.op4(x) << "\n";
        } else if (op == 5) {
            fin >> x;
            fout << tree.op5(x) << "\n";
        } else if (op == 6) {
            fin >> x >> y;
        }
    }
}
