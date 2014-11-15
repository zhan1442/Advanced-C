#include <ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer10.h"

typedef struct busnode {
	char * id;
	char * name;
	char * state;
	char * zip;
} BusNode;

typedef struct znode {
	char * id;
	//reviewlocation

	struct znode * left;
	struct znode * right;
} ZipNode;

typedef struct snode {
	char * state;
	ZipNode * ziroot;

	struct snode * left;
	struct snode * right;
} StateNode;

typedef struct YelpDataBST {
	char * name;
	StateNode * stroot; 

	struct YelpDataBST * left;
	struct YelpDataBST * right;
} NameNode;

void print_tree(struct YelpDataBST * tree);

int main() 
{
	struct YelpDataBST * nameroot = NULL;
	nameroot = create_business_bst("/home/shay/a/ece264p0/share/yelp_data/businesses.tsv","reviews_path");
	//print_tree(nameroot);
	return 0;
}

void print_tree(struct YelpDataBST * tree)
{
	if (tree == NULL) return;
	print_tree(tree->left);
	printf("tree->name= %s\n",tree->name);
	print_tree(tree->right);
}
