#include <ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer10.h"

typedef struct YelpDataBST {
	char * name;
	char * id;
	long int boffset;
	long int roffset;
	const char * bpath;
	const char * rpath;

	struct YelpDataBST * left;
	struct YelpDataBST * right;
}Node;

void print_tree(struct YelpDataBST * tree);

int main()
{
	Node * root = NULL;
	struct Business * bus = NULL;	
	root = create_business_bst("/home/shay/a/ece264p0/share/yelp_data/businesses.short.tsv","/home/shay/a/ece264p0/share/yelp_data/reviews.short.tsv");
	//root = create_business_bst("/home/shay/a/ece264p0/share/yelp_data/businesses.tsv","/home/shay/a/ece264p0/share/yelp_data/reviews.tsv");
	//print_tree(root);

	char * name  = "starbucks";
	char * state = NULL;
	char * zip_code = NULL;
	bus = get_business_reviews(root, name, state, zip_code);
	destroy_business_bst(root);
	return 0;
}

void print_tree(struct YelpDataBST * tree)
{
	if (tree == NULL) return;
	print_tree(tree->left);
	printf("name= %s id= %s\n boff= %ld tree->roffset= %ld\n",tree->name,tree->id,tree->boffset,tree->roffset);
	print_tree(tree->right);
}





