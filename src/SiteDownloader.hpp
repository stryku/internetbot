#pragma once

#include "ThreadSafeQueue.hpp"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include <sstream>

class SiteDownloader
{
private:
    typedef ThreadSafeQueue<std::string> UrlsQueue;
    typedef ThreadSafeQueue<std::string> SiteContentsQueue;

public:
    SiteDownloader( UrlsQueue &urlsQueue,
                    SiteContentsQueue &siteContentsQueue ) :
        urlsQueue( urlsQueue ),
        siteContentsQueue( siteContentsQueue )
    {}

    void run()
    {
        while( true )
        {
            auto url = urlsQueue.pop();
            auto content = download( url );
            siteContentsQueue.push( content );
        }
    }

private:
    std::string download( const std::string &url ) const
    {
        try
        {
            std::ostringstream os;
    		curlpp::options::WriteStream ws( &os );
            curlpp::options::Url myUrl( url );
		    curlpp::Easy myRequest;

		    myRequest.setOpt( ws );
            myRequest.setOpt(myUrl);
		    myRequest.perform();

            return os.str();
        }
        catch ( curlpp::LogicError &e ) 
        {
            std::cerr << e.what() << std::endl;
        }
        catch ( curlpp::RuntimeError &e ) 
        {
            std::cerr << e.what() << std::endl;
        }
    }

    UrlsQueue &urlsQueue;
    SiteContentsQueue &siteContentsQueue;
};
