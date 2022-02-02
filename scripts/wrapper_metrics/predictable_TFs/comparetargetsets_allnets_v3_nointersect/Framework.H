#ifndef _FRAMEWORK_
#define _FRAMEWORK_
#include <map>
#include <string>
using namespace std;

class Graph;
class GeneNameMapper;
class Framework
{
	public:
		Framework();
		~Framework();
		int readNames(const char*);
		int readNetworks(const char*);
		int setFDR(double);
		int compareTFsAllNw();
		int showComparisons(const char*);
		int showComparisonsAll(const char*);
		typedef struct
		{
			double fscore;
			double pval;
			double foldenr;
		}TFStat;
	private:
		int readUniverse(const char*, map<string,int>*u);
		int makeUniverse();
		map<string,map<string,int>* > universesets;
		map<string,int> universe;
		int sortFactors(vector<string>&);
		map<string,int> datasetKeys;
		map<string,map<string,TFStat*>*> tfSimSet;
		map<string,Graph*> datasets;
		map<string,int> datasetGroup;
		map<int,string> datasetgroupIDs;
		map<string,map<int,int>*> factorGroup;
		map<string,map<int,map<int,int>*>*> factorGroupEnr;
		map<string,int> datasetPair_TotalCommonTFs;
		GeneNameMapper gnm;
		double fdr;

		map<string,int> goldNames;
};
#endif
