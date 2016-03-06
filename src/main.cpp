#include "SiteDownloader.hpp"
#include "ThreadSafeQueue.hpp"
#include "UrlsQueueProxy.hpp"
#include "SiteDownloadersManager.hpp"

int main()
{
    ThreadSafeQueue<std::string> urls;
    ThreadSafeQueue<std::string> contents;
    Trie urlsTrie;
    UrlsQueueProxy urlsProxy( urls, urlsTrie );
    SiteDownloadersManager manager;

    manager.startDownloader( urls, contents );

    urlsProxy.push( "www.o2.pl" );

    while( true )
    {
        std::cout<<contents.pop()<<"\n\n\n";
    }

    return 0;
}
