#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"answer09.h"

char * * explode(const char * str, const char * delims);
void destroyStringArray(char * * strArr, int len);

BusinessNode * create_node(char * stars, char * name, char * address)
{
	BusinessNode * node = malloc(sizeof(BusinessNode));
	node->stars = stars;
	node->name = name;
	node->address = address;

	node->left = NULL;
	node->right = NULL;

	return node;
}

BusinessNode * tree_insert(BusinessNode * node, BusinessNode * root)
{
	if (root == NULL) {
		return node;
	}
	if (strcmp(node->name,root->name) <= 0) {
		root->left = tree_insert(node, root->left);
	}
	else {
		root->right = tree_insert(node, root->right);
	}
	return root;
}

BusinessNode * load_tree_from_file(char * filename)
{
	FILE * fp = NULL;
	int err = 0;
	BusinessNode * node = NULL;
	BusinessNode * root = NULL;

	fp = fopen(filename, "r");
	if(!fp) {
		fprintf(stderr,"Failed to open file '%s'\n", filename);
		err = 1;
	}

	if(!err) {
		char * str = (char *) malloc(2000 * sizeof(char));
		char * * strarr = NULL;
		while (!feof(fp)) {
			fgets(str,2000,fp);
			if (!feof(fp)) {
				//printf("str = %s\n",str);
				strarr = explode(str,"\t");
				node = create_node(strarr[0], strarr[1], strarr[2]);
				root = tree_insert(node, root);
				free(strarr);
			}
		}
		free(str);
		fclose(fp);
	}
	return root;
}

BusinessNode * tree_search_name(char * name, BusinessNode * root)
{
	if (root == NULL) return NULL;
	if (strcmp(name,root->name) == 0) return root;
	if (strcmp(name,root->name) < 0) return tree_search_name(name, root->left);
	return tree_search_name(name, root->right);
}

void print_tree(BusinessNode * tree)
{
	if (tree == NULL) return;
	printf("tree->name= %s, tree->stars= %s, tree->address= %s\n",tree->name,tree->stars,tree->address);
	print_tree(tree->left);
	print_tree(tree->right);
}

char * * explode(const char * str, const char * delims)
{
	int N = 0;
	int i;
	int arrind = 0;
	int last = 0;

	for (i = 0; i < strlen(str); i++) {
		if (strchr(delims, str[i]) != NULL) {
			N++;
		}
	}		
	char * * strarr = (char * *) malloc(sizeof(char *) * (N + 1));
	for (i = 0; i < (int) strlen(str); i++) {
		if (strchr(delims, str[i]) != NULL) {
			strarr[arrind] = (char *) malloc(sizeof(char)*(i - last + 1));
			memcpy(strarr[arrind], str + last, i - last);
			strarr[arrind][i-last] = '\0';
			last = i + 1;
			arrind++;
		}
	}

	strarr[arrind] = (char *) malloc(sizeof(char)*(i - last + 1));
	memcpy(strarr[arrind], str + last, i - last);
	strarr[arrind][i-last] = '\0';

	return(strarr);

}
void destroyStringArray(char * * strArr, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		free(strArr[i]);
	}
	free(strArr);
}

void destroy_tree(BusinessNode * root)
{ 
	if(root == NULL) return;
    destroy_tree(root->left);
	destroy_tree(root->right);
	free(root->name);
	free(root->stars);
	free(root->address);
	free(root);
}
