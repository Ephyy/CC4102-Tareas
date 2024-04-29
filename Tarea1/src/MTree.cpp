class MTree {
public:
    MTree() {
        root = new MTreeNode();
    }

    void insert(vector<int> key) {
        MTreeNode* cur = root;
        for (int i = 0; i < key.size(); i++) {
            if (cur->children.find(key[i]) == cur->children.end()) {
                cur->children[key[i]] = new MTreeNode();
            }
            cur = cur->children[key[i]];
        }
    }

    bool search(vector<int> key) {
        MTreeNode* cur = root;
        for (int i = 0; i < key.size(); i++) {
            if (cur->children.find(key[i]) == cur->children.end()) {
                return false;
            }
            cur = cur->children[key[i]];
        }
        return true;
    }

    void remove(vector<int> key) {
        MTreeNode* cur = root;
        vector<MTreeNode*> path;
        for (int i = 0; i < key.size(); i++) {
            if (cur->children.find(key[i]) == cur->children.end()) {
                return;
            }
            path.push_back(cur);
            cur = cur->children[key[i]];
        }
        for (int i = key.size() - 1; i >= 0; i--) {
            if (cur->children.size() > 0) {
                break;
            }
            delete cur;
            cur = path.back();
            path.pop_back();
            cur->children.erase(key[i]);
        }
    }
}