// btree.h

#ifndef BTREE_H
#define BTREE_H

template <class keyType>
class BTree;

#include "btnode.h"
#include "recfile.h"
#include "fixfld.h"
#include "indbuff.h"
#include <iostream>
#include <fstream>

// btree needs to be able to pack, unpack, read and
// 	write index records
// 	also, handle the insert, delete, split, merge,
//	growth of the tree
//	a pool of nodes will be managed
//	keep at least a branch of the tree in memory
//	

template <class keyType>
class BTree
// this is the full version of the BTree
{public:
	BTree(int order, int keySize = sizeof(keyType), int unique = 1);
	~BTree();
	int Open (char * name, int mode);
	int Create (char * name, int mode);
	int Close ();
	int Insert (const keyType key, const int recAddr);
	int Remove (const keyType key, const int recAddr = -1);
	int Search (const keyType key, const int recAddr = -1);
	void Print (ostream &);
	void Print (ostream &, int nodeAddr, int level);
protected:
	typedef BTreeNode<keyType> BTNode;// useful shorthand
	BTNode * FindLeaf (const keyType key);
	// load a branch into memory down to the leaf with key
	BTNode * NewNode ();
	BTNode * Fetch(const int recaddr);
	int Store (BTNode *);
	int Discard(BTNode *);

	int Merge(BTNode* leftNode, BTNode* rightNode, BTNode* parentNode);
	int Distribute(BTNode* leftNode, BTNode* rightNode, BTNode* parentNode);

	BTNode Root;
	int Height; // height of tree
	int Order; // order of tree
	int PoolSize;
	BTNode ** Nodes; // pool of available nodes
	// Nodes[1] is level 1, etc. (see FindLeaf)
	// Nodes[Height-1] is leaf
	FixedFieldBuffer Buffer;
	RecordFile<BTNode> BTreeFile;	
};



const int MaxHeight = 5;
template <class keyType>
BTree<keyType>::BTree(int order, int keySize, int unique)
	: Buffer(1 + 2 * order, sizeof(int) + order * keySize + order * sizeof(int)),
	BTreeFile(Buffer), Root(order)
{
	Height = 1;
	Order = order;
	PoolSize = MaxHeight * 2;
	Nodes = new BTNode *[PoolSize];
	BTNode::InitBuffer(Buffer, order);
	Nodes[0] = &Root;
}

template <class keyType>
BTree<keyType>::~BTree()
{
	Close();
	delete Nodes;
}

template <class keyType>
int BTree<keyType>::Open(char * name, int mode)
{
	int result;
	result = BTreeFile.Open(name, mode);
	if (!result) return result;
	// load root
	BTreeFile.Read(Root);
	Height = 1; // find height from BTreeFile!
	return 1;
}

template <class keyType>
int BTree<keyType>::Create(char * name, int mode)
{
	int result;
	result = BTreeFile.Create(name, mode);
	if (!result) return result;
	// append root node
	result = BTreeFile.Write(Root);
	Root.RecAddr = result;
	return result != -1;
}

template <class keyType>
int BTree<keyType>::Close()
{
	int result;
	result = BTreeFile.Rewind();
	if (!result) return result;
	result = BTreeFile.Write(Root);
	if (result == -1) return 0;
	return BTreeFile.Close();
}


template <class keyType>
int BTree<keyType>::Insert(const keyType key, const int recAddr)
{
	int result; int level = Height - 1;
	int newLargest = 0; keyType prevKey, largestKey;
	BTNode * thisNode, *newNode = nullptr, *parentNode;
	thisNode = FindLeaf(key);

	// test for special case of new largest key in tree
	if (key > thisNode->LargestKey())
	{
		newLargest = 1; prevKey = thisNode->LargestKey();
	}

	result = thisNode->Insert(key, recAddr);

	// handle special case of new largest key in tree
	if (newLargest)
		for (int i = 0; i<Height - 1; i++)
		{
			Nodes[i]->UpdateKey(prevKey, key);
			if (i>0) Store(Nodes[i]);
		}

	while (result == -1) // if overflow and not root
	{
		//remember the largest key
		largestKey = thisNode->LargestKey();
		// split the node
		newNode = NewNode();
		thisNode->Split(newNode);
		Store(thisNode); Store(newNode);
		level--; // go up to parent level
		if (level < 0) break;
		// insert newNode into parent of thisNode
		parentNode = Nodes[level];
		result = parentNode->UpdateKey
		(largestKey, thisNode->LargestKey());
		result = parentNode->Insert
		(newNode->LargestKey(), newNode->RecAddr);
		thisNode = parentNode;
	}
	Store(thisNode);
	if (level >= 0) return 1;// insert complete
							 // else we just split the root
	int newAddr = BTreeFile.Append(Root); // put previous root into file
										  // insert 2 keys in new root node
	Root.Keys[0] = thisNode->LargestKey();
	Root.RecAddrs[0] = newAddr;
	Root.Keys[1] = newNode->LargestKey();
	Root.RecAddrs[1] = newNode->RecAddr;
	Root.NumKeys = 2;
	Height++;
	return 1;
}

template <class keyType>
int BTree<keyType>::Remove(const keyType key, const int recAddr)
{
	// left for exercise
	int result; //���� �� �����.
	
	int level = Height - 1; // ���� �����ϰ� �ִ� Node�� depth
	
	bool is_newlargest = false; // ���� Node�� largest key�� �����Ǿ� ���� ���ϴ� ��� true.
	keyType largestKey;

	/* �ʿ��� Node ���  */
	BTNode* thisNode; //���� �����ϰ� �ִ� Node
	int thisIndex; //�θ� Node���� ���� �����ϰ� �ִ� Node�� ���� index 
	BTNode* newNode = nullptr; //���� �� ���� �����Ǵ� ���
	BTNode* leftNode = nullptr; // ���� depth���� �̿��ϰ� �ִ� ���� ���
	BTNode* rightNode = nullptr; // ���� depth���� �̿��ϰ� �ִ� ������ ���
	BTNode* parentNode; //���� �����ϰ� �ִ� Node�� �θ� ���

	//1. LeafNode�� ã�´�.
	thisNode = FindLeaf(key);

	//2. �����ϰ��� �ϴ� key�� leaf�� �� ���� �ִ��� Ȯ���Ѵ�.
	//(�� ���� ���� �� ���� Node�� internal Node�� �ȴ�.)
	if (key == thisNode->LargestKey())
		is_newlargest = true;

	//3. ���� Node���� key�� �����Ѵ�.
	result = thisNode->Remove(key, recAddr);

	//4. ���� Node���� key�� ������ �����Ѵ�.
	if (result == 0) 
		return -1;

	//5. ���� Node�� Root�� �����Ѵ�.
	if (level == 0)
		return 1;

	//6. ���� Key�� internal Node������ �θ� ������ �����Ѵ�.
	if (is_newlargest)
	{
		largestKey = thisNode->LargestKey();
		for (int i = 0; i < Height - 1; i++)
		{
			Nodes[i]->UpdateKey(key, largestKey);
			if (i > 0) 
				if(Store(Nodes[i]) == -1) 
					return -1;
		}
	}

	//7. ���� Node�� Root Node�� �ƴϰ�, Underflow�� �߻����� ���
	while (result == -1)
	{
		parentNode = Nodes[level - 1];
		//exact find�� ���� Node�� ���� index�� ã�´�.
		thisIndex = parentNode->Find(thisNode->LargestKey(), -1, 1);
		if (thisIndex == -1)
			return -1;

		leftNode = rightNode = nullptr;
		// ���� Node�� ���� Node�� �����´�.
		if(0 <= thisIndex - 1 )
			leftNode = Fetch(parentNode->RecAddrs[thisIndex -1]);
		// ���� Node�� ������ Node�� �����´�.
		if (thisIndex + 1 < parentNode->numKeys())
			rightNode = Fetch(parentNode->RecAddrs[thisIndex + 1]);

		// ���� ���� ���� ��带 �պ� �õ��Ѵ�.
		result = Merge(leftNode, thisNode, parentNode);
		if (result == 1)
			break;
		else if (result == -1)
		{
			thisNode = Nodes[--level];
			//�θ� ��� ������ ����.
			if (level - 1 < 0)
				break;
			continue;
		}
		// ���н� ������ ���� ���� ��带 �պ� �õ��Ѵ�.
		result = Merge(thisNode, rightNode, parentNode);
		if (result == 1)
			break;
		else if (result == -1)
		{
			thisNode = Nodes[--level];
			//�θ� ��� ������ ����.
			if (level - 1 < 0)
				break;
			continue;
		}
		
		//���н� ���� ���� ��й� �õ��Ѵ�.
		result = Distribute(leftNode, thisNode, parentNode);
		//������ ����
		if (result == 1) 
			break;

		//���н� ������ ���� ��й� �õ��Ѵ�.
		result = Distribute(thisNode, rightNode, parentNode);
		//������ ����
		if (result == 1)
			break;
	}
	if(Store(thisNode) == -1)
		return -1;

	// Remove �� (Root Node���� �������� ����)
	if (level > 0) return 1;

	// Root�� key�� �ϳ��� �����Ѵٸ�, ��ü depth�� �������Ѵ�.
	if (Root.numKeys() == 1)
	{
		//Root�� �ڽ� ��带 ��´�.
		thisNode = Fetch(thisNode->RecAddrs[0]);
		//Root�� �ڽ� ��带 Root�� �����Ѵ�.
		Root.NumKeys = thisNode->numKeys();
		for (int i = 0; i < thisNode->numKeys(); i++)
		{
			Root.Keys[i] = thisNode->Keys[i];
			Root.RecAddrs[i] = thisNode->RecAddrs[i];
		}
		//�ڽ� Node�� ���Ͽ��� �����Ѵ�.
		if(Discard(thisNode) == -1)
			return -1;
		Height--;
		return 1;
	}

	return -1;
}

template <class keyType>
int BTree<keyType>::Search(const keyType key, const int recAddr)
{
	BTNode * leafNode;
	leafNode = FindLeaf(key);
	return leafNode->Search(key, recAddr);
}

template <class keyType>
void BTree<keyType>::Print(ostream & stream)
{
	stream << "BTree of height " << Height << " is " << endl;
	Root.Print(stream);
	if (Height>1)
		for (int i = 0; i<Root.numKeys(); i++)
		{
			Print(stream, Root.RecAddrs[i], 2);
		}
	stream << "end of BTree" << endl;
}

template <class keyType>
void BTree<keyType>::Print
(ostream & stream, int nodeAddr, int level)
{
	BTNode * thisNode = Fetch(nodeAddr);
	stream << "Node at level " << level << " address " << nodeAddr << ' ';
	thisNode->Print(stream);
	if (Height>level)
	{
		level++;
		for (int i = 0; i<thisNode->numKeys(); i++)
		{
			Print(stream, thisNode->RecAddrs[i], level);
		}
		stream << "end of level " << level << endl;
	}
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::FindLeaf(const keyType key)
// load a branch into memory down to the leaf with key
{
	int recAddr, level;
	for (level = 1; level < Height; level++)
	{
		recAddr = Nodes[level - 1]->Search(key, -1, 0);//inexact search
		Nodes[level] = Fetch(recAddr);
	}
	return Nodes[level - 1];
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::NewNode()
{// create a fresh node, insert into tree and set RecAddr member
	BTNode * newNode = new BTNode(Order);
	int recAddr = BTreeFile.Append(*newNode);
	newNode->RecAddr = recAddr;
	return newNode;
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::Fetch(const int recaddr)
{// load this node from File into a new BTreeNode
	int result;
	BTNode * newNode = new BTNode(Order);
	result = BTreeFile.Read(*newNode, recaddr);
	if (result == -1) return NULL;
	newNode->RecAddr = result;
	return newNode;
}

template <class keyType>
int BTree<keyType>::Store(BTreeNode<keyType> * thisNode)
{
	return BTreeFile.Write(*thisNode, thisNode->RecAddr);
}
template <class keyType>
int BTree<keyType>::Discard(BTreeNode<keyType> * thisNode)
{
	return BTreeFile.Remove(*thisNode, thisNode->RecAddr);
}

template <class keyType>
int  BTree<keyType>::Merge(BTNode* leftNode, BTNode* rightNode, BTNode* parentNode)
{
	if (leftNode == nullptr || rightNode == nullptr)
		return 0;

	// Return ��
	// -1 : ����������, �θ� Node�� Underflow
	// 0 : ����
	// 1 : ����
	int result;

	//�պ�, ���� ������ ���ʰ� �������� �� ������ Ű
	keyType prev_leftkey;
	keyType prev_rightkey;
	BTNode* removedNode;

	//������ �θ� Node���� index�� �Ǵ� key���� �����ϰ�,
	// �� ��带 �պ���.
	prev_leftkey = leftNode->LargestKey();
	prev_rightkey = rightNode->LargestKey();
	result = leftNode->Merge(rightNode);

	if (result == 1)
	{
		//  ���� Node�� �ش��ϴ� �θ� Node�� Key�� ����
		if (parentNode->Remove(prev_leftkey) == 0)
			return -1;
		// ������ Node�� �ش��ϴ� �θ� Node�� Key�� ����
		if (parentNode->Remove(prev_rightkey) == 0)
			return -1;
		// �պ��� Node�� �ش��ϴ� �θ� Node�� Key�� �߰�
		result = parentNode->Insert(leftNode->LargestKey(), leftNode->RecAddr);
		// �θ� Node�� Underflow�� ���, underflow�� üũ���ش�.
		if (result == 1 && parentNode->underflow())
			result = -1;
		switch(result)
		{
			// �������� ���, ���Ͽ� �ݿ��Ѵ�.
		case 1:
		case -1:
			removedNode = rightNode;
			if(Discard(removedNode) == -1)
				return 0;
			if(Store(leftNode) == -1)
				return 0;
			if(Store(parentNode) == -1)
				return 0;
			break;
		}
		return result;
	}
	return 0;
}

template <class keyType>
int  BTree<keyType>::Distribute(BTNode* leftNode, BTNode* rightNode, BTNode* parentNode)
{
	if (leftNode == nullptr || rightNode == nullptr)
		return 0;

	// Return ��
	// 0 : ����
	// 1 : ����
	int result;

	//�պ�, ���� ������ ���ʰ� �������� �� ������ Ű
	keyType prev_leftkey;

	//������ �θ� Node���� index�� �Ǵ� key���� �����ϰ�,
	// �� ��带 ��й���.
	prev_leftkey = leftNode->LargestKey();
	result = leftNode->Distribute(rightNode);
	if (result == 1)
	{
		//�������� ���, �θ� Node���� left�ʿ� �ش��ϴ� key�� �ٲ�����Ƿ�, �������ش�.
		result = parentNode->UpdateKey(prev_leftkey, leftNode->LargestKey());
		// �������� ���, ���Ͽ� �ݿ��Ѵ�.
		if (result)
		{
			if(Store(leftNode) == -1)
				return 0;
			if(Store(rightNode) == -1)
				return 0;
			if(Store(parentNode) == -1)
				return 0;
			return 1;
		}
		else
			return 0;
	}
	return 0;
}
#endif
