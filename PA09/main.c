#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"answer09.h"

int main()
{
	BusinessNode * tree;
	tree = load_tree_from_file("shortfile.tsv");
	print_tree(tree);
	destroy_tree(tree);
	return 0;
}
