struct Node;

struct Trie {
    Node *root = nullptr;
    
    Trie();
    
    ~Trie();
    
    void addString(const unsigned char *str);
};
