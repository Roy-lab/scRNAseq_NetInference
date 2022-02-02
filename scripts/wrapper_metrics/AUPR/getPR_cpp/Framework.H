#ifndef _FRAMEWORK_
#define _FRAMEWORK_

#include <map>
#include <string>
#include <vector>

using namespace std;
typedef map<string,double> STRDBLMAP;
typedef map<string,double>::iterator STRDBLMAP_ITER;

class Framework
{
	public:
	Framework();
	~Framework();
	int readTrueNet(const char*);
	int readPredNet(const char*,int,int);
	int estimatePR(const char*);
	private:
	int sortEdges(vector<string>&);
	STRDBLMAP truenet;
	STRDBLMAP prednet;
	map<string,int> tflist;
	map<string,int> tgtlist;
};
#endif
