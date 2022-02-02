#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "Framework.H"

int sortfunc(const void* first, const void* second);
int* sortingind=NULL;
double* sortededge=NULL;
  
Framework::Framework()
{
}

Framework::~Framework()
{
}

int 
Framework::readTrueNet(const char* nwFName)
{
	ifstream inFile(nwFName);
	char buffer[1024];
	while(inFile.good())
	{
		inFile.getline(buffer,1023);
		if(strlen(buffer)<=0)
		{
			continue;
		}
		char* tok=strtok(buffer,"\t");
		int tokCnt=0;
		string tfname;
		string tgtname;
		while(tok!=NULL)
		{
			if(tokCnt==0)
			{
				tfname.append(tok);	
			}
			else if(tokCnt==1)
			{
				tgtname.append(tok);
			}
			tok=strtok(NULL,"\t");
			tokCnt++;
		}
		tflist[tfname]=0;
		tgtlist[tgtname]=0;
		string key(tfname);
		key.append("\t");
		key.append(tgtname);
		truenet[key]=0;
	}
	inFile.close();
	return 0;
}

int 
Framework::readPredNet(const char* aFName,int tfpos, int tgtpos)
{
	ifstream inFile(aFName);
	char buffer[1024];
	while(inFile.good())
	{
		inFile.getline(buffer,1023);
		if(strlen(buffer)<=0)
		{
			continue;
		}
		char* tok=strtok(buffer,"\t");
		int tokCnt=0;
		string tfname;
		string tgtname;
		double strength=0;
		while(tok!=NULL)
		{
			if(tokCnt==tgtpos)
			{
				tgtname.append(tok);	
			}
			else if(tokCnt==tfpos)
			{
				tfname.append(tok);
			}
			else if(tokCnt==2)
			{
				//strength=fabs(atof(tok));
				strength=atof(tok);
			}
			tok=strtok(NULL,"\t");
			tokCnt++;
		}
		if((tflist.find(tfname)==tflist.end()) ||  (tgtlist.find(tgtname)==tgtlist.end()))
		{
			continue;
		}
		string key(tfname);
		key.append("\t");
		key.append(tgtname);
		if(prednet.find(key)==prednet.end())
		{
			prednet[key]=strength;
		}
		else
		{
			double currstrength=prednet[key];
			if(currstrength<strength)
			{
				prednet[key]=strength;
			}
		}
	}
	cout <<"Found " << prednet.size() << " edges " << endl;
	inFile.close();
	return 0;
}

	
int 
Framework::estimatePR(const char* aFName)
{
	vector<string> sortedEdges;
	sortEdges(sortedEdges);
	ofstream oFile(aFName);
	int tpcnt=0;
	int totaltruecnt=truenet.size();
	int totalpredcnt=0;
	int totalpossible=tflist.size()*tgtlist.size();
	for(int i=0;i<sortedEdges.size();i++)
	//for(int i=0;i<totalpossible;i++)
	{
		if(i<sortedEdges.size())
		{
			if(truenet.find(sortedEdges[i])!=truenet.end())
			{
				tpcnt++;
			}
		}	
		double fracconsidered=(double)(i+1)/((double)(totalpossible));
		totalpredcnt++;
		double prec=((double)tpcnt)/((double)totalpredcnt);
		double recall=((double)tpcnt)/((double)totaltruecnt);
		double considered=((double)totalpredcnt)/((double)totalpossible);
		double rand=fracconsidered*totaltruecnt;
		//oFile <<prec<<"\t" << recall << endl;
		if (i < sortedEdges.size()-1 && prednet[sortedEdges[i]]==prednet[sortedEdges[i+1]])
		{
			continue;
		}
		oFile <<recall<<"\t" << prec<< endl;
		//oFile <<prec<<"\t" << recall << "\t" << considered<< endl;
		//oFile <<considered<< "\t" << tpcnt<< "\t" << rand<< "\t" << recall << "\t" << prec <<  endl;

	}
	oFile.close();
	return 0;
}
/*
int
Framework::sortEdges(vector<string>& edgeSet)
{
	for(STRDBLMAP_ITER eIter=prednet.begin();eIter!=prednet.end();eIter++)
	{
		edgeSet.push_back(eIter->first);
	}
	for(int i=0;i<edgeSet.size();i++)
	{
		for(int j=i+1;j<edgeSet.size();j++)
		{
			double e1=prednet[edgeSet[i]];
			double e2=prednet[edgeSet[j]];
			if(e1<e2)
			{
				string temp(edgeSet[i]);
				edgeSet[i].clear();
				edgeSet[i].append(edgeSet[j]);
				edgeSet[j].clear();
				edgeSet[j].append(temp.c_str());
			}
		}
	}
	return 0;
}*/


int
Framework::sortEdges(vector<string>& edgeSet)
{
	sortingind=new int[prednet.size()];
	sortededge=new double[prednet.size()];
	int ind=0;
	map<int,string> edgeIDNameMap;
	for(STRDBLMAP_ITER eIter=prednet.begin();eIter!=prednet.end();eIter++)
	{
		sortingind[ind]=ind;
		sortededge[ind]=eIter->second;
		edgeIDNameMap[ind]=eIter->first;
		ind++;
	}
	qsort(sortingind,prednet.size(),sizeof(int),&sortfunc);
	for(int i=0;i<prednet.size();i++)
	{
		int sid=sortingind[i];
		//cout << sortededge[sid]<< endl;
		edgeSet.push_back(edgeIDNameMap[sid]);
	}
	return 0;
}

int 
sortfunc(const void* first, const void* second)
{
	int ind1=*((int*)first);	
	int ind2=*((int*)second);
	double pval1=sortededge[ind1];
	double pval2=sortededge[ind2];
	int compstat=0;
	if(pval1>pval2)
	{
		compstat=-1;
	}
	else if(pval1<pval2)
	{
		compstat=1;
	}
	return compstat;
}

int
main(int argc, const char** argv)
{
	if(argc!=6)
	{
		cout <<"getPR truenet prednet tfpos tgtpos prout" << endl;
		return 0;
	}
	Framework fw;
	fw.readTrueNet(argv[1]);
	fw.readPredNet(argv[2],atoi(argv[3]),atoi(argv[4]));
	fw.estimatePR(argv[5]);
	return 0;
}
