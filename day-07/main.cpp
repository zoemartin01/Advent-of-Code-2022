#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <set>

enum Command
{
    NONE,
    LS,
    CD
};
enum NodeType
{
    T_DIR,
    T_FILE
};

class Node
{
private:
    std::size_t __size;

public:
    std::string name;
    std::vector<Node *> children;
    NodeType type;
    Node *parent;
    std::size_t level;

    Node(std::string _name, NodeType _type, std::size_t size, Node *_parent)
    {
        name = _name;
        type = _type;
        __size = size;
        parent = _parent;
        level = 0;
    }

    size_t size()
    {
        if (type == T_FILE)
            return __size;

        std::vector<std::size_t> child_sizes;
        std::transform(children.begin(),
                       children.end(),
                       std::back_inserter(child_sizes),
                       [](Node *child)
                       { return child->size(); });
        return std::reduce(child_sizes.begin(), child_sizes.end());
    }

    void add_child(Node *child)
    {
        child->level = level + 1;
        children.push_back(child);
    }

    void set_size(size_t size)
    {
        if (type == T_DIR)
            return;
        __size = size;
    }

    std::string to_string()
    {
        std::stringstream result;
        result << std::string(level, ' ');
        result << "- " << name << " ";
        result << "(" << (type == T_DIR ? "dir" : "file") << ", ";
        result << "parent=" << (parent == nullptr ? "none" : parent->name) << ", ";
        result << "level=" << level << ",";
        result << "size=" << size() << ")";
        return result.str();
    }
};

void readCommand(std::string &line, Command &active_command, std::string &node_name)
{
    std::stringstream stream(line);
    std::vector<std::string> tokens;
    for (std::string token; stream >> token;)
    {
        tokens.push_back(token);
    }

    if (tokens[0] != "$")
        return;

    if (tokens[1] == "ls")
        active_command = LS;
    else if (tokens[1] == "cd")
        active_command = CD;

    if (tokens.size() > 2)
        node_name = tokens[2];
}

Node *executeCD(Command &active_command, Node *active_node, std::string &node_name)
{
    if (active_command != CD)
        return active_node;

    if (node_name == "..")
    {
        return active_node->parent;
    }

    if (node_name == "/")
    {
        while (active_node->level != 0)
            active_node = active_node->parent;
        return active_node;
    }

    for (Node *child : active_node->children)
    {
        if (child->name == node_name)
        {
            return child;
        }
    }
    Node *new_node = new Node(node_name, T_DIR, 0, active_node);
    active_node->add_child(new_node);
    active_node = new_node;
    return active_node;
}

void readLS(Node *active_node, std::string &token, std::string &node_name)
{
    if (token == "dir")
    {
        for (Node *child : active_node->children)
        {
            if (child->name == node_name)
            {
                return;
            }
        }
        Node *new_node = new Node(node_name, T_DIR, 0, active_node);
        active_node->add_child(new_node);
    }
    else
    {
        size_t size = std::stoi(token);
        for (Node *child : active_node->children)
        {
            if (child->name == node_name)
            {
                child->set_size(size);
                return;
            }
        }
        Node *new_node = new Node(node_name, T_FILE, size, active_node);
        active_node->add_child(new_node);
    }
}

Node *parse()
{
    std::ifstream infile("input.txt");
    Command active_command = NONE;
    bool skip = true;
    Node *active_node = new Node("/", T_DIR, 0, nullptr);
    for (std::string line, node_name; std::getline(infile, line);)
    {
        if (skip)
        {
            skip = false;
            continue;
        }

        if (line[0] == '$')
        {
            readCommand(line, active_command, node_name);
            active_node = executeCD(active_command, active_node, node_name);
            continue;
        }

        if (active_command != LS)
            continue;

        std::stringstream stream(line);
        std::string t, n;
        stream >> t >> n;

        readLS(active_node, t, n);
    }
    while (active_node->level != 0)
        active_node = active_node->parent;
    return active_node;
}

void printTree(Node *node)
{
    std::cout << node->to_string() << std::endl;
    for (Node *child : node->children)
    {
        printTree(child);
    }
}

void find(Node *node, std::set<Node *> *nodes, std::size_t threshold, bool below)
{
    if (node->type == T_FILE)
        return;
    if (below ? node->size() <= threshold : node->size() >= threshold)
        nodes->insert(node);
    for (Node *child : node->children)
        find(child, nodes, threshold, below);
}

std::size_t part_one(Node *node)
{
    auto *nodes = new std::set<Node *>();
    find(node, nodes, 100000, true);
    std::vector<std::size_t> sizes;
    std::transform(
        nodes->begin(), nodes->end(), std::back_inserter(sizes), [](Node *n)
        { return n->size(); });
    return std::reduce(sizes.begin(), sizes.end());
}

std::size_t part_two(Node *node)
{
    std::size_t unused = 70000000 - node->size();
    std::size_t needed = 30000000 - unused;

    auto *nodes = new std::set<Node *>();
    find(node, nodes, needed, false);
    std::vector<size_t> sorted;
    std::transform(nodes->begin(), nodes->end(), std::back_inserter(sorted),
                   [](Node *n)
                   { return n->size(); });
    std::sort(sorted.begin(), sorted.end());
    return sorted.front();
}

int main()
{
    Node *base_node = parse();
    printTree(base_node);

    std::cout << "Part One: " << part_one(base_node) << std::endl;
    std::cout << "Part Two: " << part_two(base_node) << std::endl;
    return 0;
}
