#include <bits/stdc++.h>

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(): val(0), left(nullptr), right(nullptr) {}
    explicit TreeNode(const int x): val(x), left(nullptr), right(nullptr) {}
    TreeNode(const int x, TreeNode* left, TreeNode* right): val(x), left(left), right(right) {}
};

TreeNode* invertTree(TreeNode* root) {
    std::stack<TreeNode*> stack;
    stack.push(root);

    while (!stack.empty()) {
        auto* node = stack.top();
        stack.pop();

        if (node) {
            stack.push(node->left);
            stack.push(node->left);

            std::swap(node->left, node->right);
        }
    }

    return root;
}

int maxDepth(TreeNode* root) {
    if (!root) return 0;

    std::queue<TreeNode*> queue;
    queue.push(root);

    int depth = 0;
    while (!queue.empty()) {
        const size_t level_size = queue.size();

        for (size_t i = 0; i < level_size; ++i) {
            const TreeNode* node = queue.front();
            queue.pop();

            if (node->left) queue.push(node->left);
            if (node->right) queue.push(node->right);
        }

        depth++;
    }

    return depth;
}

bool isSame(TreeNode* p, TreeNode* q) {
    std::stack<std::pair<TreeNode*, TreeNode*>> stack;
    stack.emplace(p, q);

    while (!stack.empty()) {
        const auto [node1, node2] = stack.top();
        stack.pop();

        if (!node1 and !node2) continue;

        if (!node1 or !node2 or node1->val != node2->val) return false;

        stack.emplace(node1->right, node2->right);
        stack.emplace(node1->left, node2->left);
    }

    return true;
}

bool isSubtree(TreeNode* root, TreeNode* sub_root) {
    if (!root) return false;

    return (root->val == sub_root->val and isSame(root, sub_root)) or isSubtree(root->left, sub_root) or isSubtree(
        root->right, sub_root);
}

TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root) return nullptr;

    const int current = root->val;
    if (current > p->val and current > q->val) return lowestCommonAncestor(root->left, p, q);
    if (current < p->val and current < q->val) return lowestCommonAncestor(root->right, p, q);

    return root;
}

std::vector<std::vector<int>> levelOrder(TreeNode* root) {
    if (!root) return {};

    std::queue<TreeNode*> queue;
    queue.emplace(root);

    std::vector<std::vector<int>> level_order;
    while (!queue.empty()) {
        std::vector<int> level;
        const size_t size = queue.size();
        for (size_t i = 0; i < size; ++i) {
            const auto node = queue.front();
            queue.pop();

            level.emplace_back(node->val);
            if (node->left) queue.push(node->left);
            if (node->right) queue.push(node->right);
        }

        level_order.emplace_back(level);
    }

    return level_order;
}

bool isValidBST(const TreeNode* root, const long min_possible, const long max_possible) {
    if (!root) return true;

    if (root->val <= min_possible or root->val >= max_possible) return false;

    return isValidBST(root->left, min_possible, root->val) and isValidBST(root->right, root->val, max_possible);
}

bool isValidBST(const TreeNode* root) {
    return isValidBST(root, LONG_MIN, LONG_MAX);
}

int kthSmallest(TreeNode* root, const int k) {
    std::stack<TreeNode*> stack;
    auto current = root;

    int n = 0;
    while (current or !stack.empty()) {
        while (current) {
            stack.emplace(current);
            current = current->left;
        }

        current = stack.top();
        stack.pop();

        n++;
        if (n == k) return current->val;

        current = current->right;
    }

    return -1;
}

TreeNode* buildTree(const std::vector<int>& preorder, const size_t preorder_start, const size_t preorder_end,
                    const std::vector<int>& inorder, const size_t inorder_start, const size_t inorder_end,
                    const std::unordered_map<int, size_t>& inorder_map) {
    if (preorder_start > preorder_end or inorder_start > inorder_end) return nullptr;

    auto* root = new TreeNode(preorder[preorder_start]);

    const size_t inorder_root = inorder_map.at(root->val);
    const size_t nums_left = inorder_root - inorder_start;

    root->left = buildTree(preorder, preorder_start + 1, preorder_start + nums_left, inorder, inorder_start,
                           inorder_root - 1, inorder_map);
    root->right = buildTree(preorder, preorder_start + nums_left + 1, preorder_end, inorder, inorder_root + 1,
                            inorder_end, inorder_map);

    return root;
}

TreeNode* buildTree(const std::vector<int>& preorder, const std::vector<int>& inorder) {
    std::unordered_map<int, size_t> inorder_map;
    for (size_t i = 0; i < inorder.size(); ++i) inorder_map[inorder[i]] = i;

    TreeNode* root = buildTree(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1, inorder_map);

    return root;
}

int maxPathSum(const TreeNode* root, int& max_path) {
    if (!root) return 0;
    // Find max path sum for left and right, ignoring negatives
    const int left_sum = std::max(0, maxPathSum(root->left, max_path)),
              right_sum = std::max(0, maxPathSum(root->right, max_path));

    max_path = std::max(max_path, root->val + left_sum + right_sum);
    // Return max path sum including current node
    return root->val + std::max(left_sum, right_sum);
}

int maxPathSum(const TreeNode* root) {
    int max_path_sum = INT_MIN;

    maxPathSum(root, max_path_sum);

    return max_path_sum;
}

class Codec {
public:
    /// Encodes a tree to a single string
    static std::string serialize(TreeNode* root) {
        if (!root) return "";

        std::string serialized{};
        std::queue<TreeNode*> queue;
        queue.push(root);

        while (!queue.empty()) {
            const TreeNode* current = queue.front();
            queue.pop();

            if (!current) serialized.append("#,");
            else serialized.append(std::to_string(current->val) + ',');

            if (current) {
                queue.push(current->left);
                queue.push(current->right);
            }
        }

        return serialized;
    }

    /// Decodes your encoded data to a tree
    static TreeNode* deserialize(const std::string& serialized) {
        if (serialized.empty()) return nullptr;

        const auto tokens = serialized
            | std::views::split(',') | std::views::transform([](auto&& range) {
                return std::string_view(range.begin(), range.end());
            })
            | std::ranges::to<std::vector>();

        auto it = tokens.begin();
        const auto root_val = *it++;
        if (root_val == "#") return nullptr;

        auto* root = new TreeNode(std::stoi(std::string(root_val)));
        std::queue<TreeNode*> queue;
        queue.push(root);

        while (!queue.empty()) {
            auto* current = queue.front();
            queue.pop();

            if (it != tokens.end()) {
                if (const auto left_val = *it++; left_val != "#") {
                    current->left = new TreeNode(std::stoi(std::string(left_val)));
                    queue.push(current->left);
                }
            }
            if (it != tokens.end()) {
                if (const auto right_val = *it++; right_val != "#") {
                    current->left = new TreeNode(std::stoi(std::string(right_val)));
                    queue.push(current->left);
                }
            }
        }

        return root;
    }
};
