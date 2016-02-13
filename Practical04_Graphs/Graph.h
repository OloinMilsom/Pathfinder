#ifndef GRAPH_H
#define GRAPH_H

#include "stdafx.h"
#include <list>
#include <queue>

using namespace std;

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph {
private:

    // typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

// ----------------------------------------------------------------
//  Description:    An array of all the nodes in the graph.
// ----------------------------------------------------------------
    Node** m_pNodes;

// ----------------------------------------------------------------
//  Description:    The maximum number of nodes in the graph.
// ----------------------------------------------------------------
    int m_maxNodes;


// ----------------------------------------------------------------
//  Description:    The actual number of nodes in the graph.
// ----------------------------------------------------------------
    int m_count;


public:           
    // Constructor and destructor functions
    Graph( int size );
    ~Graph();

    // Accessors
    Node** nodeArray() const {
       return m_pNodes;
    }

    // Public member functions.
	bool addNode(NodeType data, float x, float y, sf::Font * font, int index);
    void removeNode( int index );
    bool addArc( int from, int to, ArcType weight );
    void removeArc( int from, int to );
    Arc* getArc( int from, int to );        
    void clearMarks();
    void depthFirst( Node* pNode, void (*pProcess)(Node*) );
    void breadthFirst( Node* pNode, void (*pProcess)(Node*) );
	void breadthFirstSearch(Node* pStart, Node* pGoal, void (*pProcess)(Node*));
	void ucs(Node* pStart, Node* pDest, void(*pVisitFunc)(Node*), std::vector<Node *>& path);
	void aStar(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node *>& path);
	void draw(sf::RenderWindow * window);
	Node* getNodeAtMouse(int x, int y);
	void reset();

	// functor struct for priority queue for ucs
	struct UCSSearchCostCompare{
	public:
		bool operator()(Node * n1, Node * n2) {
			return n1->getSearchDistance() > n2->getSearchDistance();
		}
	};

	// functor struct for priority queue for A*
	struct AStarSearchCostCompare{
	public:
		bool operator()(Node * n1, Node * n2) {
			return (n1->getHeuristic() + n1->getSearchDistance()) > (n2->getHeuristic() + n2->getSearchDistance());
		}
	};
};

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph( int size ) : m_maxNodes( size ) {
   int i;
   m_pNodes = new Node * [m_maxNodes];
   // go through every index and clear it to null (0)
   for( i = 0; i < m_maxNodes; i++ ) {
        m_pNodes[i] = 0;
   }

   // set the node count to 0.
   m_count = 0;
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
   int index;
   for( index = 0; index < m_maxNodes; index++ ) {
        if( m_pNodes[index] != 0 ) {
            delete m_pNodes[index];
        }
   }
   // Delete the actual array
   delete m_pNodes;
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode(NodeType data, float x, float y, sf::Font * font, int index) {
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( m_pNodes[index] == 0) {
      nodeNotPresent = true;
      // create a new node, put the data in it, and unmark it.
      m_pNodes[index] = new Node;
      m_pNodes[index]->setData(data);
      m_pNodes[index]->setMarked(false);
	  m_pNodes[index]->setText(font);
	  m_pNodes[index]->setPosition(x, y);

      // increase the count and return success.
      m_count++;
    }
        
    return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode( int index ) {
     // Only proceed if node does exist.
     if( m_pNodes[index] != 0 ) {
         // now find every arc that points to the node that
         // is being removed and remove it.
         int node;
         Arc* arc;

         // loop through every node
         for( node = 0; node < m_maxNodes; node++ ) {
              // if the node is valid...
              if( m_pNodes[node] != 0 ) {
                  // see if the node has an arc pointing to the current node.
                  arc = m_pNodes[node]->getArc( m_pNodes[index] );
              }
              // if it has an arc pointing to the current node, then
              // remove the arc.
              if( arc != 0 ) {
                  removeArc( node, index );
              }
         }
        

        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_pNodes[index];
        m_pNodes[index] = 0;
        m_count--;
    }
}

// ----------------------------------------------------------------
//  Name:           addArd
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) {
     bool proceed = true; 
     // make sure both nodes exist.
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     if( m_pNodes[from]->getArc( m_pNodes[to] ) != 0 ) {
         proceed = false;
     }

     if (proceed == true) {
        // add the arc to the "from" node.
        m_pNodes[from]->addArc( m_pNodes[to], weight );
     }
        
     return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc( int from, int to ) {
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         nodeExists = false;
     }

     if (nodeExists == true) {
        // remove the arc.
        m_pNodes[from]->removeArc( m_pNodes[to] );
     }
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
// Dev-CPP doesn't like Arc* as the (typedef'd) return type?
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc( int from, int to ) {
     Arc* pArc = 0;
     // make sure the to and from nodes exist
     if( m_pNodes[from] != 0 && m_pNodes[to] != 0 ) {
         pArc = m_pNodes[from]->getArc( m_pNodes[to] );
     }
                
     return pArc;
}


// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() {
     int index;
     for( index = 0; index < m_maxNodes; index++ ) {
          if( m_pNodes[index] != 0 ) {
              m_pNodes[index]->setMarked(false);
          }
     }
}


// ----------------------------------------------------------------
//  Name:           depthFirst
//  Description:    Performs a depth-first traversal on the specified 
//                  node.
//  Arguments:      The first argument is the starting node
//                  The second argument is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::depthFirst( Node* pNode, void (*pProcess)(Node*) ) {
     if( pNode != 0 ) {
           // process the current node and mark it
           pProcess( pNode );
           pNode->setMarked(true);

           // go through each connecting node
           list<Arc>::iterator iter = pNode->arcList().begin();
           list<Arc>::iterator endIter = pNode->arcList().end();
        
		   for( ; iter != endIter; ++iter) {
			    // process the linked node if it isn't already marked.
                if ( (*iter).node()->marked() == false ) {
                   depthFirst( (*iter).node(), pProcess);
                }            
           }
     }
}


// ----------------------------------------------------------------
//  Name:           breadthFirst
//  Description:    Performs a depth-first traversal the starting node
//                  specified as an input parameter.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirst( Node* pNode, void (*pProcess)(Node*) ) {
   if( pNode != 0 ) {
	  queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( pNode );
      pNode->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 ) {
         // process the node at the front of the queue.
         pProcess( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) {
              if ( (*iter).node()->marked() == false) {
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirstSearch(Node* pStart, Node* pGoal, void(*pProcess)(Node*)) {
	if (pStart != 0) {
		queue<Node*> nodeQueue;
		// place the first node on the queue, and mark it.
		nodeQueue.push(pStart);
		pStart->setMarked(true);

		bool goalReached = false;
		// loop through the queue while there are nodes in it.
		while (nodeQueue.size() != 0 && !goalReached) {
			// process the node at the front of the queue.
			//pProcess(nodeQueue.front());

			// add all of the child nodes that have not been 
			// marked into the queue
			list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
			list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();

			for (; iter != endIter && !goalReached; iter++) {
				if ((*iter).node() == pGoal) {
					goalReached = true;
					pGoal->setPrevious(nodeQueue.front());
					pProcess(pGoal);
					pGoal->printPrevious(pProcess);
				}
				if ((*iter).node()->marked() == false) {
					// mark the node and add it to the queue.
					(*iter).node()->setPrevious(nodeQueue.front());
					(*iter).node()->setMarked(true);
					nodeQueue.push((*iter).node());
				}
			}

			// dequeue the current node.
			nodeQueue.pop();
		}
	}
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::ucs(Node* pStart, Node* pDest, void(*pVisitFunc)(Node*), std::vector<Node *>& path){
	if (pStart != 0) {
		priority_queue<Node*, vector<Node *>, UCSSearchCostCompare> nodeQueue;

		// set starting search distance to be 0
		pStart->setSearchDistance(0);
		pStart->setHeuristic(0);
		for (int i = 0; i < m_count; i++)
		{
			// if not starting set to be large number
			if (m_pNodes[i] != pStart)
				m_pNodes[i]->setSearchDistance(numeric_limits<float>::infinity());
			// reset all previous pointers
			m_pNodes[i]->setPrevious(nullptr);
			m_pNodes[i]->setMarked(false);
		}

		// add start node to top of queue and mark it
		nodeQueue.push(pStart);
		pStart->setMarked(true);

		// loop through the queue while there are nodes in it.
		while (!nodeQueue.empty()) {
			// print visiting current top of queue
			//pVisitFunc(nodeQueue.top());

			// iterate through the children of the top of queue
			list<Arc>::const_iterator iter = nodeQueue.top()->arcList().begin();
			list<Arc>::const_iterator endIter = nodeQueue.top()->arcList().end();
			for (; iter != endIter; iter++) {
				if ((*iter).node() != nodeQueue.top()->getPrevious())
				{
					// if the distance of the current route is shorter than the distance of 
					// the previous shortest route, change the distance and previous node accordingly
					float dist = (*iter).weight() + nodeQueue.top()->getSearchDistance();
					if (dist < (*iter).node()->getSearchDistance()){
						(*iter).node()->setSearchDistance(dist);
						// this is for question 1 of A* assignment
						//(*iter).node()->setHeuristic(dist * 0.9);
						(*iter).node()->setPrevious(nodeQueue.top());
					}
					// add all of the child nodes that have not been 
					// marked into the queue
					if (!(*iter).node()->marked()){
						nodeQueue.push((*iter).node());
						(*iter).node()->setMarked(true);
					}
				}
			}
			// dequeue the current node.
			nodeQueue.pop();
		}
	}
	// add path nodes onto the reference vector: path
	Node * currNode = pDest;
	while (currNode != nullptr)
	{
		path.push_back(currNode);
		currNode = currNode->getPrevious();
	}
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::aStar(Node* pStart, Node* pDest, void(*pProcess)(Node*), std::vector<Node *>& path) {
	/*Let s = the starting node, g = goal node
	Let pq = a new priority queue
	Initialise g[s] to 0  
	For each node v in graph G
		Calculate h[v] // Compute estimated distance to goal for each node.
	   Initialise g[v] to infinity // Don’t yet know the distances to these nodes 

	Add s to the pq
	Mark(s)
	While the queue is not empty AND pq.top() != g
		For each child node c of pq.top()
		   If (c != previous(pq.top() AND c has not been removed from the pq)
				Let distC = h(c) + g(c) // g(c) is actual path cost to child
			   If ( distC < f(c) )
				  let f[c] = distC
				  Set previous pointer of c to pq.top()
			   End if
  				If (notMarked(c))
					  Add c to the pq 
					  Mark(c)
			   End if
			  End if
		End for
	   Remove pq.top()	
	End while
	*/
	if (pStart != 0) {
		// create priority queue with ordering based on f(n) or total cost
		priority_queue<Node*, vector<Node *>, AStarSearchCostCompare> nodeQueue;
		vector<Node *> newNodes;
		// ucs heuristic for part 1
		//ucs(pDest, pStart, pProcess, path);

		// set starting search distance to be 0
		pStart->setSearchDistance(0);
		sf::Vector2f endPos = pDest->getPosition();
		for (int i = 0; i < m_count; i++)
		{
			// if not starting set to be large number
			if (m_pNodes[i] != pStart) {
				m_pNodes[i]->setSearchDistance(numeric_limits<float>::infinity());
				
			}
			sf::Vector2f currPos = m_pNodes[i]->getPosition();
			m_pNodes[i]->setHeuristic(sqrt((currPos.x - endPos.x) * (currPos.x - endPos.x) + (currPos.y - endPos.y) * (currPos.y - endPos.y)));
			// reset all previous pointers
			m_pNodes[i]->setPrevious(nullptr);
			m_pNodes[i]->setMarked(false);
		}

		bool found = false;

		// add start node to top of queue and mark it
		nodeQueue.push(pStart);
		pStart->setMarked(true);
		while (!nodeQueue.empty() && !found) {
			// print visiting current top of queue
			pProcess(nodeQueue.top());

			// iterate through the children of the top of queue
			list<Arc>::const_iterator iter = nodeQueue.top()->arcList().begin();
			list<Arc>::const_iterator endIter = nodeQueue.top()->arcList().end();
			for (; iter != endIter; iter++) {
				if ((*iter).node() != nodeQueue.top()->getPrevious())
				{
					// if the distance of the current route is shorter than the distance of 
					// the previous shortest route, change the distance and accordingly
					float searchDist = (*iter).weight() + nodeQueue.top()->getSearchDistance();
					float distC = (*iter).node()->getHeuristic() + searchDist;
					float cost = (*iter).node()->getHeuristic() + (*iter).node()->getSearchDistance();
					if (distC < cost) {
						(*iter).node()->setSearchDistance(searchDist);
						(*iter).node()->setPrevious(nodeQueue.top());
					}
					// add all of the child nodes that have not been 
					// marked into the queue
					if (!(*iter).node()->marked()){
						newNodes.push_back((*iter).node());
						(*iter).node()->setMarked(true);
					}
					if ((*iter).node() == pDest) {
						found = true;
					}
				}
			}
			// dequeue the current node.
			nodeQueue.pop();
			for (int i = 0; i < newNodes.size(); i++)
			{
				nodeQueue.push(newNodes[i]);
			}
			newNodes.clear();
		}
	}
	Node * currNode = pDest;
	path.clear();
	while (currNode != nullptr)
	{
		path.push_back(currNode);
		currNode = currNode->getPrevious();
	}
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::draw(sf::RenderWindow * window){
	for (int i = 0; i < m_count; i++)
	{
		m_pNodes[i]->draw(window);
	}
}

template<class NodeType, class ArcType>
GraphNode<NodeType, ArcType>* Graph<NodeType, ArcType>::getNodeAtMouse(int x, int y) {
	for (int i = 0; i < m_count; i++)
	{
		if (m_pNodes[i]->intersects(x, y))
			return m_pNodes[i];
	}
	return nullptr;
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::reset(){
	for (int i = 0; i < m_count; i++)
	{
		m_pNodes[i]->setColour(sf::Color::White);
		m_pNodes[i]->setSearchDistance(numeric_limits<float>::infinity());
		m_pNodes[i]->setHeuristic(0);
	}
}

#include "GraphNode.h"
#include "GraphArc.h"


#endif
