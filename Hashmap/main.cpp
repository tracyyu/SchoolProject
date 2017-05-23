#include "http.h"
#include "provided.h" 
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[] )
{
	using namespace std;

	if( argc < 5)
	{	
		cout << "Usage: proj4.exe rulesFile seedSiteFile maxPages minPrice";
		return 0;
	}
	else
	{
		std::string rule((LPCTSTR)*argv[1]);
		ifstream ruleinfile(rule.c_str());
	    if ( ! ruleinfile )
	    {
	        cout << "Error: Cannot open advertisement rules file!" << endl;
	        return 1;
	    }
		std::string seed((LPCTSTR)*argv[2]);
		ifstream seedinfile(seed.c_str());
		if( ! seedinfile)
	    {
	        cout << "Error: Cannot open seed sites file!" << endl;
	        return 1;
	    }
		AdHunter file(ruleinfile);
		string line;
		while(getline(seedinfile, line))
		{file.addSeedSite(line);}
		cout << "Crawling..."<<endl;
		cout << endl; //create an empty line
		vector<Match>matches;
		int page = atoi(*argv[3]);
		cout.setf(ios::fixed);
		cout.setf(ios::showpoint);
		cout.precision(2);
		int price = atof(*argv[4]);
		int match_count = file.getBestAdTargets(price,page, matches);

		cout << "There were " << match_count << " matches found that met the minimum price of $" << price << " per ad:" <<endl;
		cout << endl; //follow by an empty line
		for( unsigned int k = 0; k < matches.size(); k++)
		{
			cout << "Match " << k << ": " << matches[k].ruleName << " triggered on " << matches[k].url << " for a value of $" << matches[k].dollarValue <<endl;
			cout << "\tAdvertisement: " << matches[k].ad << endl;
		}
	}
}