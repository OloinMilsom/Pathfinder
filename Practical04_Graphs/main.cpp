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
	ofstream myfile;
	myfile.open("output.txt", ios_base::app);
	myfile << "Visiting: " << pNode->data() << endl;
	myfile.close();
}

void printPath(vector<Node *> vec) {
	ofstream myfile;
	myfile.open("output.txt", ios_base::app);
	for (int i = 0; i < vec.size(); i++)
	{
		myfile << "Path: " << vec[i]->data() << endl;
	}
	myfile << endl << endl << endl;
	myfile.close();

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

	// Now traverse the graph.
	//graph.breadthFirst( graph.nodeArray()[0], visit );
	//graph.breadthFirstSearch(graph.nodeArray()[0], graph.nodeArray()[15], visit);
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			vector<Node *> vec;
			graph.ucs(graph.nodeArray()[i], graph.nodeArray()[j], visit, vec);
			printPath(vec);
		}
	}


	system("PAUSE");
	
}
