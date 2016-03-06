#pragma once

#include "ThreadSafeQueue.hpp"
#include "Trie.hpp"

#include <boost/algorithm/string/predicate.hpp>

class UrlsQueueProxy
{
public:
    UrlsQueueProxy( ThreadSafeQueue<std::string> &urlsQueue,
                    Trie &urlsTrie ) :
        urlsQueue( urlsQueue ),
        urlsTrie( urlsTrie )
    {}

    bool push( const std::string &url )
    {
        if( validUrl( url ) && urlsTrie.insert( url ) )
        {
            urlsQueue.push( url );
            return true;
        }

        return false;
    }

private:
    bool validUrl( const std::string &url )
    {
        if( !validProtocol( url ) )
            return false;

        return true;
    }

    bool validProtocol( const std::string &url ) const
    {
        static auto invalidProtocols = { "ftp", "ftps", "dict", "file", "gopher", "imap", "imaps", "ldap", "ldaps", "pop3", "pop3s", "rtmp", "rtsp", "scp", "sftp", "smb", "smbs", "smtp", "smtps" };

        for( const auto &protocol : invalidProtocols )
            if( boost::starts_with( url, protocol ) )
                return false;

        return true;
    }

    ThreadSafeQueue<std::string> &urlsQueue;
    Trie &urlsTrie;
};
