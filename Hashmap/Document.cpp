#include "provided.h"
//#include "support.h"
#include "MyHashMap.h"
#include <string>
using namespace std;

class DocumentImpl 
{
public:
    DocumentImpl(string url, const string& text);
    string getURL() const;
    bool contains(string word) const;
    bool getFirstWord(string& word);
    bool getNextWord(string& word);
private:
	string url;
	string body;
	MyHashMap<int> m_document;
	std::string ToLower( std::string& text) const
	{ 
	std::string key ="";
	for(unsigned int k = 0; k < text.size(); k++)
	{	tolower(text[k]);
		key+=text[k];
	}
	return key;
	}
};

DocumentImpl::DocumentImpl(string url, const string& text)
	:url(url)
{
	 body = text;
	Tokenizer s( body, " ,?/!.;\"<>");
	string token;
	while(s.getNextToken(token))
	{
		m_document.associate(ToLower(token), 0);	// stores unique text into a map
	}
}


string DocumentImpl::getURL() const
{
    return url;  
}

bool DocumentImpl::contains(string word) const		// looks at the n body
{
	string key = ToLower(word);
	if(m_document.find(key) != NULL)
		return true;
    return false;  // This is not always correct; it's just here to compile.
}

bool DocumentImpl::getFirstWord(string& word)
{
	string content;
	if((m_document.getFirst(content)) != NULL)
	{
		word = content;
		return true;
	}
	else 
		return false;
    //return false;  // This is not always correct; it's just here to compile.
}

bool DocumentImpl::getNextWord(string& word)
{
	string content;
	if(m_document.getFirst(content) && m_document.getNext(content) != NULL)		// get teh next word in the document
	{
		word = content;
		return true;
	}
	else
		return false;
}


//******************** Document functions *******************************

// These functions simply delegate to DocumentImpl's functions.
// You probably don't want to change any of this code.

Document::Document(string url, const string& text)
{
    m_impl = new DocumentImpl(url, text);
}

Document::~Document()
{
    delete m_impl;
}

string Document::getURL() const
{
    return m_impl->getURL();
}

bool Document::contains(string word) const
{
    return m_impl->contains(word);
}

bool Document::getFirstWord(string& word)
{
    return m_impl->getFirstWord(word);
}

bool Document::getNextWord(string& word)
{
    return m_impl->getNextWord(word);
}
