#include "provided.h"
//#include "support.h"
#include <string>
using namespace std;

class ExtractLinksImpl
{
public:
    ExtractLinksImpl(const string& pageContents);
    bool getNextLink(string& link);

private:
	string website;
	int startPos;		// starting position in the webpage when finding links
	std::string ToLower(const std::string& text)
	{ 
	std::string key ="";
	for(unsigned int k = 0; k < text.size(); k++)
	{	tolower(text[k]);
		key+=text[k];
	}
	return key;
	}
};

ExtractLinksImpl::ExtractLinksImpl(const string& pageContents)
	:website(pageContents), startPos(0)
{
}

bool ExtractLinksImpl::getNextLink(string& link)
{
	string website = ToLower(website);
	int bpos = website.find("<a href=",startPos);	// find beginning position of this 
	if( bpos == website.size()-1)
		return false;
			// cut out any before that position
	 int epos = website.find("\"", bpos); // find end position

	 link = website.substr(bpos+8 ,bpos-epos);		// return the URL
	 startPos = epos;
    return true;  // This is not always correct; it's just here to compile.
}

//******************** ExtractLink functions *******************************

// These functions simply delegate to ExtractLinksImpl's functions.
// You probably don't want to change any of this code.

ExtractLinks::ExtractLinks(const string& pageContents)
{
    m_impl = new ExtractLinksImpl(pageContents);
}

ExtractLinks::~ExtractLinks()
{
    delete m_impl;
}

bool ExtractLinks::getNextLink(string& link)
{
    return m_impl->getNextLink(link);
}