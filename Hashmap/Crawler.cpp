#include "provided.h"
#include "http.h"
#include <queue>
#include <string>
using namespace std;

class CrawlerImpl
{
public:
    CrawlerImpl(string seedSite);
	~CrawlerImpl(){};
    Document* crawl();
private:
	string root_domain;
	string sub_domains;
	queue<string> same_domain;		// to hold all the url with same domain together
};

CrawlerImpl::CrawlerImpl(string seedSite)
	:root_domain(seedSite)
{same_domain.push(seedSite);}

Document* CrawlerImpl::crawl()
{
	Document* content;
	if( content != NULL)
		delete content;
	if( !same_domain.empty())
	{	
		string links;
		string pageContents;
		sub_domains = same_domain.front();		//get the first domain added into the queue
		same_domain.pop();
       if (HTTP().get(sub_domains, pageContents))
        {  
			ExtractLinks website(pageContents);			// creates a ExtractLinks object to peruse through webpage and get all the links
			while(website.getNextLink(links))		// while there are still links on webpage
			{
					Tokenizer t(seed,"//");			//get the url
					string token;
					if(t.getNextToken(token))
						string seedSite = token;
					else
						string seedSite = url;
					int epos = seedSite.find('/');			// find where seed site domain name cuts off
					string Site = seedSite.substr(0, epos-1);	// gets domain name
					Tokenizer t(url,"//");			//get the url
					string token;
					if(t.getNextToken(token))
						string url_link = token;
					else
						string url_link = url;
					int url_epos = Site.find('/');		// find where url site domain name cuts off
					string url_site = url_link.substr(0, url_epos-1);			// gets domain name
					if( url_site.compare(seedSite) == 0)
						same_domain.push(links);					// add to the queue if the same
			}
			content = new Document(sub_domains, pageContents);				// create a new Document for it
			return content;
	   }
	   return NULL;
	}
	else
		return NULL;
}
//******************** Crawler functions *******************************

// These functions simply delegate to CrawlerImpl's functions.
// You probably don't want to change any of this code.

Crawler::Crawler(string seedSite)
{
    m_impl = new CrawlerImpl(seedSite);
}

Crawler::~Crawler()
{
    delete m_impl;
}

Document* Crawler::crawl()
{
    return m_impl->crawl();
}