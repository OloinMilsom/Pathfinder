#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "Graph.h"

using namespace std;


typedef GraphArc<int, int> Arc;
typedef GraphNode<string, int> Node;

void visit( Node * pNode ) {
	cout << "Visiting: " << pNode->data() << endl;
}

void printPath(vector<Node *> vec) {
	for (int i = 0; i < vec.size(); i++)
	{
		cout << "Path: " << vec[i]->data() << endl;
	}
}

int main(int argc, char *argv[]) {
   
	Graph<string, int> graph(30);
	
	string c = "";
	int i = 0;
	ifstream myfile;
	myfile.open ("Q1Nodes.txt");

	while ( myfile >> c ) {
		graph.addNode(c, i++);
	}

	myfile.close();
	myfile.open("Q1Arcs.txt");

	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
		graph.addArc(from, to, weight);
	}
    myfile.close();
	
	vector<Node *> vec;
	graph.aStar(graph.nodeArray()[0], graph.nodeArray()[9], visit, vec);
	printPath(vec);
	/*for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			vector<Node *> vec;
			graph.ucs(graph.nodeArray()[i], graph.nodeArray()[j], visit, vec);
			printPath(vec);
		}
	}*/


	system("PAUSE");
	
}
