#include "Trie.hpp"

std::mutex Trie::sync;
std::atomic<unsigned long long> Trie::TrieStruct::realSize{ 0 };
