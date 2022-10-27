#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <string.h>
#include "gsl/gsl_randist.h"
using namespace std;

map<string,map<int,double>*> geneExpression;

int readDataMatrix(const char*);
int generatePartition(int,const char*,int);
int generatePartition_Excl(int,const char*,int);
int readDataMatrixTranspose(const char*);
int generatePartitionTranspose(int,const char*,int);
int generatePartition_ExclTranspose(int,const char*,int);

int 
main(int argc, const char** argv)
{
	if(argc!=7)
	{
		cout <<"Usage: partitionData inputdata partitions outputdir partitionsize partitiontype[rand|exclusive] orientation[normal|transpose]" << endl;
		cout <<"\tExpression matrix could be in normal orientation (gene on row) or transposed (gene on column)" << endl;
		cout <<"\tExpression matrix should not have sample IDs (just genes and expression)" << endl;
		cout <<"\tOutput directory should already exist" << endl;
		return 0;
	}
	if(strcmp(argv[6],"normal")==0)
	{
		readDataMatrix(argv[1]);
	}
	else
	{
		readDataMatrixTranspose(argv[1]);
	}
	if(strcmp(argv[5],"rand")==0)
	{
		if(strcmp(argv[6],"normal")==0)
		{
			generatePartition(atoi(argv[2]),argv[3],atoi(argv[4]));
		}
		else
		{
			generatePartitionTranspose(atoi(argv[2]),argv[3],atoi(argv[4]));
		}
	}
	else
	{
		if(strcmp(argv[6],"normal")==0)
		{
			generatePartition_Excl(atoi(argv[2]),argv[3],atoi(argv[4]));
		}
		else
		{
			generatePartition_ExclTranspose(atoi(argv[2]),argv[3],atoi(argv[4]));
		}
	}
	return 0;
}

int
readDataMatrixTranspose(const char* aFName)
{
	ifstream inFile(aFName);
	char* buffer=NULL;
	int bufflen=0;
	string buffstr;
	int lineNum=0;
	map<int,string> id2gene;
	while(inFile.good())
	{
		getline(inFile,buffstr);
		if(buffstr.length()<=0)
		{
			continue;
		}
		if(bufflen<=buffstr.length())
		{
			bufflen=buffstr.length()+1;
			if(buffer!=NULL)
			{
				delete[] buffer;
			}
			buffer=new char[bufflen];
		}
		strcpy(buffer,buffstr.c_str());
		if (lineNum == 0) // Gene names
		{
			char* tok=strtok(buffer,"\t");
			int tokCnt=0;
			while(tok!=NULL)
			{
				string geneName(tok);
				id2gene[tokCnt] = geneName;
				map<int,double>* expMap=new map<int,double>;
				geneExpression[geneName]=expMap;
				tok=strtok(NULL,"\t");
				tokCnt++;
			}
		}
		else
		{
			char* tok=strtok(buffer,"\t");
			int tokCnt=0;
			map<int,double>* expMap=NULL;
			while(tok!=NULL)
			{
				string geneName = id2gene[tokCnt];
				double aVal=atof(tok);
				map<int,double>* expMap = geneExpression[geneName];
				(*expMap)[lineNum-1]=aVal;
				tok=strtok(NULL,"\t");
				tokCnt++;
			}
		}
		lineNum++;
	}
	inFile.close();
	return 0;
}

int
readDataMatrix(const char* aFName)
{
	ifstream inFile(aFName);
	char* buffer=NULL;
	int bufflen=0;
	string buffstr;
	while(inFile.good())
	{
		getline(inFile,buffstr);
		if(buffstr.length()<=0)
		{
			continue;
		}
		if(bufflen<=buffstr.length())
		{
			bufflen=buffstr.length()+1;
			if(buffer!=NULL)
			{
				delete[] buffer;
			}
			buffer=new char[bufflen];
		}
		strcpy(buffer,buffstr.c_str());
		char* tok=strtok(buffer,"\t");
		int tokCnt=0;
		map<int,double>* expMap=NULL;
		while(tok!=NULL)
		{
			if(tokCnt==0)
			{
				string geneName(tok);
				expMap=new map<int,double>;
				geneExpression[geneName]=expMap;
			}
			else
			{
				double aVal=atof(tok);
				(*expMap)[tokCnt-1]=aVal;
			}
			tok=strtok(NULL,"\t");
			tokCnt++;
		}
	}
	inFile.close();
	return 0;
}

int
generatePartitionTranspose(int partitions, const char* outputDir,int subsetsize)
{
	gsl_rng* r=gsl_rng_alloc(gsl_rng_default);
	int randseed=getpid();
	gsl_rng_set(r,randseed);
	int size=geneExpression.begin()->second->size();
	double step=1.0/(double)size;
	char aFName[1024];
	int maxind=0;
	int minind=1000;
	for(int p=0;p<partitions;p++)
	{
		sprintf(aFName,"%s/dataindices%d.txt",outputDir,p);
		ofstream oFile(aFName);
		sprintf(aFName,"%s/dataset%d.txt",outputDir,p);
		ofstream dFile(aFName);
		map<int,int> usedInit;
		for(int i=0;i<subsetsize;i++)
		{
			double rVal=gsl_ran_flat(r,0,1);
			int rind=(int)(rVal/step);
			while(usedInit.find(rind)!=usedInit.end())
			{
				rVal=gsl_ran_flat(r,0,1);
				rind=(int)(rVal/step);
			}
			usedInit[rind]=0;
			if(rind<minind)
			{
				minind=rind;
			}
			if(rind>maxind)
			{
				maxind=rind;
			}
		}
		int colCnt=0;
		for(map<string,map<int,double>*>::iterator gIter=geneExpression.begin();gIter!=geneExpression.end();gIter++)
		{
			if (colCnt>0)
			{
				dFile << "\t";
			}
			dFile << gIter->first;
			colCnt++;
		}
		dFile << endl;
		for(map<int,int>::iterator aIter=usedInit.begin();aIter!=usedInit.end();aIter++)
		{
			colCnt=0;
			for(map<string,map<int,double>*>::iterator gIter=geneExpression.begin();gIter!=geneExpression.end();gIter++)
			{
				if (colCnt>0)
				{
					dFile << "\t";
				}
				map<int,double>* expMap=gIter->second;
				double aVal=(*expMap)[aIter->first];
				dFile << aVal;
				colCnt++;
			}
			dFile << endl;
		}
		for(map<int,int>::iterator rIter=usedInit.begin();rIter!=usedInit.end();rIter++)
		{
			oFile << rIter->first << endl;
		}
		oFile.close();
		dFile.close();
		usedInit.clear();
	}
	cout <<"Maxind " << maxind << " Minind " << minind << endl;
	return 0;
}

int
generatePartition(int partitions, const char* outputDir,int subsetsize)
{
	gsl_rng* r=gsl_rng_alloc(gsl_rng_default);
	int randseed=getpid();
	gsl_rng_set(r,randseed);
	int size=geneExpression.begin()->second->size();
	double step=1.0/(double)size;
	char aFName[1024];
	int maxind=0;
	int minind=1000;
	for(int p=0;p<partitions;p++)
	{
		sprintf(aFName,"%s/dataindices%d.txt",outputDir,p);
		ofstream oFile(aFName);
		sprintf(aFName,"%s/dataset%d.txt",outputDir,p);
		ofstream dFile(aFName);
		map<int,int> usedInit;
		for(int i=0;i<subsetsize;i++)
		{
			double rVal=gsl_ran_flat(r,0,1);
			int rind=(int)(rVal/step);
			while(usedInit.find(rind)!=usedInit.end())
			{
				rVal=gsl_ran_flat(r,0,1);
				rind=(int)(rVal/step);
			}
			usedInit[rind]=0;
			if(rind<minind)
			{
				minind=rind;
			}
			if(rind>maxind)
			{
				maxind=rind;
			}
		}
		for(map<string,map<int,double>*>::iterator gIter=geneExpression.begin();gIter!=geneExpression.end();gIter++)
		{
			dFile << gIter->first;
			map<int,double>* expMap=gIter->second;
			for(map<int,int>::iterator aIter=usedInit.begin();aIter!=usedInit.end();aIter++)
			{
				double aVal=(*expMap)[aIter->first];
				dFile <<"\t" << aVal;
			}
			dFile << endl;
		}
		for(map<int,int>::iterator rIter=usedInit.begin();rIter!=usedInit.end();rIter++)
		{
			oFile << rIter->first << endl;
		}
		oFile.close();
		dFile.close();
		usedInit.clear();
	}
	cout <<"Maxind " << maxind << " Minind " << minind << endl;
	return 0;
}


int
generatePartition_Excl(int partitions, const char* outputDir,int subsetsize)
{
	int size=geneExpression.begin()->second->size();
	int testsetsize=size/partitions;
	char aFName[1024];
	int maxind=0;
	int minind=1000;
	for(int p=0;p<partitions;p++)
	{
		sprintf(aFName,"%s/dataindices%d.txt",outputDir,p);
		ofstream oFile(aFName);
		sprintf(aFName,"%s/dataset%d.txt",outputDir,p);
		ofstream dFile(aFName);
		map<int,int> usedInit;
		int testSetStart=p*testsetsize;
		int testSetEnd=(p+1)*testsetsize;
		if(p==partitions-1)
		{
			testSetEnd=size;
		}
		for(int i=0;i<size;i++)
		{
			if(i>=testSetStart && i<testSetEnd)
			{
				continue;
			}
			usedInit[i];
		}
		for(map<string,map<int,double>*>::iterator gIter=geneExpression.begin();gIter!=geneExpression.end();gIter++)
		{
			dFile << gIter->first;
			map<int,double>* expMap=gIter->second;
			for(map<int,int>::iterator aIter=usedInit.begin();aIter!=usedInit.end();aIter++)
			{
				double aVal=(*expMap)[aIter->first];
				dFile <<"\t" << aVal;
			}
			dFile << endl;
		}
		for(map<int,int>::iterator rIter=usedInit.begin();rIter!=usedInit.end();rIter++)
		{
			oFile << rIter->first << endl;
		}
		oFile.close();
		dFile.close();
		usedInit.clear();
	}
	cout <<"Maxind " << maxind << " Minind " << minind << endl;
	return 0;
}

int
generatePartition_ExclTranspose(int partitions, const char* outputDir,int subsetsize)
{
	int size=geneExpression.begin()->second->size();
	int testsetsize=size/partitions;
	char aFName[1024];
	int maxind=0;
	int minind=1000;
	for(int p=0;p<partitions;p++)
	{
		sprintf(aFName,"%s/dataindices%d.txt",outputDir,p);
		ofstream oFile(aFName);
		sprintf(aFName,"%s/dataset%d.txt",outputDir,p);
		ofstream dFile(aFName);
		map<int,int> usedInit;
		int testSetStart=p*testsetsize;
		int testSetEnd=(p+1)*testsetsize;
		if(p==partitions-1)
		{
			testSetEnd=size;
		}
		for(int i=0;i<size;i++)
		{
			if(i>=testSetStart && i<testSetEnd)
			{
				continue;
			}
			usedInit[i];
		}
		int colCnt=0;
		for(map<string,map<int,double>*>::iterator gIter=geneExpression.begin();gIter!=geneExpression.end();gIter++)
		{
			if (colCnt>0)
			{
				dFile << "\t";
			}
			dFile << gIter->first;
			colCnt++;
		}
		dFile << endl;
		for(map<int,int>::iterator aIter=usedInit.begin();aIter!=usedInit.end();aIter++)
		{
			colCnt=0;
			for(map<string,map<int,double>*>::iterator gIter=geneExpression.begin();gIter!=geneExpression.end();gIter++)
			{
				if (colCnt>0)
				{
					dFile << "\t";
				}
				map<int,double>* expMap=gIter->second;
				double aVal=(*expMap)[aIter->first];
				dFile << aVal;
				colCnt++;
			}
			dFile << endl;
		}
		for(map<int,int>::iterator rIter=usedInit.begin();rIter!=usedInit.end();rIter++)
		{
			oFile << rIter->first << endl;
		}
		oFile.close();
		dFile.close();
		usedInit.clear();
	}
	cout <<"Maxind " << maxind << " Minind " << minind << endl;
	return 0;
}

