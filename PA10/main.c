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
	char * address;
	char * city;
	long int reviewloc;
} BusNode;

typedef struct inode {
	char * id;
	long int reviewloc;

	struct inode * left;
	struct inode * right;
} IdNode;

typedef struct znode {
	char * zip;
	IdNode * idroot;

	struct znode * left;
	struct znode * right;
} ZipNode;

typedef struct snode {
	char * state;
	ZipNode * ziproot;

	struct snode * left;
	struct snode * right;
} StateNode;

typedef struct YelpDataBST {
	char * name;
	StateNode * stroot; 

	struct YelpDataBST * left;
	struct YelpDataBST * right;
} NameNode;

//char * strupr(char *str);//
void print_zip(ZipNode * zip);
void print_tree(struct YelpDataBST * tree);
void print_state(StateNode * st);
//NameNode * searchname(NameNode * nameroot, char * name);//
//StateNode * searchstate(StateNode * stroot, char * state);//
//ZipNode * searchzip(ZipNode * ziproot, char * zip);//
void print_id(IdNode * id);

//IdNode * searchid(IdNode * idroot, char * id);//
int main() 
{
	//NameNode * anamefo = NULL;
	//StateNode * astatefo = NULL;
	//ZipNode * azipfo = NULL;
	struct YelpDataBST * nameroot = NULL;
	struct Business * bus = NULL;
	//nameroot = create_business_bst("/home/shay/a/ece264p0/share/yelp_data/businesses.tsv","/home/shay/a/ece264p0/share/yelp_data/reviews.tsv");
	nameroot = create_business_bst("/home/shay/a/ece264p0/share/yelp_data/businesses.tsv","reviews.tsv");
	//anamefo = searchname(nameroot, "RUBY'S DINER");
	//print_tree(nameroot);
	//print_state(anamefo->stroot);
	//astatefo = (anamefo == NULL) ? NULL : searchstate(anamefo->stroot,"AZ");
	//print_zip(astatefo->ziproot);
	//azipfo = (astatefo == NULL) ? NULL : searchzip(astatefo->ziproot,"85226");
	//if (azipfo != NULL) print_id(azipfo->idroot);
	char * name  = "walmart";
	char * state = NULL;
	char * zip_code = "";
	bus = get_business_reviews(nameroot, name, state, zip_code);
	
	return 0;
}

void print_tree(struct YelpDataBST * tree)
{
	if (tree == NULL) return;
	print_tree(tree->left);
	printf("tree->name= %s\n",tree->name);
	print_tree(tree->right);
}

void print_state(StateNode * st)
{
	if (st == NULL) return;
	print_state(st->left);
	printf("state = %s\n",st->state);
	print_state(st->right);
}

void print_zip(ZipNode * zip)
{
	if (zip == NULL) return;
	print_zip(zip->left);
	printf("zip = %s\n",zip->zip);
	print_zip(zip->right);
}

void print_id(IdNode * id)
{
	if (id == NULL) return;
	print_id(id->left);
	printf("id = %s reviewloc = %ld\n",id->id,id->reviewloc);
	print_id(id->right);
}
/*
ZipNode * searchzip(ZipNode * ziproot, char * zip)
{
	if (ziproot == NULL) return NULL;
	if (strcmp(zip,ziproot->zip) == 0) {printf("found!\n"); return ziproot;}
	if (strcmp(zip,ziproot->zip) < 0) return searchzip(ziproot->left, zip);
	return searchzip(ziproot->right, zip);
}
	
StateNode * searchstate(StateNode * stroot, char * state)
{
	if (stroot == NULL) return NULL;
	if (strcmp(state,stroot->state) == 0) {printf("found!\n"); return stroot;}
	if (strcmp(state,stroot->state) < 0) return searchstate(stroot->left, state);
	return searchstate(stroot->right, state);
}

NameNode * searchname(NameNode * nameroot, char * name)
{
	if (nameroot == NULL) return NULL;
	if (strcmp(name,strupr(nameroot->name)) == 0) {printf("found!\n");  return nameroot;}
	if (strcmp(name,strupr(nameroot->name)) < 0) return searchname(nameroot->left, name);
	return searchname(nameroot->right, name);
}



char * strupr(char *str)
{
		char *s;

			for(s = str; *s; s++)
						*s = toupper(*s);
							return str;
}
*/
