#ifndef TREE_H
#define TREE_H

/*
 *  tree.h
 *  Mothur
 *
 *  Created by Sarah Westcott on 1/22/09.
 *  Copyright 2009 Schloss Lab UMASS Amherst. All rights reserved.
 *
 */

#include "treenode.h"
#include "counttable.h"
#include "currentfile.h"
/* This class represents the treefile. */

class Tree {
public: 
	Tree(string, vector<string>&);  //do not use tree generated by this constructor its just to extract the treenames, its a chicken before the egg thing that needs to be revisited.
	Tree(int, CountTable*, vector<string>&);
	Tree(CountTable*, vector<string>&);		//to generate a tree from a file
    Tree(CountTable*, vector< vector<double> >&, vector<string>&); //create tree from sim matrix
	~Tree();
	
    CountTable* getCountTable() { return ct; }
    vector<string> getTreeNames() { return Treenames; }
	void getCopy(Tree*);  //makes tree a copy of the one passed in.
    void getCopy(Tree* copy, bool); //makes a copy of the tree structure passed in, (just parents, children and br). Used with the Tree(TreeMap*) constructor. Assumes the tmap already has set seqs groups you want.  Used by subsample to reassign seqs you don't want included to group "doNotIncludeMe".
	void getSubTree(Tree*, vector<string>);  //makes tree a that contains only the names passed in.
    //int getSubTree(Tree* originalToCopy, vector<string> seqToInclude, map<string, string> nameMap);  //used with (int, TreeMap) constructor. SeqsToInclude contains subsample wanted - assumes these are unique seqs and size of vector=numLeaves passed into constructor. nameMap is unique -> redundantList can be empty if no namesfile was provided. 
    
	void assembleRandomTree();
	void assembleRandomUnifracTree(vector<string>);
	void assembleRandomUnifracTree(string, string);
    
	void createNewickFile(string);
	int getIndex(string);
	void setIndex(string, int);
	int getNumNodes() { return numNodes; }
	int getNumLeaves(){	return numLeaves; }
	map<string, int> mergeUserGroups(int, vector<string>);  //returns a map with a groupname and the number of times that group was seen in the children
	void printTree();
	void print(ostream&);
	void print(ostream&, string);
    void print(ostream&, map<string, string>);
	int findRoot();  //return index of root node
	
	//this function takes the leaf info and populates the non leaf nodes
	int assembleTree();	
	
	vector<Node> tree;		//the first n nodes are the leaves, where n is the number of sequences.
	map< string, vector<int> > groupNodeInfo;	//maps group to indexes of leaf nodes with that group, different groups may contain same node because of names file.
			
private:
	CountTable* ct;
	int numNodes, numLeaves;
	ofstream out;
	string filename;
	
    //map<string, string> names;
	map<string, int>::iterator it, it2;
	map<string, int> mergeGroups(int);  //returns a map with a groupname and the number of times that group was seen in the children
	map<string,int> mergeGcounts(int);
    map<string, int> indexes; //maps seqName -> index in tree vector
	
	void addNamesToCounts(map<string, string>);
	void randomTopology();
	void randomBlengths();
	void randomLabels(vector<string>);
	//void randomLabels(string, string);
	void printBranch(int, ostream&, map<string, string>);  //recursively print out tree
    void printBranch(int, ostream&, string);
	vector<string> parseTreeFile(string);	//parses through tree file to find names of nodes and number of them
							//this is required in case user has sequences in the names file that are
							//not included in the tree. 
							//only takes names from the first tree in the tree file and assumes that all trees use the same names.
	vector<string> readTreeString(ifstream&);
	int populateNewTree(vector<Node>&, int, int&);
	void printBranch(int, ostream&, string, vector<Node>&);
		
	MothurOut* m;
    CurrentFile* current;
    vector<string> Treenames;
		
};

#endif
