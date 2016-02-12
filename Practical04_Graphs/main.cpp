//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include "Graph.h"

using namespace std;

typedef GraphArc<int, int> Edge;
typedef GraphNode<string, int> Node;

void visit( Node * pNode ) {

	pNode->setColour(sf::Color::Magenta);
	cout << "Visiting: " << pNode->data() << endl;
}

void printPath(vector<Node *> vec) {
	for (int i = 0; i < vec.size(); i++)
	{
		vec[i]->setColour(sf::Color::Green);
		cout << "Path: " << vec[i]->data() << endl;
	}
}

pair<sf::VertexArray, sf::Text> setupEdges(sf::Vector2f from, sf::Vector2f to, int weight, sf::Font* font)
{
	sf::VertexArray lines(sf::Lines, 2);
	lines[0].position = sf::Vector2f(from.x + 30, from.y + 30);
	lines[1].position = sf::Vector2f(to.x + 30, to.y + 30);
	sf::Text text;
	text.setPosition((lines[0].position + lines[1].position) * 0.5f);
	text.setCharacterSize(12);
	text.setString(to_string(weight));
	text.setFont(*font);
	return make_pair(lines, text);
}

void initialiseGraph(Graph<string, int> & g, sf::Font * font, vector<pair<sf::VertexArray, sf::Text>> &arcs) {
	string c = "";
	int i = 0;
	float x, y;
	ifstream myfile;
	myfile.open("Q1Nodes.txt");

	while (myfile >> c >> x >> y) {
		g.addNode(c, x+100, y+100, font, i++);
	}

	myfile.close();
	myfile.open("Q1Arcs.txt");

	int from, to, weight;
	while (myfile >> from >> to >> weight) {
		g.addArc(from, to, weight);
		arcs.push_back(setupEdges(g.nodeArray()[from]->getPosition(), g.nodeArray()[to]->getPosition(), weight, font));
	}
	myfile.close();
}



int main(int argc, char *argv[]) {
	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML First Program");

	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	Graph<string, int> graph(30);
	vector<pair<sf::VertexArray, sf::Text>> arcs;
	initialiseGraph(graph, &font, arcs);
	
	vector<Node *> vec;
	graph.aStar(graph.nodeArray()[0], graph.nodeArray()[29], visit, vec);
	printPath(vec);

	// Start game loop 
	while (window.isOpen())
	{
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed)
				window.close();

			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();

			if (Event.type == sf::Event::MouseButtonPressed) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
					graph.getNodeAtMouse(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
				}
			}
		}

		//prepare frame
		window.clear();

		
		for (int i = 0; i < arcs.size(); i++)
		{
			window.draw(arcs[i].first);
			window.draw(arcs[i].second);
		}

		graph.draw(&window);

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame
}

