#pragma once

#include "SiteDownloader.hpp"

#include <thread>
#include <vector>

class SiteDownloadersManager
{
private:
    typedef ThreadSafeQueue<std::string> UrlsQueue;
    typedef ThreadSafeQueue<std::string> SiteContentsQueue;

public:
    void startDownloader( UrlsQueue &urlsQueue,
                          SiteContentsQueue &siteContentsQueue )
    {
        downloaders.push_back( {urlsQueue, siteContentsQueue } );

        auto &downloader = downloaders.back();

        threads.push_back( std::thread{ &SiteDownloader::run, &downloader } );
    }

private:
    std::vector<SiteDownloader> downloaders;
    std::vector<std::thread> threads;
};
