#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "Vertex.H"
#include "GeneNameMapper.H"
#include "Graph.H"
#include "HyperGeomPval.H"
#include "Framework.H"
int sortfunc(const void* first, const void* second);
int* sortingind=NULL;
double* sortedpvals=NULL;

Framework::Framework()
{
	gnm.readGeneNames();
}

Framework::~Framework()
{
}

int Framework::readNames(const char* fName)
{
	ifstream inFile(fName);
	char buffer[1024];
	while(inFile.good())
	{
		inFile.getline(buffer,1023);
		if(strlen(buffer)==0)
		{
			continue;
		}
		char* tok=strtok(buffer,"\t");
		string name(tok);
		goldNames[name]=0;
	}
	inFile.close();
	return 0;
}

int Framework::readUniverse(const char* fName, map<string,int>*u)
{
	ifstream inFile(fName);
	char buffer[1024];
	while(inFile.good())
	{
		inFile.getline(buffer,1023);
		if(strlen(buffer)==0)
		{
			continue;
		}
		char* tok=strtok(buffer,"\t");
		int tokCnt=0;
		while(tok!=NULL)
		{
			if(tokCnt==1)
			{
				string uKey(tok);
				(*u)[uKey] = 0;
			}
			tokCnt++;
			tok=strtok(NULL,"\t");
		}
	}
	inFile.close();
	return 0;
}

int Framework::makeUniverse()
{
	universe.clear();
	map<string,int> goldtargets;
	map<string,int> nongoldtargets;
	for (map<string,map<string,int>* >::iterator itr=universesets.begin();itr!=universesets.end();itr++)
	{
		string netname=itr->first;
		if (goldNames.find(netname) == goldNames.end())
		{
			continue;
		}
		map<string,int>* firstu = itr->second;
		for(map<string,int>::iterator iitr=firstu->begin();iitr!=firstu->end();iitr++)
		{
			string name = iitr->first;
			goldtargets[name]=0;
		}
	}
	for (map<string,map<string,int>* >::iterator itr=universesets.begin();itr!=universesets.end();itr++)
	{
		string netname=itr->first;
		if (goldNames.find(netname) != goldNames.end())
		{
			continue;
		}
		map<string,int>* firstu = itr->second;
		for(map<string,int>::iterator iitr=firstu->begin();iitr!=firstu->end();iitr++)
		{
			string name = iitr->first;
			nongoldtargets[name]=0;
		}
	}
	//no intersection
	for(map<string,int>::iterator iitr=goldtargets.begin();iitr!=goldtargets.end();iitr++)
	{
		string name = iitr->first;
		universe[name]=0;
	}
	for(map<string,int>::iterator iitr=nongoldtargets.begin();iitr!=nongoldtargets.end();iitr++)
	{
		string name = iitr->first;
		universe[name]=0;
	}
	cout << "Target set of gold standards is: " << goldtargets.size() << endl;
	cout << "Target set of non gold standards is: " << nongoldtargets.size() << endl;
	cout << "The union: " << universe.size() << endl;
	return 0;
}

int 
Framework::readNetworks(const char* aFName)
{
	ifstream inFile(aFName);
	char buffer[1024];
	while(inFile.good())
	{
		string datasetName;
		string fileName;
		inFile.getline(buffer,1023);
		if(strlen(buffer)<=0)
		{
			continue;
		} 
		if(strstr(buffer,"#")!=NULL)
		{
			continue;
		}
		char* tok=strtok(buffer,"\t");
		int tokCnt=0;
		int datasetID;
		while(tok!=NULL)
		{
			if(tokCnt==0)
			{
				datasetName.append(tok);
			}
			else if(tokCnt==1)
			{
				fileName.append(tok);
			}
			else if(tokCnt==2)
			{
				datasetID=atoi(tok);
			}
			tok=strtok(NULL,"\t");
			tokCnt++;
		}
		Graph* g=new Graph;
		//g->reverseEdge();
		g->makeGraph(fileName.c_str());	
		map<string,int>* uu = new map<string,int>;
		readUniverse(fileName.c_str(),uu);
		universesets[datasetName]=uu;
		datasets[datasetName]=g;
		datasetGroup[datasetName]=datasetID;
		char key[1024];
		strcpy(key,datasetName.c_str());
		char* pos=strchr(key,'_');
		if(pos!=NULL)
		{
			*pos='\0';
		}
		string newkey(key);
		datasetgroupIDs[datasetID]=newkey;
	}
	makeUniverse();
	inFile.close();
	return 0;
}

int
Framework::setFDR(double f)
{
	fdr=f;
	return 0;
}

int 
Framework::compareTFsAllNw()
{
	HyperGeomPval hgp;
	for(map<string,Graph*>::iterator gIter=datasets.begin();gIter!=datasets.end();gIter++)
	{
		if (goldNames.find(gIter->first) == goldNames.end())
		{
			continue;
		}
		Graph* g1=gIter->second;
		int gid1=datasetGroup[gIter->first];
		map<string,Vertex*>& vset1=g1->getVertexSet();
		for(map<string,Graph*>::iterator hIter=datasets.begin();hIter!=datasets.end();hIter++)
		{
			if (goldNames.find(hIter->first) != goldNames.end())
			{
				continue;
			}
			Graph* g2=hIter->second;
			int gid2=datasetGroup[hIter->first];
			map<string,Vertex*>& vset2=g2->getVertexSet();
			map<string,int> totalVertex;
			for(map<string,Vertex*>::iterator vIter=vset1.begin();vIter!=vset1.end();vIter++)
			{
				if(vIter->second->getInNeighbours().size()==0)
				{
					continue;
				}
				if(vset2.find(vIter->first)==vset2.end())
				{
					continue;
				}
				if(vset2[vIter->first]->getInNeighbours().size()==0)
				{
					continue;
				}
				totalVertex[vIter->first]=0;
			}	
			for(map<string,Vertex*>::iterator vIter=vset2.begin();vIter!=vset2.end();vIter++)
			{
				if(vIter->second->getInNeighbours().size()==0)
				{
					continue;
				}
				//totalVertex[vIter->first]=0;
			}
			string datasetKey(gIter->first);
			datasetKey.append("-");
			datasetKey.append(hIter->first);
			datasetKeys[datasetKey]=0;
			//int total=totalVertex.size();
			int total=universe.size();
			vector<double> factorPvals;
			map<int,string> pvalsFactorMap;
			for(map<string,Vertex*>::iterator pIter=vset1.begin();pIter!=vset1.end();pIter++)
			{
				Vertex* v=pIter->second;
				if(v->getImmediateNeighbours().size()==0)
				{
					continue;
				}
				if(vset2.find(pIter->first)==vset2.end())
				{
					//	cout <<"No gene " << pIter->first << " in true network" << endl;
					continue;
				}
				if(strcmp(pIter->first.c_str(),"SRF")==0)
				{
					cout <<"Stop here" << endl;
				}
				map<string,Vertex*>& predNeighbours=v->getImmediateNeighbours();
				map<string,Vertex*>& trueNeighbours=vset2[pIter->first]->getImmediateNeighbours();
				if(trueNeighbours.size()==0)
				{
					continue;
				}
				double match=0;
				//double totalpred=((double) predNeighbours.size());
				//double totalactual=((double) trueNeighbours.size());
				double totalpred=0;//((double) predNeighbours.size());
				double totalactual=0;//((double) trueNeighbours.size());
				for(map<string,Vertex*>::iterator nIter=predNeighbours.begin();nIter!=predNeighbours.end();nIter++)
				{
					if(universe.find(nIter->first)==universe.end())
						continue;
					totalpred++;
				}
				for(map<string,Vertex*>::iterator nIter=trueNeighbours.begin();nIter!=trueNeighbours.end();nIter++)
				{
					if(universe.find(nIter->first)==universe.end())
						continue;
					totalactual++;
				}
				for(map<string,Vertex*>::iterator nIter=predNeighbours.begin();nIter!=predNeighbours.end();nIter++)
				{
					if(trueNeighbours.find(nIter->first)==trueNeighbours.end())
					{
						continue;
					}
					match++;
				}
				double fscore=0;
				double prec=0;
				double recall=0;
				double pval=1;
				double foldenr=0;
				if(match>0)
				{
					double randprec=totalactual/total;
					double randrecall=totalpred/total;
					prec=match/totalpred;
					recall=match/totalactual;
					fscore=(2*prec*recall)/(prec+recall);
					pval=hgp.getOverRepPval(totalpred,match,totalactual,total-totalactual);
					if(prec<=randprec || recall<=randrecall)
					{
						pval=1;
					}
					foldenr=(match/totalpred)/(totalactual/total);
					if(pval>1e-4)
					{
	//					cout <<"Prec " << prec << " Recall " << recall << endl;
					}
				}
				pvalsFactorMap[factorPvals.size()]=pIter->first;
				factorPvals.push_back(pval);
				Framework::TFStat* tf=new Framework::TFStat;
				tf->fscore=fscore;
				tf->pval=pval;
				tf->foldenr=foldenr;
				map<string,Framework::TFStat*>* tfsim=NULL;
				if(tfSimSet.find(pIter->first)==tfSimSet.end())
				{
					tfsim=new map<string,Framework::TFStat*>;
					tfSimSet[pIter->first]=tfsim;
				}
				else
				{
					tfsim=tfSimSet[pIter->first];
				}
				(*tfsim)[datasetKey]=tf;
				map<int,int>* dIds=NULL;
				if(factorGroup.find(pIter->first)==factorGroup.end())
				{
					dIds=new map<int,int>;
					factorGroup[pIter->first]=dIds;
				}
				else
				{
					dIds=factorGroup[pIter->first];
				}
				(*dIds)[gid1]=0;
				(*dIds)[gid2]=0;
				if(datasetPair_TotalCommonTFs.find(datasetKey)==datasetPair_TotalCommonTFs.end())
				{
					datasetPair_TotalCommonTFs[datasetKey]=1;
				}	
				else
				{
					datasetPair_TotalCommonTFs[datasetKey]=datasetPair_TotalCommonTFs[datasetKey]+1;
				}
			}
			if (factorPvals.size() == 0)
			{
				continue;
			}
			sortedpvals=new double[factorPvals.size()];
			sortingind=new int[factorPvals.size()];
			for(int t=0;t<factorPvals.size();t++)
			{
				sortedpvals[t]=factorPvals[t];
				sortingind[t]=t;
			}
			qsort(sortingind,factorPvals.size(),sizeof(int),&sortfunc);
			vector<double> corrpvals(factorPvals.size());
			//using Pouya's code
			double rx=1/((double)(factorPvals.size()));
			double m=(double)factorPvals.size();
			int maxk=-1;
			for(int k=0;k<factorPvals.size();k++)
			{
				int sid=sortingind[k];
				double cpval=(factorPvals[sid]*m)/((double)(k+1));
				corrpvals[sid]=cpval;
				//cout <<" Pvals " << pvals[sid] << " CorrPval " << cpval << endl;
				if(corrpvals[sid]<=fdr)
				{
					maxk=k;
				}
			}
			//The AFDR is the approximate FDR or the q-value? It is defined as the minimum fdr at which I
			//would call a test significant
			double minfdr=corrpvals[sortingind[corrpvals.size()-1]];
			for(int l=corrpvals.size()-1;l>=0;l--)
			{
				int sid=sortingind[l];
				if(corrpvals[sid]>minfdr)
				{
					corrpvals[sid]=minfdr;
				}
				else 
				{
					minfdr=corrpvals[sid];
				}
			}
			//Now dump the maxk hypothesis
			//for(int k=0;k<=maxk;k++)
			for(int k=0;k<corrpvals.size();k++)
			{
				int sid=sortingind[k];
				double oldpval=factorPvals[sid];
				double corrpval=corrpvals[sid];
				string& factor=pvalsFactorMap[sid];
				if(strcmp(factor.c_str(),"Nfkbiz")==0)
				{
					cout << "Stop here for net" << gIter->first <<" " << hIter->first << " "<< corrpval << " oldpval " << oldpval << endl;
				}
				if(gid1==gid2)
				{
					continue;
				}
				map<string,Framework::TFStat*>* tfsim=tfSimSet[factor];
				Framework::TFStat* tf=(*tfsim)[datasetKey];
				tf->pval=corrpval;
				if(k>maxk)
				{
					continue;
				}
				if((strcmp(datasetKey.c_str(),"C-E1")==0) || (strcmp(datasetKey.c_str(),"E1-M")==0))
				{
					//cout << datasetKey << "\t" << factor << endl;
				}
				map<int,map<int,int>*>* dIdsEnr=NULL;
				if(factorGroupEnr.find(factor)==factorGroupEnr.end())
				{
					dIdsEnr=new map<int,map<int,int>*>;
					factorGroupEnr[factor]=dIdsEnr;
				}
				else
				{
					dIdsEnr=factorGroupEnr[factor];
				}
				map<int,int>* otherids=NULL;
				if(dIdsEnr->find(gid1)==dIdsEnr->end())
				{
					otherids=new map<int,int>;
					(*dIdsEnr)[gid1]=otherids;
				}
				else
				{
					otherids=(*dIdsEnr)[gid1];
				}
				(*otherids)[gid2]=0;
				otherids=NULL;
				if(dIdsEnr->find(gid2)==dIdsEnr->end())
				{
					otherids=new map<int,int>;
					(*dIdsEnr)[gid2]=otherids;
				}
				else
				{
					otherids=(*dIdsEnr)[gid2];
				}
				(*otherids)[gid1]=0;
			}
			corrpvals.clear();
			factorPvals.clear();
			pvalsFactorMap.clear();
			delete[] sortingind;
			delete[] sortedpvals;
		}
		
	}
	return 0;
}

int
Framework::showComparisonsAll(const char* outSuff)
{
	char outFName[1024];
	sprintf(outFName,"%s_factor_coenr_all.txt",outSuff);
	ofstream oFile(outFName);
	oFile <<"TF";
	for(map<string,int>::iterator dIter=datasetKeys.begin();dIter!=datasetKeys.end();dIter++)
	{
		oFile <<"\t" << dIter->first;
	}
	oFile <<endl;
	map<string,map<string,int>*> datasetpair_sigTFoverlap;
	for(map<string,map<string,Framework::TFStat*>*>::iterator tIter=tfSimSet.begin();tIter!=tfSimSet.end();tIter++)
	{
		string& tfname=(string&)tIter->first;
		if(factorGroup.find(tfname)==factorGroup.end())
		{
			continue;
		}
		map<int,int>* datasetOn=factorGroup[tfname];
		//if(factorGroupEnr.find(tfname)==factorGroupEnr.end())
		//{
		//	continue;
		//}
		//Consider on factors assayed
		int hit=0;
		map<string,Framework::TFStat*>* tfset=tIter->second;
		for(map<string,int>::iterator dIter=datasetKeys.begin();dIter!=datasetKeys.end();dIter++)
		{
			if(tfset->find(dIter->first)==tfset->end())
			{
				continue;
			}
			Framework::TFStat* tf=(*tfset)[dIter->first];
			if(tf->foldenr>1 && tf->pval<fdr)
			{
				hit=hit+1;
			}
		}
		//Need to be enriched in at least more than one dataset. so if in Venters, it should be enriched
		//in hu's as well
		//if(hit<=2)
		oFile <<gnm.getCommonName(tIter->first.c_str());
		for(map<string,int>::iterator dIter=datasetKeys.begin();dIter!=datasetKeys.end();dIter++)
		{
			double foldenr=-1;
			double pval=1;
			if(tfset->find(dIter->first)!=tfset->end())
			{
				foldenr=(*tfset)[dIter->first]->foldenr;
				pval=(*tfset)[dIter->first]->pval;
			}
			oFile <<"\t" << foldenr;
		}
		oFile << endl;
	}
	oFile.close();
	return 0;
}

int
Framework::showComparisons(const char* outSuff)
{
	char outFName[1024];
	sprintf(outFName,"%s_factor_coenr.txt",outSuff);
	ofstream oFile(outFName);
	sprintf(outFName,"%s_perdatasetpair.txt",outSuff);
	ofstream dFile(outFName);
	oFile <<"TF";
	for(map<string,int>::iterator dIter=datasetKeys.begin();dIter!=datasetKeys.end();dIter++)
	{
		oFile <<"\t" << dIter->first;
	}
	oFile <<endl;
	map<string,map<string,int>*> datasetpair_sigTFoverlap;
	for(map<string,map<string,Framework::TFStat*>*>::iterator tIter=tfSimSet.begin();tIter!=tfSimSet.end();tIter++)
	{
		string& tfname=(string&)tIter->first;
		if(factorGroup.find(tfname)==factorGroup.end())
		{
			continue;
		}
		map<int,int>* datasetOn=factorGroup[tfname];
		if(factorGroupEnr.find(tfname)==factorGroupEnr.end())
		{
			continue;
		}
		//Consider on factors assayed
		int hit=0;
		map<string,Framework::TFStat*>* tfset=tIter->second;
		for(map<string,int>::iterator dIter=datasetKeys.begin();dIter!=datasetKeys.end();dIter++)
		{
			if(tfset->find(dIter->first)==tfset->end())
			{
				continue;
			}
			Framework::TFStat* tf=(*tfset)[dIter->first];
			if(tf->foldenr>1 && tf->pval<fdr)
			{
				hit=hit+1;
			}
		}
		//Need to be enriched in at least more than one dataset. so if in Venters, it should be enriched
		//in hu's as well
		//if(hit<=2)
		oFile <<gnm.getCommonName(tIter->first.c_str());
		for(map<string,int>::iterator dIter=datasetKeys.begin();dIter!=datasetKeys.end();dIter++)
		{
			double foldenr=-1;
			double pval=1;
			if(tfset->find(dIter->first)!=tfset->end())
			{
				foldenr=(*tfset)[dIter->first]->foldenr;
				pval=(*tfset)[dIter->first]->pval;
				if((strcmp(dIter->first.c_str(),"C-E1")==0) || (strcmp(dIter->first.c_str(),"E1-M")==0))
				{
					//cout << dIter->first << "\t" << tIter->first << endl;
				}
				if(foldenr>1 && pval<fdr)
				{
					map<string,int>* factorsEnrForPair=NULL;
					if(datasetpair_sigTFoverlap.find(dIter->first)==datasetpair_sigTFoverlap.end())
					{
						factorsEnrForPair=new map<string,int>;
						datasetpair_sigTFoverlap[dIter->first]=factorsEnrForPair;
					}
					else
					{
						factorsEnrForPair=datasetpair_sigTFoverlap[dIter->first];
					}
					(*factorsEnrForPair)[gnm.getCommonName(tIter->first.c_str())]=0;
				}
			}
			oFile <<"\t" << foldenr;
		}
		oFile << endl;
	}
	oFile.close();
	//Dataset Map
	for(map<string,map<string,int>*>::iterator dIter=datasetpair_sigTFoverlap.begin();dIter!=datasetpair_sigTFoverlap.end();dIter++)
	{
		map<string,int>* tfset=dIter->second;
		int commonTFs=0;
		if(datasetPair_TotalCommonTFs.find(dIter->first)!=datasetPair_TotalCommonTFs.end())
		{
			commonTFs=datasetPair_TotalCommonTFs[dIter->first];
		}
		dFile << dIter->first << "\t" << dIter->second->size() << "\t" << commonTFs;
		for(map<string,int>::iterator tIter=tfset->begin();tIter!=tfset->end();tIter++)
		{
			dFile << "\t" << tIter->first;
		}
		dFile << endl;
	}
	sprintf(outFName,"%s_pertf_stats.txt",outSuff);
	ofstream tFile1(outFName);
	sprintf(outFName,"%s_pertf_datasetpresence.txt",outSuff);
	ofstream tFile2(outFName);
	tFile2 <<"TF";
	for(map<int,string>::iterator aIter=datasetgroupIDs.begin();aIter!=datasetgroupIDs.end();aIter++)
	{
		tFile2 <<"\t" << aIter->second;
	}
	tFile2 << endl;
	//for(map<string,map<int,map<int,int>*>*>::iterator fIter=factorGroupEnr.begin();fIter!=factorGroupEnr.end();fIter++)
	for(map<string,map<int,int>*>::iterator fIter=factorGroup.begin();fIter!=factorGroup.end();fIter++)
	{
		if(fIter->second->size()==1)
		{
	//		continue;
		}
		int total=fIter->second->size();
		tFile2 << gnm.getCommonName(fIter->first.c_str());
		map<int,map<int,int>*>* enrData=NULL;
		int enrichedin=0;
		if(factorGroupEnr.find(fIter->first)!=factorGroupEnr.end())
		{
			enrData=factorGroupEnr[fIter->first];
			enrichedin=enrData->size();
		}
		tFile1 << gnm.getCommonName(fIter->first.c_str()) << "\t" << fIter->second->size() <<"\t" << enrichedin<< endl;
		for(map<int,string>::iterator aIter=datasetgroupIDs.begin();aIter!=datasetgroupIDs.end();aIter++)
		{
			int cnt=0;
			if(factorGroup[fIter->first]->find(aIter->first)==factorGroup[fIter->first]->end())
			{
				cnt=-1;
			}
			else
			{	
				cnt=0;
			}
		
			if(enrData!=NULL && enrData->find(aIter->first)!=enrData->end())
			{
				cnt=(*enrData)[aIter->first]->size();
			}
			if(cnt>=0)
			{
				//tFile2<<"\t" << (double)cnt/total;
				tFile2<<"\t" << cnt;
			}
			else
			{
				tFile2 <<"\t" << cnt;
			}
		}
		tFile2<<endl;
	}
	tFile1.close();
	tFile2.close();
	dFile.close();
	return 0;
}

int 
sortfunc(const void* first, const void* second)
{
	int ind1=*((int*)first);	
	int ind2=*((int*)second);
	double pval1=sortedpvals[ind1];
	double pval2=sortedpvals[ind2];
	int compstat=0;
	if(pval1<pval2)
	{
		compstat=-1;
	}
	else if(pval1>pval2)
	{
		compstat=1;
	}
	return compstat;
}

int
main(int argc, const char** argv)
{
	if(argc!=5)
	{
		cout <<"Usage: " << argv[0] << " datasets.txt goldnames.txt fdr output_prefix" << endl;
		return 0;
	}
	Framework fw;
	fw.readNames(argv[2]);
	fw.readNetworks(argv[1]);
	fw.setFDR(atof(argv[3]));
	fw.compareTFsAllNw();
	fw.showComparisons(argv[4]);
	fw.showComparisonsAll(argv[4]);
	return 0;
}
