#pragma once 

#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <algorithm>

class Trie
{
public:
    Trie() :
        root( std::make_shared<TrieStruct>() )
    {}
    Trie( const Trie & ) = delete;
    Trie& operator=( const Trie & ) = delete;

    bool insert( const std::string &str )
    {
        if( !isValidString( str ) )
            return false;

        std::lock_guard<std::mutex> lock( sync );

        auto currentPtr = root;

        for( const char c : str )
        {
            auto ptrIndex = charToPtrIndex[c];
            currentPtr = currentPtr->childrens[ptrIndex];

            if( currentPtr == nullptr )
                currentPtr = std::make_shared<TrieStruct>();
        }
        
        if( !currentPtr->endOfString )
        {
            currentPtr->endOfString = true;
            ++stringsInTrie;
        }

        return true;
    }

    bool isInside( const std::string &str ) const
    {
        if( !isValidString( str ) )
            return false;

        std::lock_guard<std::mutex> lock( sync );

        auto currentPtr = root;

        for( const char c : str )
        {
            auto ptrIndex = charToPtrIndex[c];
            currentPtr = currentPtr->childrens[ptrIndex];

            if( currentPtr == nullptr )
                return false;
        }

        return currentPtr->endOfString;
    }

    size_t realSize() const
    {
        return root->realSize;
    }

    size_t size() const
    {
        return stringsInTrie;
    }

private:
    struct TrieStruct
    {
        typedef std::shared_ptr<TrieStruct> TrieStructPtr;
        static std::atomic<unsigned long long> realSize;

        std::vector<TrieStructPtr> childrens;
        bool endOfString = false;

        TrieStruct()
        {
            childrens.resize( validChars().length() );
            realSize += realTrieSize(); 
        }

        static const size_t realTrieSize()
        {
            return validChars().length() * sizeof( TrieStructPtr ) + sizeof( bool );
        }
    };

    static const std::string& validChars()
    {
        static const std::string chars{ "qwertyuiopasdfghjklzxcvbnm?./&;=+:!1234567890_" };
        return chars;
    }
    
    static bool isValidString( const std::string &str )
    {
        auto isValidChar = []( const char c ) 
        { 
            return validChars().find( c ) != std::string::npos;
        };

        return std::all_of( str.begin(),
                            str.end(),
                            isValidChar );
    }

    static std::mutex sync;
    std::array<size_t, 128> charToPtrIndex;
    std::atomic<size_t> stringsInTrie{0};

    TrieStruct::TrieStructPtr root;
};
