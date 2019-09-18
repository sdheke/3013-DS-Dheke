//****************************************************************
// Sabin Dheke
// Assignment 1
// Professor Terry Griffin
// Due Date: 09/18/2019
// In this assignment, we build the binary search tree that hold 
// certain number of integers. We have the search tree in such 
// a way that will be full and complete.
//****************************************************************


#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
#include<fstream>

using namespace std;

struct Node
{
public:
	Node * left;
	Node *right;
	int data;
	Node()
	{
		data = -1;
		left = NULL;
		right = NULL;

	}
	Node(int x)
	{
		data = x;
		left = NULL;
		right = NULL;
	}
};
class BTree
{
public:

	// This function will create the list from 1 to i-1
	vector < int >createList(int i)
	{
		vector < int >x;
		for (int j = 1; j < i; j++)
		{
			x.push_back(j);
		}
		return x;
	}
	
	Node *root;

	BTree()
	{
		root = NULL;
	}
	
	// adding data to the tree
	void addData(int data)
	{
		if (root == NULL)
		{
			root = new Node();
			root->data = data;
			return;
		}


		Node *parent = root;

		while (parent != NULL)
		{
			if (data - (parent->data) > 0)
			{
				if (parent->right == NULL)
				{
					parent->right = new Node();
					parent = parent->right;
					break;
				}
				parent = parent->right;
			}
			else
			{
				if (parent->left == NULL)
				{
					parent->left = new Node();
					parent = parent->left;
					break;
				}
				parent = parent->left;
			}
		}
		if (parent == NULL)
		{
			parent = new Node();
		}
		parent->data = data;
	}


	// This will create the function that will make a complete binary
	// search tree from the list numbers
	void ConstructCompleteBSTfromList(vector < int >treeList)
	{
		sort(treeList.begin(), treeList.end());
		vector < vector < int >> listsQueue;	
		// this holds right and left parts in each iteeration
		listsQueue.push_back(treeList);
		while (listsQueue.size() > 0)
		{
			vector < vector < int >> tSplitLists;
			for (int i = 0; i < listsQueue.size(); i++)
			{
				vector < int >tIntList = listsQueue[i];
				int length = tIntList.size();
				int mid = calcMid(length);
				addData(tIntList[mid]);
				if (mid - 0 > 0)
				{
					tSplitLists.push_back(vector <
						int >(tIntList.begin(),
							tIntList.begin() + mid));
				}
				if (length - (mid + 1) > 0)
				{
					tSplitLists.push_back(vector <
						int >(tIntList.begin() + mid + 1,
							tIntList.end()));
				}
			}
			listsQueue = tSplitLists;
		}
	}
	
	// copied this code from the professor's code that he provided for the assignment
	void GraphVizGetIds(Node *nodePtr, ofstream &VizOut)
	{
		static int NullCount = 0;
		if (nodePtr)
		{
			GraphVizGetIds(nodePtr->left, VizOut);
			VizOut << "node" << nodePtr->data
				<< "[label=\"" << nodePtr->data << "\\n"
				//<<"Add:"<<nodePtr<<"\\n"
				//<<"Par:"<<nodePtr->parent<<"\\n"
				//<<"Rt:"<<nodePtr->right<<"\\n"
				//<<"Lt:"<<nodePtr->left<<"\\n"
				<< "\"]" << endl;
			if (!nodePtr->left)
			{
				NullCount++;
				VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;
			}
			GraphVizGetIds(nodePtr->right, VizOut);
			if (!nodePtr->right)
			{
				NullCount++;
				VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;
			}
		}
	}
	void GraphVizMakeConnections(Node *nodePtr, ofstream &VizOut)
	{
		static int NullCount = 0;
		if (nodePtr)
		{
			GraphVizMakeConnections(nodePtr->left, VizOut);
			if (nodePtr->left)
				VizOut << "node" << nodePtr->data << "->"
				<< "node" << nodePtr->left->data << endl;
			else
			{
				NullCount++;
				VizOut << "node" << nodePtr->data << "->"
					<< "nnode" << NullCount << endl;
			}

			if (nodePtr->right)
				VizOut << "node" << nodePtr->data << "->"
				<< "node" << nodePtr->right->data << endl;
			else
			{
				NullCount++;
				VizOut << "node" << nodePtr->data << "->"
					<< "nnode" << NullCount << endl;
			}

			GraphVizMakeConnections(nodePtr->right, VizOut);
		}
	}
	void GraphVizOut(string filename)
	{
		ofstream VizOut;
		VizOut.open(filename);
		VizOut << "Digraph G {\n";
		GraphVizGetIds(root, VizOut);
		GraphVizMakeConnections(root, VizOut);
		VizOut << "}\n";
		VizOut.close();
	}	
	
	// This function will calcuate the mid number and take it as a root among
	// all the numbers. The send the smaller number to left and bigger number 
	// to the right side. 
	int calcMid(int listLength)
	{
		if (listLength <= 4)
		{
			return listLength / 2;
		}
		int levelNodesCount = 1;
		int totalNodes = 1;
		while (totalNodes < listLength)
		{
			levelNodesCount *= 2;
			totalNodes += levelNodesCount;
		}
		int excess = listLength - (totalNodes - levelNodesCount);
		int minMid = (totalNodes - levelNodesCount + 1) / 2;
		if (excess <= levelNodesCount / 2)
		{
			return minMid + (excess - 1);
		}
		else
		{
			int midExcess = levelNodesCount / 2;
			return minMid + (midExcess - 1);
		}
	}

};

int main()
{
	// read array
	vector<int>data;
	int temp;
	// read the number from the file "input.dat"
	ifstream input("input.dat");
	while (input >> temp) {
		data.push_back(temp);
	}

	BTree btree;
	btree.ConstructCompleteBSTfromList(data);
	btree.GraphVizOut("before.txt");
}
