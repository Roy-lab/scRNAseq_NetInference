#include <iostream>

using namespace std;

#include "Vertex.H"
#include "Clique.H"


Clique::Clique()
{
}

Clique::~Clique()
{
}

int 
Clique::addMember(Vertex* anode)
{
	cliqueMembers.push_back(anode);
	return 0;
}

int 
Clique::getMemberCnt()
{
	return cliqueMembers.size();
}

Vertex* 
Clique::getMemberAt(int index)
{
	return cliqueMembers[index];
}

//Check if the node n is in this clique, i.e. it is connected to all the nodes in this
//clique
bool
Clique::inClique(Vertex* n)
{
	bool yes=true;
	int nodeIter=0;
	while(nodeIter<cliqueMembers.size() && yes)
	{
		Vertex* member=cliqueMembers[nodeIter];
		if((!member->isOutNeighbour(n)) || (!n->isOutNeighbour(member)))
		{
			yes=false;
		}
		else
		{
			nodeIter++;
		}
	}
	return yes;
}

int
Clique::showClique(ofstream& oFile, int minClq)
{
	if(cliqueMembers.size() <=minClq)
	{
		return 0;
	}
	for(int i=0;i<cliqueMembers.size();i++)
	{
		oFile << " " << cliqueMembers[i]->getName() ;
	}
	oFile << endl;
	return 0;
}
