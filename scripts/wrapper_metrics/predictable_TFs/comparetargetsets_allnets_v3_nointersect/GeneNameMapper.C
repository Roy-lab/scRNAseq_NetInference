
#include <iostream>
#include <fstream>
#include <string.h>
#include "GeneNameMapper.H"

#define ORF_COMMON "registry.genenames.tab"

GeneNameMapper::GeneNameMapper()
{
}

GeneNameMapper::~GeneNameMapper()
{
}

int
GeneNameMapper::readGeneNames()
{
	ifstream inFile(ORF_COMMON);
	char buffer[4096];

	while(inFile.good())
	{
		inFile.getline(buffer,4095);
		if(strlen(buffer)<=0)
		{
			continue;
		}
			
		int tokCnt=0;
		string commName;
		string orfName;
		char* begin=buffer;
		char* end=buffer;

		while(end!=NULL)
		{
			end=strchr(begin,'\t');
			if(end!=NULL)
			{
				*end='\0';
				if(tokCnt==0)
				{
					commName.append(begin);
					char cName[100];
					int len=strlen(begin);
					int i=0;
					while(i<len)
					{
						cName[i]=tolower(begin[i]);
						i++;
					}
					cName[len]='\0';
				}
				else if(tokCnt==5)
				{
					orfName.append(begin);
				}
				tokCnt++;
				begin=end+1;
			}
		}
		
		orfToCommon[orfName]=commName;		
	}
	inFile.close();
	return 0;
}


const char*
GeneNameMapper::getCommonName(const char* aGene)
{
	string key(aGene);
	if(orfToCommon.find(key)==orfToCommon.end())
	{
		return aGene;
	}
	return orfToCommon[key].c_str();
}

