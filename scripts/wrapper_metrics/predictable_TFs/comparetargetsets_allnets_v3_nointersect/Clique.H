/*Stores a clique of vertices. This can be considered as a compound node. We need to inherit from*/

#ifndef _CLIQUE
#define _CLIQUE
#include <vector>
#include <fstream>
using namespace std;

class Vertex;
class Clique
{
	public:
		Clique();
		~Clique();
		int addMember(Vertex*);
		int getMemberCnt();
		Vertex* getMemberAt(int);
		bool inClique(Vertex*);
		int showClique(ofstream&,int);
	private:
		vector<Vertex*> cliqueMembers;
};

#endif
