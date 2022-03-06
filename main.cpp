#include <iostream>
#include <fstream>
#include <stdlib.h>

class HtreeNode{
	public:
	std::string chStr;
	int prob;
	std::string code;
	HtreeNode* left;
	HtreeNode* right;
	HtreeNode* next;
	
	HtreeNode(std::string chStr, int prob, std::string code, HtreeNode* left, HtreeNode* right, HtreeNode* next){
		this->chStr = chStr;
		this->prob = prob;
		this->code = code;
		this->left = left;
		this->right = right;
		this->next = next;
	}
	
	std::string printNode(){
		std::string pNext = "NULL";
		std::string pLeft = "NULL";
		std::string pRight = "NULL";
		
		if(this->next != nullptr){
			pNext = this->next->chStr;
		}
		if(this->left != nullptr){
			pLeft = this->left->chStr;
		}
		if(this->right != nullptr){
			pRight = this->right->chStr;
		}
		
		std::string result = "("+this->chStr+", "+std::to_string(this->prob)+", "+this->code+", "+pNext+", "+pLeft+", "+pRight+")";
		return result;
	}
};

class HuffmanBinaryTree{
	public:
	HtreeNode* listHead;
	HtreeNode* root;
	
	HuffmanBinaryTree(){
		this->listHead = new HtreeNode("HEAD",0,"",nullptr,nullptr,nullptr);
		this->root = new HtreeNode("ROOT",0,"",nullptr,nullptr,nullptr);
	}
	
	HtreeNode* findSpot(int prob){
		HtreeNode* spot = this->listHead;
		while(spot->next != nullptr && spot->next->prob < prob){
			spot = spot->next;
		}
		return spot;
	}
	
	void listInsert(HtreeNode* spot,HtreeNode* newNode){
		newNode->next = spot->next;
		spot->next = newNode;
	}
	
	HtreeNode* constructHuffmanLList(std::ifstream& inFile, std::ofstream& outFile){
		std::string chr;
		int prob;
		while(inFile >> chr){
			inFile >> prob;
			HtreeNode* newNode = new HtreeNode(chr, prob, "", nullptr, nullptr, nullptr);
			HtreeNode* spot = this->findSpot(prob);
			this->listInsert(spot, newNode);
			this->printList(outFile);
		}
		return this->listHead;
	}
	HtreeNode* constructHuffmanBinTree(std::ofstream& outFile){
		while(this->listHead->next->next != nullptr){
			HtreeNode* newNode = new HtreeNode("",0,"",nullptr,nullptr,nullptr);
			newNode->prob = this->listHead->next->prob + this->listHead->next->next->prob;
			newNode->chStr = this->listHead->next->chStr + this->listHead->next->next->chStr;
			newNode->left = this->listHead->next;
			newNode->right = this->listHead->next->next;
			newNode->next = nullptr;
			
			HtreeNode* spot = findSpot(newNode->prob);
			this->listInsert(spot, newNode);
			this->listHead->next = this->listHead->next->next->next;
			this->printList(outFile);
		}
		return this->listHead->next;
	}
	
	void preOrderTraversal(HtreeNode* T, std::ofstream& outFile){
		if(isLeaf(T)){
			outFile<<T->printNode()<<std::endl;
		}
		else{
			outFile<<T->printNode()<<", ";
			preOrderTraversal(T->left, outFile);
			preOrderTraversal(T->right, outFile);
		}
	}
	void inOrderTraversal(HtreeNode* T, std::ofstream& outFile){
		if(isLeaf(T)){
			std::string result = T->printNode();
			outFile<<result<<std::endl;
		}
		else{
			inOrderTraversal(T->left, outFile);
			std::string result = T->printNode();
			outFile<<result<<", ";
			inOrderTraversal(T->right, outFile);
		}
	}
	void postOrderTraversal(HtreeNode* T, std::ofstream& outFile){
		if(isLeaf(T)){
			outFile<<T->printNode()<<std::endl;
		}
		else{
			postOrderTraversal(T->left, outFile);
			postOrderTraversal(T->right, outFile);
			outFile<<T->printNode()<<", ";
		}
	}
	void constructCharCode(HtreeNode* T, std::string code, std::ofstream& outFile){
		if(isLeaf(T)){
			T->code = code;
			outFile<<T->chStr<<", "<<T->code<<std::endl;
		}
		else{
			this->constructCharCode(T->left,code+"0",outFile);
			this->constructCharCode(T->right,code+"1",outFile);
		}
	}
	bool isLeaf(HtreeNode* node){
		if(node->left == nullptr && node->right == nullptr){
			return true;
		}
		return false;
	}
	void printList(std::ofstream& outFile){
		HtreeNode* printer = this->listHead;
		std::string result;
		while(printer != nullptr){
			result += printer->printNode() + "->";
			printer = printer->next;
		}
		outFile<<result<<std::endl;
	}
};

int main(int argc, char** argv){
	//check arguments correct
	if(argc != 4){
		std::cout<<"You must include 3 arguments (inFile, outFile1, outFile2)"<<std::endl;
		return -1;
	}
	//read input file and check if readable
	std::ifstream inFile(argv[1]);
	if(!inFile.good()){
		std::cout<<"Invalid input file, please check the spelling!"<<std::endl;
		return -1;
	}
	inFile.close();
	//init for out files
	std::ofstream outFile1;
	std::ofstream outFile2;
	
	//process data
	inFile.open(argv[1]);
	outFile1.open(argv[2],std::ios::out | std::ios::app);
	outFile2.open(argv[3],std::ios::out | std::ios::app);
	
	HuffmanBinaryTree* huff = new HuffmanBinaryTree();
	outFile2<<"Making Linked List"<<std::endl;
	HtreeNode* listHead = huff->constructHuffmanLList(inFile, outFile2);
	huff->printList(outFile1);
	outFile2<<"Making Bin Tree"<<std::endl;
	HtreeNode* root = huff->constructHuffmanBinTree(outFile2);
	huff->constructCharCode(root, "", outFile1);
	
	outFile1<<"Printing PreOrder"<<std::endl;
	huff->preOrderTraversal(root, outFile1);
	outFile1<<"Printing InOrder"<<std::endl;
	huff->inOrderTraversal(root, outFile1);
	outFile1<<"Printing PostOrder"<<std::endl;
	huff->postOrderTraversal(root, outFile1);
	
	//close all files
	inFile.close();
	outFile1.close();
	outFile2.close();
	
	return 0;
}