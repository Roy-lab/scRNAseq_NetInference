#include <iostream>
#include "Gene.H"

Gene::Gene()
{
}

Gene::~Gene()
{
}

int 
Gene::addExpVals(DBL_MAP* valSet)
{
	expValues.push_back(valSet);
	return 0;
}

int 
Gene::collapseReplicate(map<int,string>& experimentID)
{
	for(map<int,string>::iterator aIter=experimentID.begin();aIter!=experimentID.end();aIter++)
	{
		double s=0;
		double obs=0;
		for(int j=0;j<expValues.size();j++)
		{
			DBL_MAP* vset=expValues[j];
			if(vset->find(aIter->first)==vset->end())
			{
				continue;
			}
			s=s+(*vset)[aIter->first];
			obs=obs+1;
		}
		if(obs==0)
		{
			continue;
		}
		value[aIter->first]=s/obs;
	}
	if(expValues.size()>1)
	{
		cout <<"Collapsed " << name << endl;
	}
	return 0;
}

double
Gene::getValue(int dim)
{
	if(value.find(dim)==value.end())
	{
		return -999;
	}
	return value[dim];
}
