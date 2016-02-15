#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include "stdafx.h"
#include <list>

// Forward references
template <typename NodeType, typename ArcType> class GraphArc;

// -------------------------------------------------------
// Name:        GraphNode
// Description: This is the node class. The node class 
//              contains data, and has a linked list of 
//              arcs.
// -------------------------------------------------------
template<class NodeType, class ArcType>
class GraphNode {
private:    
// typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;
// -------------------------------------------------------
// Description: data inside the node
// -------------------------------------------------------
    NodeType m_data;

// -------------------------------------------------------
// Description: physical position of the node
// -------------------------------------------------------
	sf::Vector2f m_pos;

// -------------------------------------------------------
// Description: current distance from search node (g(n))
// -------------------------------------------------------
	float m_searchDistance;

// -------------------------------------------------------
// Description: current heuristic from search node (h(n))
// -------------------------------------------------------
	float m_heuristic;

// -------------------------------------------------------
// Description: list of arcs that the node has.
// -------------------------------------------------------
    list<Arc> m_arcList;

// -------------------------------------------------------
// Description: This remembers if the node is marked.
// -------------------------------------------------------
    bool m_marked;

// -------------------------------------------------------
// Description: The previous node for a search
// -------------------------------------------------------
	Node* m_prevNode;

	vector<sf::Text> m_text;
	sf::CircleShape m_shape;

public:
    // Accessor functions
    list<Arc> const & arcList() const {
        return m_arcList;              
    }

    bool marked() const {
        return m_marked;
    }

    NodeType const & data() const {
        return m_data;
    }

	sf::Vector2f getPosition() const {
		return m_pos;
	}

	float const & getSearchDistance() const {
		return m_searchDistance;
	}

	float const & getHeuristic() const {
		return m_heuristic;
	}

	float const & getCost() const {
		return m_heuristic + m_searchDistance;
	}

    // Manipulator functions
    void setData(NodeType data) {
        m_data = data;
    }

	void setPosition(float x, float y) {
		m_pos.x = x;
		m_pos.y = y;
		m_shape = sf::CircleShape(30);
		m_shape.setPosition(m_pos);
		m_text[0].setPosition(m_pos + sf::Vector2f(10,0));
		m_text[1].setPosition(m_pos + sf::Vector2f(10, 16));
		m_text[2].setPosition(m_pos + sf::Vector2f(10, 32));
	}

	void setText(sf::Font * f) {
		m_text = vector<sf::Text>(3);
		m_text[0].setString(m_data);
		m_text[0].setFont(*f);
		m_text[0].setCharacterSize(20);
		m_text[0].setColor(sf::Color::Black);

		m_text[1].setString("g(n):0");
		m_text[1].setFont(*f);
		m_text[1].setCharacterSize(14);
		m_text[1].setColor(sf::Color::Black);

		m_text[2].setString("h(n):0");
		m_text[2].setFont(*f);
		m_text[2].setCharacterSize(14);
		m_text[2].setColor(sf::Color::Black);
	}
    
    void setMarked(bool mark) {
        m_marked = mark;
    }

	void setSearchDistance(float dist) {
		m_searchDistance = dist;
		if (m_searchDistance != numeric_limits<float>::infinity())
			m_text[1].setString("g(n):" + to_string(static_cast<int>(m_searchDistance)));
		else
			m_text[1].setString("g(n):0");
	}

	void setHeuristic(float h) {
		m_heuristic = h;
		m_text[2].setString("h(n):" + to_string(static_cast<int>(m_heuristic)));
	}

	void setPrevious(Node* prev) {
		m_prevNode = prev;
	}

	void setColour(sf::Color c) {
		m_shape.setFillColor(c);
	}

	Node* getPrevious(){
		return m_prevNode;
	}

    Arc* getArc( Node* pNode );    
    void addArc( Node* pNode, ArcType pWeight );
    void removeArc( Node* pNode );
	void printPrevious(void(*pProcess)(Node*));
	void draw(sf::RenderWindow * window);
	bool intersects(int x, int y);
};

// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    This finds the arc in the current node that
//                  points to the node in the parameter.
//  Arguments:      The node that the arc connects to.
//  Return Value:   A pointer to the arc, or 0 if an arc doesn't
//                  exist from this to the specified input node.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
GraphArc<NodeType, ArcType>* GraphNode<NodeType, ArcType>::getArc( Node* pNode ) {

     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();
     Arc* pArc = 0;
     
     // find the arc that matches the node
     for( ; iter != endIter && pArc == 0; ++iter ) {         
          if ( (*iter).node() == pNode) {
               pArc = &( (*iter) );
          }
     }

     // returns null if not found
     return pArc;
}


// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    This adds an arc from the current node pointing
//                  to the first parameter, with the second parameter 
//                  as the weight.
//  Arguments:      First argument is the node to connect the arc to.
//                  Second argument is the weight of the arc.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::addArc( Node* pNode, ArcType weight ) {
   // Create a new arc.
   Arc a;
   a.setNode(pNode);
   a.setWeight(weight);   
   // Add it to the arc list.
   m_arcList.push_back( a );
}


// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This finds an arc from this node to input node 
//                  and removes it.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::removeArc( Node* pNode ) {
     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();

     int size = m_arcList.size();
     // find the arc that matches the node
     for( ; iter != endIter && m_arcList.size() == size;  
                                                    ++iter ) {
          if ( (*iter).node() == pNode) {
             m_arcList.remove( (*iter) );
          }                           
     }
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::printPrevious(void(*pProcess)(Node*)) {
	if (m_prevNode != nullptr) {
		pProcess(m_prevNode);
		m_prevNode->printPrevious(pProcess);
	}
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::draw(sf::RenderWindow * window){
	window->draw(m_shape);
	for (int i = 0; i < m_text.size(); i++)
	{
		window->draw(m_text[i]);
	}
}

template<typename NodeType, typename ArcType>
bool GraphNode<NodeType, ArcType>::intersects(int x, int y){
	return x > m_pos.x && x < m_pos.x + 2 * m_shape.getRadius() && y > m_pos.y && y < m_pos.y + 2 * m_shape.getRadius();
}

#include "GraphArc.h"

#endif
