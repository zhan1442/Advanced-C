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
	const char* reviews_path; //newly added
} BusNode;

typedef struct inode {
	char * id;
	char * address;
	char * city;
	long int reviewloc;
	const char* reviews_path;//newly added

	struct inode * left;
	struct inode * right;
} IdNode;

typedef struct znode {
	char * zip;
	IdNode * idroot; //keep eye on!!

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

char * * explode(const char * str, const char * delims);
void destroyStringArray(char * * strArr, int len);
NameNode * insert_name(BusNode bus, NameNode * nameroot);
NameNode * create_namenode(BusNode bus);
StateNode * create_statenode(BusNode bus);
StateNode * insert_st(BusNode bus, StateNode * stroot);
ZipNode * insert_zip(BusNode bus, ZipNode * ziproot);
ZipNode * create_zipnode(BusNode bus);
IdNode * create_idnode(BusNode bus);
IdNode * insert_id(BusNode bus, IdNode * idroot);

uint32_t addreview(char * id,long int reviewloc,struct Review * *reviews, uint32_t * num_reviews, const char* reviews_path);
void locidcount(IdNode * idroot, uint32_t * num_locations);
void loczipcount(ZipNode * ziproot, uint32_t * num_locations);
void loccount(StateNode * stfound, uint32_t * num_locations);
void createlocarr(StateNode * stfound, struct Location * locarr, int * ind);
void createloczp(ZipNode * ziproot, char * state, int * ind, struct Location * locarr);
void createlocip(IdNode * idroot, char * zip, char * state, int * ind, struct Location * locarr);

NameNode * searchname(NameNode * nameroot, char * name);
StateNode * searchstate(StateNode * stroot, char * state);

struct YelpDataBST* create_business_bst(const char* businesses_path, const char* reviews_path)
{
	FILE * fp = NULL;
	FILE * fpr = NULL;
	int err = 0;
	NameNode * nameroot = NULL;
	BusNode bus;

	fp = fopen(businesses_path, "r");
	fpr = fopen(reviews_path, "r");
	if(!fp) {
		fprintf(stderr,"Failed to open file '%s'\n", businesses_path);
		err = 1;
	}
	if (!fpr) {
		fprintf(stderr,"Failed to open file '%s'\n", reviews_path);
		err = 1;
	}
	if(!err) {
		char * str = (char *) malloc(2000 * sizeof(char));
		char * strrev = (char *) malloc(8000 * sizeof(char));
		char * * strarr = NULL;
		char * * strarrrev = NULL;
		int notf = 0;
		int loc = -1;
		while (!feof(fp)) {
			fgets(str,2000,fp);
			if (!feof(fp)) {
				strarr = explode(str,"\t");
				bus.id = strarr[0];
				bus.name = strarr[1];
				bus.address = strarr[2];
				bus.city = strarr[3];
				bus.state = strarr[4];
				bus.zip = strarr[5];
				bus.reviewloc = -1;
				bus.reviews_path = reviews_path;

				while (!feof(fpr)) {
					if (!notf) {
						loc = ftell(fpr);
						fgets(strrev,8000,fpr);
					}
					if (!feof(fpr)) {
						if (!notf) strarrrev = explode(strrev,"\t");
						//printf("bus.id = %s, strarrrev[0] = %s\n",bus.id,strarrrev[0]);
						if (strcmp(bus.id,strarrrev[0]) == 0) {
							//printf("id = %s strarrrev[0] = %s\n",bus.id,strarrrev[0]);
							bus.reviewloc = loc;
							notf = 0;
							destroyStringArray(strarrrev, 6);
							break;
						}
						else if (atoi(bus.id) < atoi(strarrrev[0])) { 
							printf("id = %s strarrrev[0] = %s review not found!\n",bus.id,strarrrev[0]);
							notf = 1;
							break;
						}
						//printf("id = %s strarrrev[0] = %s \n",bus.id,strarrrev[0]);
						destroyStringArray(strarrrev, 6);
					}
				}
				nameroot = insert_name(bus, nameroot);
				destroyStringArray(strarr, 7);
			}
		}
		free(str);
		free(strrev);
		fclose(fp);
		fclose(fpr);
	}
	return nameroot;
}

StateNode * searchstate(StateNode * stroot, char * state)
{
	if (stroot == NULL) return NULL;
	if (strcasecmp(state,stroot->state) == 0) {/*printf("state found!\n");*/ return stroot;}
	if (strcasecmp(state,stroot->state) < 0) return searchstate(stroot->left, state);
	return searchstate(stroot->right, state);
}

NameNode * searchname(NameNode * nameroot, char * name)
{
	if (nameroot == NULL) return NULL;
	if (strcasecmp(name,nameroot->name) == 0) {/*printf("name found!\n");*/  return nameroot;}
	if (strcasecmp(name,nameroot->name) < 0) return searchname(nameroot->left, name);
	return searchname(nameroot->right, name);
}

void locidcount(IdNode * idroot, uint32_t * num_locations)
{
	if (idroot == NULL) return;
	locidcount(idroot->left, num_locations);
	(*num_locations) ++;
	locidcount(idroot->right, num_locations);
}

void loczipcount(ZipNode * ziproot, uint32_t * num_locations)
{
	if (ziproot == NULL) return;
	loczipcount(ziproot->left, num_locations);
	locidcount(ziproot->idroot, num_locations);
	loczipcount(ziproot->right, num_locations);
}

void loccount(StateNode * stfound, uint32_t * num_locations)
{
	if (stfound == NULL) return;
	loccount(stfound->left, num_locations);
	loczipcount(stfound->ziproot, num_locations);
	loccount(stfound->right, num_locations);
}

uint32_t addreview(char * id,long int reviewloc,struct Review * * reviews, uint32_t * num_reviews, const char* reviews_path)
{
	FILE * fpr = NULL;
	char * strrev = (char *) malloc(8000 * sizeof(char));
	char * * strarr = NULL;
	int count = 0;
	fpr = fopen(reviews_path,"r");
	if (!fpr) {
		fprintf(stderr,"Failed to open file '%s'\n", reviews_path);
		reviews = NULL;
		return 0;
	}
	if (reviewloc == -1) {
		printf("%s not found!\n",id);
		reviews = NULL;
		return 0;
	}
	fseek(fpr,reviewloc,SEEK_SET);
	while (!feof(fpr)) {
		fgets(strrev,8000,fpr);
		if (!feof(fpr)) {
			strarr = explode(strrev,"\t"); 
			if (strcmp(id,strarr[0]) != 0) {
				destroyStringArray(strarr, 6);
				break;
			}
			count ++;
			destroyStringArray(strarr, 6);
		}
	}
	//printf("count = %d\n",count);
	*reviews = malloc(sizeof(struct Review)*count);////
	fseek(fpr,reviewloc,SEEK_SET);
	int ind = 0;
	while (!feof(fpr)) {
		fgets(strrev,8000,fpr);
		if (!feof(fpr)) {
			strarr = explode(strrev,"\t");
			if (strcmp(id,strarr[0]) != 0) {
				destroyStringArray(strarr, 6);
				break;
			}

			((*reviews)[ind]).text = strdup(strarr[5]);
			//printf("%s\n",(reviews[ind]).text);
			((*reviews)[ind]).stars = (uint8_t) atoi(strarr[1]);
			//printf("%d\n",(uint8_t) atoi(strarr[1]));
			//(*reviews) ++;
			ind ++;
			destroyStringArray(strarr, 6);
		}
	}
	//may qsort here......
	free(strrev);
	fclose(fpr);

	return (uint32_t) count;

}

void createlocip(IdNode * idroot, char * zip, char * state, int * ind, struct Location * locarr)
{
	if (idroot == NULL) return;
	createlocip(idroot->left, zip, state, ind,locarr);
	(locarr[*ind]).state = strdup(state);
	(locarr[*ind]).zip_code = strdup(zip);
	(locarr[*ind]).address = strdup(idroot->address);
	(locarr[*ind]).city = strdup(idroot->city);
	//printf("state = %s, zip_code = %s, address = %s, city = %s\n",locarr->state, locarr->zip_code, locarr->address,locarr->city);
	//review struct and num_review......
	(locarr[*ind]).num_reviews = addreview(idroot->id,idroot->reviewloc,&((locarr[*ind]).reviews),&((locarr[*ind]).num_reviews), idroot->reviews_path);
	(*ind) ++;
	createlocip(idroot->right, zip, state, ind,locarr);
}

void createloczp(ZipNode * ziproot, char * state, int * ind, struct Location * locarr)
{
	if (ziproot == NULL) return;
	createloczp(ziproot->left, state, ind,locarr);
	createlocip(ziproot->idroot, ziproot->zip, state,ind,locarr);
	createloczp(ziproot->right, state, ind,locarr);
}

void createlocarr(StateNode * stfound, struct Location * locarr, int * ind)
{
	if (stfound == NULL) return;
	createlocarr(stfound->left, locarr,ind);
	//(locarr[ind]).state = strdup(stfound->state);////
	//printf("(locarr[ind]).state = %s\n",(locarr[ind]).state);
	createloczp(stfound->ziproot,stfound->state,ind,locarr);
	//may qsort city 
	createlocarr(stfound->right, locarr,ind);	
}

struct Business* get_business_reviews(struct YelpDataBST* bst, char* name, char* state, char* zip_code)
{
	struct Business * busi = malloc(sizeof(struct Business));
	NameNode * namefound = NULL;
	StateNode * stfound = NULL;
	//ZipNode * zipfound = NULL;

	namefound = searchname(bst, name);
	//zipfound = searchzip(stfound->ziproot,zip_code);

	busi->name = (namefound == NULL) ? NULL : strdup(namefound->name); ////
	if (busi->name == NULL) return NULL;
	else {//this function need change!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (state == NULL) {
			busi->num_locations = 0;
			loccount(namefound->stroot,&(busi->num_locations));
			//printf("have %d id\n",busi->num_locations);
			busi->locations = malloc(sizeof(struct Location)*(busi->num_locations));
			int ind = 0;
			createlocarr(namefound->stroot, busi->locations,&ind);
			//printf("(locarr[ind]).state = %s\n",(busi->locations[0]).state);
		}
		else if (state != NULL) {
			stfound = searchstate(namefound->stroot,state);
			if (stfound == NULL) {
				busi->locations = NULL;
				busi->num_locations = 0;
				return busi;
			}
			else {
				busi->num_locations = 0;
				loczipcount(stfound->ziproot, &(busi->num_locations));
				busi->locations = malloc(sizeof(struct Location)*(busi->num_locations));
				int ind = 0;
				createlocarr(stfound, busi->locations,&ind);
				//printf("(locarr[ind]).state = %s\n",(busi->locations[0]).state);
			}
		}

		//zipfound = searchzip(stfound->ziproot,zip_code);


	}


	return busi;
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
	zipno->idroot = NULL;//very important!!!!!!
	zipno->idroot = insert_id(bus, zipno->idroot);
	zipno->left = NULL;
	zipno->right = NULL;
	return zipno;
}

IdNode * create_idnode(BusNode bus) //malloc
{
	IdNode * idno = malloc(sizeof(IdNode));////
	idno->id = strdup(bus.id);////
	idno->city = strdup(bus.city);////
	idno->address = strdup(bus.address);////
	idno->reviewloc = bus.reviewloc; 
	idno->reviews_path = strdup(bus.reviews_path);////
	idno->left = NULL;
	idno->right = NULL;
	return idno;
}

NameNode * insert_name(BusNode bus, NameNode * nameroot)
{
	if (nameroot == NULL) {
		return create_namenode(bus);
	}
	if (strcasecmp(bus.name, nameroot->name) < 0) {
		nameroot->left = insert_name(bus, nameroot->left);
	}
	else if (strcasecmp(bus.name, nameroot->name) > 0) {
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
	if (strcmp(bus.zip, ziproot->zip) < 0) {
		ziproot->left = insert_zip(bus, ziproot->left);
	}
	else if (strcmp(bus.zip, ziproot->zip) > 0) {
		ziproot->right = insert_zip(bus, ziproot->right);
	}
	else {
		ziproot->idroot = insert_id(bus, ziproot->idroot);
	}
	return ziproot;
}

IdNode * insert_id(BusNode bus, IdNode * idroot)
{
	if (idroot == NULL) { 
		return create_idnode(bus);
	}
	if (strcmp(bus.id, idroot->id) < 0) {
		idroot->left = insert_id(bus, idroot->left);
	}
	else if (strcmp(bus.id, idroot->id) > 0) {
		idroot->right = insert_id(bus, idroot->right);
	}
	else {
		printf("what fuck??????\n");
	}
	return idroot;
}

StateNode * insert_st(BusNode bus, StateNode * stroot)
{
	if (stroot == NULL) {
		return create_statenode(bus);
	}
	if (strcmp(bus.state, stroot->state) < 0) {
		stroot->left = insert_st(bus, stroot->left);
	}
	else if (strcmp(bus.state, stroot->state) > 0) {
		stroot->right = insert_st(bus, stroot->right);
	}
	else {
		stroot->ziproot = insert_zip(bus, stroot->ziproot);
	}
	return stroot;
}

void destroy_business_bst(struct YelpDataBST* bst)
{
	return;
}

void destroy_business_result(struct Business* b)
{
	return;
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
