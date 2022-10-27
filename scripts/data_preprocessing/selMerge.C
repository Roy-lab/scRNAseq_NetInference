#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <map>

using namespace std;

typedef vector<string> PNT_VALUES;
typedef map<string,PNT_VALUES*> ORF_DATA;
typedef map<string,PNT_VALUES*>::iterator ORF_DATA_ITER;

ORF_DATA spellData;
ORF_DATA juanitaData;
PNT_VALUES selSequence;

int readOrfData(const char*,ORF_DATA&,PNT_VALUES* aSeq=NULL);
int getMatches(const char*);

int main(int argc, char** argv)
{
	if(argc!=4)
	{
		cout << "Usage selMerge selectionfile datafile outputfile" << endl;
		return 0;
	}
	readOrfData(argv[1],spellData,&selSequence);
	readOrfData(argv[2],juanitaData);
	getMatches(argv[3]);
	return 0;
}

int readOrfData(const char* dataFile,ORF_DATA& fillMe,PNT_VALUES* aSeq)
{
	ifstream inFile(dataFile);
	char* buffer=NULL;
	string buffstr;
	int len=0;
	while(inFile.good())
	{
		//inFile.getline(buffer,81910);
		getline(inFile,buffstr);
		//if(strlen(buffer)<=0)
		if(buffstr.length()<=0)
		{
			continue;
		}
		if(buffstr.length()>=len)
		{
			len=buffstr.length()+1;
			if(buffer!=NULL)
			{
				delete[] buffer;
			}
			buffer=new char[len];
		}
		strcpy(buffer,buffstr.c_str());
		int tokCnt=0;
		char* start=buffer;
		char* end=NULL;
		string orfName;
		PNT_VALUES* aPnt=new PNT_VALUES;
		
		while(start!=NULL)
		{
			end=strchr(start,'\t');
			if(end!=NULL)
			{
				*end='\0';
			}
			char* pos=strchr(start,'\r');
			if(pos!=NULL)
			{
				*pos='\0';
			}
			/*pos=strchr(start,' ');
			if(pos!=NULL)
			{
				*pos='\0';
			}*/
			if(tokCnt==0)
			{
				orfName.append(start);
			}
			else
			{
				string aValue(start);
				aPnt->push_back(aValue);
			}
			tokCnt++;
			if(end!=NULL)
			{
				if((end+1)!=NULL)
				{
					start=end+1;
				}
			}
			else
			{
				start=NULL;
			}
				
		}
		fillMe[orfName]=aPnt;
		if(aSeq!=NULL)
		{
			(*aSeq).push_back(orfName);
		}
	}
	
	return 0;
}

//Get the matches of the spellman dataset with juanita's dataset
//This can be done by iterating over spellman's dataset and 
//getting the value vector from Juanita's dataset

int
getMatches(const char* fName)
{
	ofstream oFile(fName);
	for(int i=0;i<selSequence.size();i++)
	{
		ORF_DATA_ITER dataIter=juanitaData.find(selSequence[i]);
		if(dataIter!=juanitaData.end())
		{
			oFile << dataIter->first;
			PNT_VALUES* values=dataIter->second;
			for(int i=0;i<values->size();i++)
			{
				oFile <<"\t" <<(*values)[i].c_str();
			}			
			oFile << endl;
		}
		else
		{
			cout << selSequence[i].c_str() << endl;
		}
	}	
	oFile.close();
	return 0;
}

