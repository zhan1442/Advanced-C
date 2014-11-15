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
	char * zip;
	char * id;
	//reviewlocation

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

char * strupr(char *str);
char * * explode(const char * str, const char * delims);
void destroyStringArray(char * * strArr, int len);
NameNode * insert_name(BusNode bus, NameNode * nameroot);
NameNode * create_namenode(BusNode bus);
StateNode * create_statenode(BusNode bus);
StateNode * insert_st(BusNode bus, StateNode * stroot);
ZipNode * insert_zip(BusNode bus, ZipNode * ziproot);
ZipNode * create_zipnode(BusNode bus);

struct YelpDataBST* create_business_bst(const char* businesses_path, const char* reviews_path)
{
	FILE * fp = NULL;
	int err = 0;
	NameNode * nameroot = NULL;
	BusNode bus;

	fp = fopen(businesses_path, "r");
	if(!fp) {
		fprintf(stderr,"Failed to open file '%s'\n", businesses_path);
		err = 1;
	}
	if(!err) {
		char * str = (char *) malloc(2000 * sizeof(char));
		char * * strarr = NULL;
		while (!feof(fp)) {
			fgets(str,2000,fp);
			//printf("str = %s",str);
			strarr = explode(str,"\t");
			bus.id = strarr[0];
			bus.name = strarr[1];
			bus.state = strarr[4];
			bus.zip = strarr[5];
			//printf("bus.id = %s \nbus.name = %s \nbus.state = %s \nbus.zip = %s\n\n",bus.id, bus.name, bus.state, bus.zip);
			nameroot = insert_name(bus, nameroot);
			destroyStringArray(strarr, 7);	
		}
		free(str);
		fclose(fp);
	}
	return nameroot;
}

NameNode * create_namenode(BusNode bus) //malloc
{
	NameNode * nameno = malloc(sizeof(NameNode));////
	nameno->name = strdup(bus.name);////
	nameno->stroot = NULL;//very important!!!!!!
	nameno->stroot = insert_st(bus, nameno->stroot);
	//printf("%s\n",(nameno->stroot)->state);
	nameno->left = NULL;
	nameno->right = NULL;
	return nameno;
}

StateNode * create_statenode(BusNode bus) //malloc
{
	StateNode * stateno = malloc(sizeof(StateNode));////
	stateno->state = strdup(bus.state);////
	stateno->ziproot = NULL;//very important!!!!!!
	stateno->ziproot = insert_zip(bus, stateno->ziproot);
	stateno->left = NULL;
	stateno->right = NULL;
	return stateno;
}

ZipNode * create_zipnode(BusNode bus) //malloc
{
	ZipNode * zipno = malloc(sizeof(ZipNode));////
	zipno->zip = strdup(bus.zip);////
	zipno->id = strdup(bus.id);////

	zipno->left = NULL;
	zipno->right = NULL;
	return zipno;
}


NameNode * insert_name(BusNode bus, NameNode * nameroot)
{
	if (nameroot == NULL) {
		return create_namenode(bus);
	}
	if (strcmp(strupr(bus.name),strupr(nameroot->name)) < 0) {
		nameroot->left = insert_name(bus, nameroot->left);
	}
	else if (strcmp(strupr(bus.name),strupr(nameroot->name)) > 0) {
		nameroot->right = insert_name(bus, nameroot->right);
	}
	else {
		nameroot->stroot = insert_st(bus, nameroot->stroot);
	}
	return nameroot;
}

ZipNode * insert_zip(BusNode bus, ZipNode * ziproot)
{
	if (ziproot == NULL) {
		return create_zipnode(bus);
	}
	if (strcmp(strupr(bus.zip),strupr(ziproot->zip)) < 0) {
		ziproot->left = insert_zip(bus, ziproot->left);
	}
	else if (strcmp(strupr(bus.zip),strupr(ziproot->zip)) > 0) {
		ziproot->right = insert_zip(bus, ziproot->right);
	}
	else {
		ziproot->id = strdup(bus.id);////
		printf("name = %s state = %s zip = %s id = %s\n",bus.name, bus.state, bus.zip, ziproot->id);
	}
	return ziproot;
}

StateNode * insert_st(BusNode bus, StateNode * stroot)
{
	if (stroot == NULL) {
		return create_statenode(bus);
	}
	//printf("bus.state = %s ",bus.state);
	//printf("stroot->state = %s\n",stroot->state);
	if (strcmp(strupr(bus.state),strupr(stroot->state)) < 0) {
		stroot->left = insert_st(bus, stroot->left);
	}
	else if (strcmp(strupr(bus.state),strupr(stroot->state)) > 0) {
		stroot->right = insert_st(bus, stroot->right);
	}
	else {
		stroot->ziproot = insert_zip(bus, stroot->ziproot);
	}
	return stroot;
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
char * strupr(char *str)
{
	char *s;

	for(s = str; *s; s++)
		*s = toupper((unsigned char)*s);
	return str;
}


