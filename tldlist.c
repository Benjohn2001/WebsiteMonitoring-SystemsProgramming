//Ben Johnston - 2432411j - SP Exercise 1a
//This is my own work as defined in the Academic Ethics agreement I have signed.
//tld - Top Level Domain

#include<stdlib.h>
#include<stdio.h>
#include"tldlist.h"
#include<string.h>
#include"date.h"
#include<ctype.h>

//Defining structs
struct tldlist{struct tldnode *root;Date *begin;Date *end;int inserts;};
struct tldnode{char *tld;int count;struct tldnode *left,*right,*parent;};
struct tlditerator{struct tldnode *currentNode;};

//Creating the tldlist with begin and end dates stored along with a root  node
//and inserts to store how many log entries are in list 
//Malloc failures will return NULL 
//Returns the tldlist  
TLDList *tldlist_create(Date *begin, Date *end){
	TLDList* l = (TLDList*) malloc(sizeof(TLDList));
	if(!l){
                return NULL;
        }
	l->root=NULL;
	l->begin=date_duplicate(begin);
	l->end=date_duplicate(end);
	l->inserts=0;
	return l;
}

//Post Order traversal to free the tld name from the node and then free the
//node itself until all nodes are removed from the list
void postOrderDestroy(TLDNode *n){
	if(n!=NULL){
		postOrderDestroy(n->left);
		postOrderDestroy(n->right);
		free(n->tld);
		free(n);
	}
}

//Calls post order destroy method to free all nodes from the tldlist
//Date destroy is called to free the begin and end dates 
//After all else is free'd, remove the tldlist itself
void tldlist_destroy(TLDList *tld){
	postOrderDestroy(tld->root);
	date_destroy(tld->begin);
	date_destroy(tld->end);
	free(tld);
}

//Creates a node given a hostname and a parent node
//Malloc failure will return NULL
//Tld of node is set to hostname, left and right children set  to NULL
//Parent of node is set to the parent node and count set to 1
//Returns the new node created
TLDNode *node_create(char *hostname,TLDNode *parent){
	TLDNode* n = (TLDNode*) malloc(sizeof(TLDNode));
	if(!n){ 
                return NULL;
        }
	n->tld = malloc(strlen(hostname)+1);
	if(!n->tld){ 
                return NULL;
        }
	strcpy(n->tld, hostname);
	n->left=NULL;
	n->right=NULL;
	n->parent=parent;
	n->count=1;
	return n;
}

//Method to recursively find where to insert the tld into the list
//If no root exists or if we have found where to insert the tld we set 
//this to the result of node create
//We compare the current string alphabetically with the new tld
//to find out whether to go left or right or if the tld is already
//in the list we just update the count for this tld, return root
TLDNode *insertRec(TLDNode* root, char *domain, TLDNode *parent){	
	if(root==NULL){
		root=node_create(domain,parent);
	}
	else if(strcmp(root->tld,domain)<0){
		parent=root;
		root->left=insertRec(root->left,domain,parent);
	}
	else if(strcmp(root->tld,domain)==0) {
		root->count=root->count+1;
	}
	else if(strcmp(root->tld,domain)>0){
		parent=root;
		root->right=insertRec(root->right,domain,parent);
	}
	return root;
}

//Check date of tld is within the date boundaries using date compare method
//Finds tld and makes sure it is alphabetic and converts it to lowercase 
//Adds end character '\0', then calls the insert recursive function to 
//add the tld to the list, increments the lists inserts by 1
//Returns 1 if insert succesfull and 0 if failed 
int tldlist_add(TLDList *tld, char *hostname,Date *d){
	if(date_compare(d,tld->begin)>=0 && date_compare(d,tld->end)<=0){
		int hostLen=strlen(hostname);
                int i=hostLen;
                for(; hostname[i] != '.' ; i--);
		int tldLen=(hostLen-1)-i;
		char domain[tldLen];
                for(int j=i; j<hostLen-1;j++){
			if(!(isalpha(hostname[j+1])))return 0;
			char lower=tolower(hostname[j+1]);
			domain[j-i]=lower;
                } 
                domain[tldLen]='\0';
		tld->root=insertRec(tld->root,domain,NULL);			
		tld->inserts=tld->inserts+1;
		return 1;
	}
	return 0;	
}

//Returns the ammount of inserts succesfully made into the tldlist 
long tldlist_count(TLDList *tld){
	return tld->inserts; 
}

//Creates iterator for a tldlist, if Malloc fails, return NULL
//Traverses the tree structure to find the left-most node and
//sets the iterators current node to  the result, returns the iterator
TLDIterator *tldlist_iter_create(TLDList *tld){
	TLDIterator* iter = (TLDIterator*) malloc(sizeof(TLDIterator));
	if(!iter){
                return NULL;
        }
	TLDNode *temp =tld->root;
	while(temp->left !=NULL){
		temp=temp->left;
	}
	iter->currentNode=temp;
	return iter;
}

//Find next node with iterator, if we reach the end return NULL
//Check if there  is  a right node, then if this right node has a left node
//Once this node has been found update current node and return the next node
//Else move up the tree to the parent or find the parents left node
//Return the next node from iterator
TLDNode *tldlist_iter_next(TLDIterator *iter){
	TLDNode *temp =iter->currentNode;
	TLDNode *temp2 =iter->currentNode;
	if(temp==NULL){
		return NULL;
	}
	if(temp->right != NULL){
		temp=temp->right;
		while(temp->left != NULL){
			temp=temp->left;
		}
		iter->currentNode=temp;
		return temp2;
	}
	else{
		while(temp->parent !=NULL){
			if(temp->parent->left ==temp){ 
				temp=temp->parent;
				iter->currentNode=temp;
				return temp2;
			}
		temp=temp->parent;
		}
	}
	iter->currentNode=NULL;
	return temp2;
}

//Free's the iterator
void tldlist_iter_destroy(TLDIterator *iter){
	free(iter);
}

//Return the tld for a node, or NULL if node is NULL
char *tldnode_tldname(TLDNode *node){
	if(node==NULL){
		return NULL;
	}
	return node->tld;
}

//Return the count of a tld given a node, 0 if NULL node
long tldnode_count(TLDNode *node){
	if (node==NULL){
		return 0;
	}
	return  node->count;	
}

