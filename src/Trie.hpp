#pragma once 

#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <algorithm>

class Trie
{
public:
    Trie()
    {
        static std::once_flag once;

        std::call_once( once, [this] { validCharsLen = validChars().length(); } );
        root = std::make_shared<TrieStruct>();
    }
    Trie( const Trie & ) = delete;
    Trie& operator=( const Trie & ) = delete;

    bool insert( const std::string &str )
    {
        if( !isValidString( str ) )
            return false;

        auto& currentPtr = root;

        for( const char c : str )
        {
            auto ptrIndex = charToPtrIndex[c];
            currentPtr = currentPtr->childrens[ptrIndex];

            if( currentPtr == nullptr )
                currentPtr = std::make_shared<TrieStruct>();
        }

        currentPtr->endOfString = true;

        return true;
    }

    bool isInside( const std::string &str ) const
    {
        if( !isValidString( str ) )
            return false;

        auto& currentPtr = root;

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

private:
    struct TrieStruct
    {
        typedef std::shared_ptr<TrieStruct> TrieStructPtr;
        static std::atomic<unsigned long long> realSize;

        std::vector<TrieStructPtr> childrens;
        bool endOfString = false;

        TrieStruct()
        {
            childrens.resize( validCharsLen );
            realSize += realTrieSize(); 
        }

        static const size_t realTrieSize()
        {
            return validCharsLen * sizeof( TrieStructPtr ) + sizeof( bool );
        }
    };

    static const std::string& validChars()
    {
        static const std::string chars{ "qwertyuiopasdfghjklzxcvbnm?./&;=+:!1234567890" };
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

    static size_t validCharsLen;
    std::array<size_t, 128> charToPtrIndex;

    TrieStruct::TrieStructPtr root;
};