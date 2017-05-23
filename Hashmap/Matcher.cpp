#include "provided.h"
#include <string>
#include "MyHashMap.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class MatcherImpl
{
public:
    MatcherImpl(istream& ruleStream);
	~MatcherImpl();
	std::vector<Rule*> findTheSubsetOfApplicableRulesThatApply(Document& doc) const; 
    void process(Document& doc, double minPrice, vector<Match>& matches) const;
private:
	vector<Rule*> rules;
};

MatcherImpl::MatcherImpl(istream& ruleStream)			// get all the rules, but only store rules written correctly
{
	string rule;
	while (getline(ruleStream, rule))
	{
		Rule s(rule);
		if(s.getNumElements() == 0 )
			continue;
		Rule* ptr = new Rule(rule);
		rules.push_back(ptr);
	}
}

MatcherImpl::~MatcherImpl()
{
	for( unsigned int k = 0; k < rules.size(); k++)
		delete rules[k];
}

std::vector<Rule*> MatcherImpl::findTheSubsetOfApplicableRulesThatApply(Document& doc) const		// find the right rules for the document and store into a vector
{
	vector<Rule*> doc_rule;
	for( unsigned int k= 0; k < rules.size(); k++)
	{
		for( int j = 0; j < rules[k]->getNumElements(); k++)
		{	
			if( doc.contains(rules[k]->getElement(j)))
				doc_rule.push_back(rules[k]);
		}
	}
	return doc_rule;
}

void MatcherImpl::process(Document& doc, double minPrice, vector<Match>& matches) const
{
	std::vector<Rule*> applicableRules = findTheSubsetOfApplicableRulesThatApply(doc);

	for(unsigned int i = 0; i < applicableRules.size(); i++)								// compare each appropriate rules for the document if it matches and price is applicable
	{	if(applicableRules[i]->match(doc) &&
			applicableRules[i]->getDollarValue() >= minPrice)
		{
			Match s;
			s.ad = applicableRules[i]->getAd();
			s.ruleName = applicableRules[i]->getName();
			s.dollarValue = applicableRules[i]->getDollarValue();
			s.url = doc.getURL();
			matches.push_back(s);
		}
	}
}

//******************** Matcher functions *******************************

// These functions simply delegate to MatcherImpl's functions.
// You probably don't want to change any of this code.

Matcher::Matcher(istream& ruleStream)
{
    m_impl = new MatcherImpl(ruleStream);
}

Matcher::~Matcher()
{
    delete m_impl;
}

void Matcher::process(Document& doc, double minPrice, vector<Match>& matches) const
{
    m_impl->process(doc, minPrice, matches);
}
