#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "graph.h"

/**
 * A graph is made up of vertices and edges
 * A vertex can be connected to other vertices via weighted, directed edge
 */


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** constructor, empty graph */
Graph::Graph() {
	numberOfVertices = 0;
	numberOfEdges = 0;
}

/** destructor, delete all vertices and edges
    only vertices stored in map
    no pointers to edges created by graph */
Graph::~Graph() {}

/** return number of vertices */
int Graph::getNumVertices() const { 
	return numberOfVertices; 
}

/** return number of vertices */
int Graph::getNumEdges() const { 
	return numberOfEdges; 
}

/** add a new edge between start and end vertex
    if the vertices do not exist, create them
    calls Vertex::connect
    a vertex cannot connect to itself
    or have multiple edges to another vertex */
bool Graph::add(std::string start, std::string end, int edgeWeight) {
	Vertex* startVertex = findOrCreateVertex(start);
	Vertex* endVertex = findOrCreateVertex(end);
	numberOfEdges++;
	return startVertex->connect(end, edgeWeight);
}

/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const { 
	// if both vertex exist
	if (findVertex(start) != nullptr && findVertex(end) != nullptr) {
		Vertex* startVertex = findVertex(start);
		startVertex->resetNeighbor();
		std::string neighborVertexLabel = 
			startVertex->getNextNeighbor();

		// loop until end is found or getNextNeighbor returns itself
		while (neighborVertexLabel != start && 
				neighborVertexLabel != end) {
			neighborVertexLabel = startVertex->getNextNeighbor();
		}
		if (neighborVertexLabel == end) {// connection found
			return startVertex->getEdgeWeight(end);
		} else { // connection not found
			return INT_MAX;
		}
	}
	return INT_MAX; // one of the vertex does not exist
}

/** read edges from file and adds to existing graph
    the first line of the file is an integer, indicating number of edges
    each edge line is in the form of "string string int"
    fromVertex  toVertex    edgeWeight */
void Graph::readFile(std::string filename) {
	std::ifstream myFile;
	myFile.open(filename);
	if (myFile.is_open()) {
		std::string firstLine;
		int edgeCount;
		int edgesNum;
		getline(myFile, firstLine);// gets first line only. No use here
		std::istringstream edgeStream(firstLine);
		if (edgeStream >> edgeCount) {
			edgesNum = edgeCount; // not using at the moment
		}
		std::string line;
		std::string from;
		std::string to;
		int num;
		int bound = 0;
		while(!myFile.eof() && bound < edgeCount) {
			bound++;
			getline(myFile, line); // gets lines below first
			std::istringstream lineStream(line);
			while (lineStream >> from >> to >> num) {
				std::string start = from;
				std::string end = to;
				int edgeWeight = num;
				add(from, to, edgeWeight);
			}
		}
		// sets numberOfEdges as read from file only if graph is empty
		// doing it just to use the integer read from file 
		// and variable related to it
		if (numberOfEdges == 0) {
			numberOfEdges = edgesNum;
		}
	} else {
		std::cout << "Did not open the file!" << std::endl;
	}
	myFile.close();
}

/** depth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel,
                                void visit(const std::string&)) {
	unvisitVertices();
	Vertex* startVertex = findVertex(startLabel);
	depthFirstTraversalHelper(startVertex, visit);
	
}

/** breadth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,
                                  void visit(const std::string&)) {}
	
/** find the lowest cost from startLabel to all vertices that can be reached
    using Djikstra's shortest-path algorithm
    record costs in the given map weight
    weight["F"] = 10 indicates the cost to get to "F" is 10
    record the shortest path to each vertex using given map previous
    previous["F"] = "C" indicates get to "F" via "C"

    cpplint gives warning to use pointer instead of a non-const map
    which I am ignoring for readability */
void Graph::djikstraCostToAllVertices(
    std::string startLabel,
    std::map<std::string, int>& weight,
    std::map<std::string, std::string>& previous) {}

/** helper for depthFirstTraversal */
void Graph::depthFirstTraversalHelper(Vertex* startVertex,
                                      void visit(const std::string&)) {
	startVertex->visit();
	std::cout << "vertexLabel: " << startVertex->getLabel() << 
			std::endl;

	startVertex->resetNeighbor();
	for (std::string neighborVertexLabel = 
			startVertex->getNextNeighbor(); neighborVertexLabel !=
		       	startVertex->getLabel(); neighborVertexLabel = 
			startVertex->getNextNeighbor()) {	
		Vertex* neighborVertex = findVertex(neighborVertexLabel);
		if (!(neighborVertex->isVisited())) {
			depthFirstTraversalHelper(neighborVertex, visit);
		}
	}
}

/** helper for breadthFirstTraversal */
void Graph::breadthFirstTraversalHelper(Vertex* startVertex,
                                        void visit(const std::string&)) {}

/** mark all verticies as unvisited */
void Graph::unvisitVertices() {
	for (std::map<std::string, Vertex*>::iterator it = vertices.begin();
			it != vertices.end(); ++it) {
		it->second->unvisit();
	}
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const { 
	if (vertices.count(vertexLabel) == 0) {
		return nullptr;
	}
	return vertices.find(vertexLabel)->second; 
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) { 
	if (vertices.count(vertexLabel) == 0) {
		Vertex* newVertex = new Vertex(vertexLabel);
		vertices[vertexLabel] = newVertex; // adds it the map vertices
		numberOfVertices++;
		return newVertex; // does it matter or need to be 
		// vertices.find(vertexLabel)->second
	}
	return vertices.find(vertexLabel)->second; 
}
