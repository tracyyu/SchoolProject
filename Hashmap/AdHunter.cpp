#include "provided.h"
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class AdHunterImpl
{
public:
    AdHunterImpl(istream& ruleStream);
    void addSeedSite(string site);
    int getBestAdTargets(double minPrice, int pagesPerSite, vector<Match>& matches);
	//bool operator>(const Match& a, const Match& b );
private:
	vector<string>websites;
	Matcher content;
	int total_matches;
};

AdHunterImpl::AdHunterImpl(istream& ruleStream)
	:	content(ruleStream)
{}

void AdHunterImpl::addSeedSite(string site)
{
	websites.push_back(site);

}

int AdHunterImpl::getBestAdTargets(double minPrice, int pagesPerSite, vector<Match>& matches)
{
	matches.clear();										// clear any existing items in the match vector
	for( unsigned int k = 0; k < websites.size(); k++)
	{	
		Crawler s(websites[k]);								// crawls through the url
		while(s.crawl() != NULL)
		{
			content.process(*(s.crawl()), minPrice, matches);			//process the document passed upon the parameters
			total_matches = matches.size();
			sort(matches.begin(), matches.end());	
			reverse(matches.begin(), matches.end());				// reverse the sorted vector
		}
	}
    return total_matches;							// returns the number of total matches for the document
}

bool operator<(const Match& a, const Match& b )			// write the operator for comparing two matches
{
	if( b.dollarValue > a.dollarValue)
		return true;
	else if( a.dollarValue > b.dollarValue)
		return false;
	else if(b.url > a.url)
		return false;
	else if( a.url > b.url)
		return true;
	else if( a.ruleName > b.ruleName)
		return true;
	else
		return false;
}


//******************** AddHunter functions *******************************

// These functions simply delegate to AdHunterImpl's functions.
// You probably don't want to change any of this code.

AdHunter::AdHunter(istream& ruleStream)
{
    m_impl = new AdHunterImpl(ruleStream);
}

AdHunter::~AdHunter()
{
    delete m_impl;
}

void AdHunter::addSeedSite(string site)
{
    return m_impl->addSeedSite(site);
}

int AdHunter::getBestAdTargets(double minPrice, int pagesPerSite, vector<Match>& matches)
{
    return m_impl->getBestAdTargets(minPrice, pagesPerSite, matches);
}