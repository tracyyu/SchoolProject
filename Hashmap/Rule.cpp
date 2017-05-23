#include "provided.h"
//#include "support.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
using namespace std;

class RuleImpl
{
public:
    RuleImpl(string ruleText);
	bool isPostfix(vector<string> elements);
    string getName() const;
    double getDollarValue() const;
    int getNumElements() const;
    string getElement(int elementNum) const;
    string getAd() const;
    bool match(const Document& doc) const;
private:
	string Rule;
	string Name;
	float DollarValue;
	vector<string> elements;
	std::string ToLower(const std::string& text)
	{ 
	std::string key ="";
	for(unsigned int k = 0; k < text.size(); k++)
	{	tolower(text[k]);
		key+=text[k];
	}
	return key;
	}
	vector<string> required;
	vector<string> optional;
	int element_count;
	string Advertisement;
	bool goodRule;
};

RuleImpl::RuleImpl(string ruleText)
	:Rule(ruleText), DollarValue(0), element_count(0),				// divide the rule text into proper arguments
	goodRule(true)
{
	Tokenizer t(ruleText," ");
	string token;
	string name;
	name = t.getNextToken(token);
	if( name.find("rule-")== 0)
		Name = name;								// get name of rule
	else
		goodRule = false;
	string value;
	 value = t.getNextToken(token);
	 DollarValue = atof(value.c_str());				// get dollar value
	 string postfix;
	 int operators = 0;
	 int operands = 0;
	 while( t.getNextToken(token) && token != "<")				// return the number of elements
	 {
		 if( token == "&" || token == "|")
			 operators++;
		 else if( operators >= operands)
			 goodRule = false;
		 else
		 {
			operands++;
			ToLower(token);
			elements.push_back(token);
		 }
		element_count++;
	 }
	 if(operators >= operands)
		 goodRule = false;
														// then check the postfix				
	if(!isPostfix(elements)) 
		goodRule = false;
	int b_ad = Rule.find("<");
	string Advertisement = Rule.substr( b_ad+2, Rule.size() - (b_ad+2));
	if( Advertisement == "")
		goodRule = false;
}

bool RuleImpl::isPostfix(vector<string> elements)	// checks postfix, if it is, postfix  equation, else postfix is empty
{
	required.clear();
	optional.clear();
	if( elements.size() == 0)
	{
		return false;
	}
	vector<string>postfix;
	for( unsigned int k = 0; k < elements.size(); k++)
	{
		string key = elements[k];
		string key1;
		string key2;
		if( key == "&")
		{		if(postfix.size() >= 2)
				{
					key1=postfix.back();
					postfix.pop_back();
					required.push_back(key1);
					key2=postfix.back();
					postfix.pop_back();
					required.push_back(key2);
				}
				else
				{
					key1=postfix.back();
					postfix.pop_back();
					required.push_back(key1);
				}
		}
		else if( key == "|")
		{		if(postfix.size() >= 2)
				{
					key1=postfix.back();
					postfix.pop_back();
					optional.push_back(key1);
					key2=postfix.back();
					postfix.pop_back();
					optional.push_back(key2);
				}
				else
				{
					key1=postfix.back();
					postfix.pop_back();
					optional.push_back(key1);
				}
		}
		else
		{		postfix.push_back(key);
		}
	}
	if( postfix.size() > 0)
		return false;
	return true;
}

string RuleImpl::getName() const
{
    return Name;  
}

double RuleImpl::getDollarValue() const
{
    return DollarValue; 
}

int RuleImpl::getNumElements() const
{
	if( goodRule == false)
		return 0;
    return element_count;  
}

string RuleImpl::getElement(int elementNum) const
{
    return elements[elementNum];  
}

string RuleImpl::getAd() const
{
    return Advertisement;  
}

bool RuleImpl::match(const Document& doc) const			// check if the document matches any of the rules
{
	if( goodRule == false)
		return false;
	for( unsigned int k = 0; k < required.size(); k++)
	{	
		string key = required[k];
		if(!doc.contains(key))
			return false;
		else 
			return true;
	}
	for( unsigned int k = 0; k < optional.size(); k++)
	{
		string key = optional[k];
		if(doc.contains(key))
			return true;
		if(!doc.contains(key) && (k == optional.size()-1))
			return false;
	}
    return true;  
}

//******************** Rule functions *******************************

// These functions simply delegate to RuleImpl's functions.
// You probably don't want to change any of this code.

Rule::Rule(string ruleText)
{
    m_impl = new RuleImpl(ruleText);
}

Rule::~Rule()
{
    delete m_impl;
}

Rule::Rule(const Rule& other)
 : m_impl(new RuleImpl(*other.m_impl))
{}

Rule& Rule::operator=(const Rule& other)
{
    if (this != &other)
    {
        RuleImpl* copyOfOther = new RuleImpl(*other.m_impl);
        delete m_impl;
        m_impl = copyOfOther;
    }
    return *this;
}

string Rule::getName() const
{
    return m_impl->getName();
}

double Rule::getDollarValue() const
{
    return m_impl->getDollarValue();
}

int Rule::getNumElements() const
{
    return m_impl->getNumElements();
}

string Rule::getElement(int elementNum) const
{
    return m_impl->getElement(elementNum);
}

string Rule::getAd() const
{
    return m_impl->getAd();
}

bool Rule::match(const Document& doc) const
{
    return m_impl->match(doc);
}
