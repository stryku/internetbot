#pragma once

#include "Trie.hpp"

class EmailsContainer
{
public:
    void push_back( const std::string &email )
    {
        emailsTrie.insert( email );
    }

    size_t size() const
    {
        return emailsTrie.size();
    }

    size_t realSize() const
    {
        return emailsTrie.realSize();
    }

private:
    Trie emailsTrie;
};
