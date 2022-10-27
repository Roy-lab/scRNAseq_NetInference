#ifndef _GENE_
#define _GENE_
#include <map>
#include <string>
#include <vector>
using namespace std;
typedef map<int,double> DBL_MAP;
typedef map<int,double>::iterator DBL_MAP_ITER;
class Gene
{
	public:
	Gene();
	~Gene();
	int addExpVals(DBL_MAP*);
	int collapseReplicate(map<int,string>&);
	double getValue(int);
	string name;
	vector<DBL_MAP*> expValues;
	DBL_MAP value;
};
#endif
