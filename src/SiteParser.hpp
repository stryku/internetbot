#pragma once

#include "ThreadSafeQueue.hpp"
#include "EmailsContainer.hpp"

#include <regex>
#include <vector>
#include <string>

class SiteParser
{
private:
    typedef ThreadSafeQueue<std::string> UrlsQueue;
    typedef ThreadSafeQueue<std::string> SiteContentsQueue;

public:
    SiteParser( UrlsQueue &urlsQueue,
                SiteContentsQueue &siteContentsQueue,
                EmailsContainer &emailsContainer ) :
        urlsQueue( urlsQueue ),
        siteContentsQueue( siteContentsQueue ),
        emailsContainer( emailsContainer )
    {}

    void run()
    {
        while( true )
            parse( siteContentsQueue.pop() );
    }

private:
    auto extract( const std::string &src, const std::regex &pattern ) const 
    {
        std::vector<std::string> result;

        auto begin = std::sregex_iterator( src.begin(), 
                                           src.end(), 
                                           pattern );
        auto end = std::sregex_iterator();

        std::copy( begin,
                   end,
                   std::back_inserter( result ) );

        return result;
    }

    auto extractUrls( const std::string &siteContent ) const
    {
        static std::regex urlRegex{ "<a\\s+(?:[^>]*?\\s+)?href=\"([^\"]*)\"" };
        return extract( siteContent, urlRegex );
    }

    auto extractEmails( const std::string &siteContent ) const
    {
        static std::regex emailRegex{ "\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,}\\b" };
        return extract( siteContent, emailRegex );
    }

    void parse( const std::string &siteContent ) 
    {
        auto urls = extractUrls( siteContent );

        for( const auto &url : urls )
            urlsQueue.push( url );

        auto emails = extractEmails( siteContent );

        std::copy( emails.begin(),
                   emails.end(),
                   std::back_inserter( emailsContainer ) );
    }

    UrlsQueue &urlsQueue;
    SiteContentsQueue &siteContentsQueue;
    EmailsContainer &emailsContainer;
};
