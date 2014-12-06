#include <ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
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

Node * search_name(char * name, Node * root);
Node * insert_node(Node * node, Node * root);
char * * explode(const char * str, const char * delims, int len);
void destroyStringArray(char * * strArr, int len);
uint32_t addreview(char * id,long int reviewloc,struct Review * * reviews, uint32_t * num_reviews, const char* reviews_path);
int compar_rev(const void * arg1, const void * arg2);
int compar_loc(const void * arg1, const void * arg2);

struct YelpDataBST* create_business_bst(const char* businesses_path, const char* reviews_path)
{
	FILE * fp = NULL;
	FILE * fpr = NULL;
	fp = fopen(businesses_path, "r");
	fpr = fopen(reviews_path, "r");
	if(!fp) {
		fprintf(stderr,"Failed to open file '%s'\n", businesses_path);
		return NULL;
	}
	if (!fpr) {
		fprintf(stderr,"Failed to open file '%s'\n", reviews_path);
		return NULL;
	}

	char * str = (char *) malloc(2000 * sizeof(char));
	char * strrev = (char *) malloc(8000 * sizeof(char));
	char * * strarr = NULL;
	char * * strarrrev = NULL;

	Node * root = NULL; //tree root
	Node * node = NULL;
	//char * id = NULL;
	//char * name = NULL;
	long int boffset = 0;
	long int roffset = 0;
	int notf = 0;

	while (!feof(fp)) {
		fgets(str,2000,fp);
		node = malloc(sizeof(Node)); //////
		strarr = explode(str,"\t",7);
		node->id = strdup(strarr[0]);
		node->name = strdup(strarr[1]);
		node->boffset = boffset;
		boffset = ftell(fp); //boffset for the next
		node->left = NULL;
		node->right = NULL;

		//add review loc
		node->roffset = -1;
		while (!feof(fpr)) {
			if (!notf) {
				roffset = ftell(fpr);
				fgets(strrev,8000,fpr);
				strarrrev = explode(strrev,"\t",6);
			}
			if (strcmp(node->id,strarrrev[0]) == 0) {
				node->roffset = roffset;
				//printf("name= %s id= %s\n boff= %ld tree->roffset= %ld\n",node->name,node->id,node->boffset,node->roffset);
				//printf("%s\n\n",strarrrev[5]);
				notf = 0;
				destroyStringArray(strarrrev, 6);
				break;
			}
			else if (atoi(node->id) < atoi(strarrrev[0])) {
				notf = 1;
				break;
			}
			destroyStringArray(strarrrev, 6);
		}
		root = insert_node(node, root);
		destroyStringArray(strarr, 7);
	}
	free(str);
	free(strrev);
	fclose(fp);
	fclose(fpr);

	root->bpath = businesses_path;
	root->rpath = reviews_path;

	return root;

}

struct Business* get_business_reviews(struct YelpDataBST* bst, char* name, char* state, char* zip_code)
{
	if (name == NULL) return NULL;
	FILE * fpb = fopen(bst->bpath, "r");
	//FILE * fpr = fopen(bst->rpath, "r");

	if(!fpb) {
		fprintf(stderr,"Failed to open file '%s'\n", bst->bpath);
		return NULL;
	}   
	/*if (!fpr) {
	  fprintf(stderr,"Failed to open file '%s'\n", bst->rpath);
	  return NULL;
	  }*/

	Node * nodefound = NULL;
	struct Business* busi = malloc(sizeof(struct Business));
	busi->name = NULL;
	int locnum = 0;

	char * str = (char *) malloc(2000 * sizeof(char));
	char * * strarr = NULL;
	int meet = 0;
	nodefound = search_name(name,bst);
	if (nodefound != NULL) {
		do {
			fseek(fpb, nodefound->boffset, SEEK_SET);
			fgets(str,2000,fpb);
			strarr = explode(str,"\t",7);
			//printf("strarr[1] = %s state = %s zip = %s\n",strarr[1],state,zip_code);
			meet = (state==NULL ? 1:(strcasecmp(state,strarr[4])==0))&&(zip_code==NULL ? 1:(strcasecmp(zip_code,strarr[5])==0));
			//printf("meet = %d\n",meet);
			if (meet) locnum ++;
			nodefound = search_name(name,nodefound->left);
			destroyStringArray(strarr, 7);
		}while (nodefound != NULL);
	}	
	//printf("numloc = %d\n",locnum);
	busi->num_locations = locnum;
	busi->locations = malloc(sizeof(struct Location)*(busi->num_locations));

	nodefound = search_name(name,bst);
	if (nodefound != NULL) {
		busi->name = strdup(name);
		int ind = 0;
		do {
			fseek(fpb, nodefound->boffset, SEEK_SET);
			fgets(str,2000,fpb);
			strarr = explode(str,"\t",7);
			meet = (state==NULL ? 1:(strcasecmp(state,strarr[4])==0))&&(zip_code==NULL ? 1:(strcasecmp(zip_code,strarr[5])==0));
			if (meet) {
				((busi->locations)[ind]).address = strdup(strarr[2]);
				//printf("address = %s\n",((busi->locations)[ind]).address);
				((busi->locations)[ind]).city = strdup(strarr[3]);
				((busi->locations)[ind]).state = strdup(strarr[4]);
				((busi->locations)[ind]).zip_code = strdup(strarr[5]);
				//add reviews here
				((busi->locations)[ind]).num_reviews = addreview(nodefound->id,nodefound->roffset,&(((busi->locations)[ind]).reviews),&(((busi->locations)[ind]).num_reviews),bst->rpath);
				ind ++;
			}
			nodefound = search_name(name,nodefound->left);
			destroyStringArray(strarr, 7);
		}while (nodefound != NULL);
	}

	qsort(&(busi->locations[0]),busi->num_locations,sizeof(struct Location),compar_loc);

	free(str);
	fclose(fpb);

	return busi;
}
Node * search_name(char * name, Node * root)
{
	if(root == NULL) return NULL; 
	if(strcasecmp(name, root->name) == 0) return root; 
	if(strcasecmp(name, root->name) > 0){
		return search_name(name, root->right); 
	}
	return search_name(name, root->left);
}

Node * insert_node(Node * node, Node * root)
{
	if (root == NULL) return node;
	if (strcasecmp(node->name, root->name) <= 0) {
		root->left = insert_node(node, root->left);
	}
	else {
		root->right = insert_node(node, root->right);
	}
	return root;
}

void destroy_business_bst(struct YelpDataBST* bst)
{
	if(bst == NULL) return;
	destroy_business_bst(bst->left);
	destroy_business_bst(bst->right);
	free(bst->name);
	free(bst->id);
	free(bst);
}

void destroy_loc(struct Location* locations, uint32_t num_locations);
void destroy_rev(struct Review* reviews, uint32_t num_reviews);

void destroy_rev(struct Review* reviews, uint32_t num_reviews)
{
	int i;
	for (i = 0; i < num_reviews; i++) {
		free((reviews[i]).text);
	}
	free(reviews);
}

void destroy_loc(struct Location* locations, uint32_t num_locations)
{
	int i;
	for (i = 0; i < num_locations; i++) {
		free((locations[i]).address);
		free((locations[i]).city);
		free((locations[i]).state);
		free((locations[i]).zip_code);
		destroy_rev((locations[i]).reviews, (locations[i]).num_reviews);
	}
	free(locations);
}

void destroy_business_result(struct Business* b)
{
	free(b->name);
	destroy_loc(b->locations, b->num_locations);
	free(b);
}

char * * explode(const char * str, const char * delims, int len)
{
	int N = len;
	int i;
	int arrind = 0;
	int last = 0;

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
			strarr = explode(strrev,"\t",6); 
			if (strcmp(id,strarr[0]) != 0) {
				destroyStringArray(strarr, 6);
				break;
			}
			count ++;
			destroyStringArray(strarr, 6);
		}
	}
	*reviews = malloc(sizeof(struct Review)*count);////
	fseek(fpr,reviewloc,SEEK_SET);
	int ind = 0;
	while (!feof(fpr)) {
		fgets(strrev,8000,fpr);
		if (!feof(fpr)) {
			strarr = explode(strrev,"\t\n",7);
			if (strcmp(id,strarr[0]) != 0) {
				destroyStringArray(strarr, 7);
				break;
			}

			((*reviews)[ind]).text = strdup(strarr[5]);
			((*reviews)[ind]).stars = (uint8_t) atoi(strarr[1]);

			ind ++;
			destroyStringArray(strarr, 7);
		}
	}
	qsort(&((*reviews)[0]),(int) count,sizeof(struct Review),compar_rev);
	free(strrev);
	fclose(fpr);

	return (uint32_t) count;

}


int compar_rev(const void * arg1, const void * arg2)
{
	const struct Review* ptr1 = (const struct Review*) arg1;
	const struct Review* ptr2 = (const struct Review*) arg2;
	const uint8_t stars1 =  ptr1->stars;
	const uint8_t stars2 =  ptr2->stars;
	if (stars1 > stars2) return -1;
	else if (stars1 < stars2) return 1;
	else {
		const char * str1 = ptr1->text;
		const char * str2 = ptr2->text;
		return strcasecmp(str1,str2);
	}
}

int compar_loc(const void * arg1, const void * arg2)
{
	int result;
	const struct Location* ptr1 = (const struct Location*) arg1;
	const struct Location* ptr2 = (const struct Location*) arg2;
	const char * state1 = ptr1->state;
	const char * state2 = ptr2->state;
	result = strcasecmp(state1,state2);
	if (result == 0) {
		const char * city1 = ptr1->city;
		const char * city2 = ptr2->city;
		result = strcasecmp(city1,city2);
		if (result == 0) {
			const char * address1 = ptr1->address;
			const char * address2 = ptr2->address;
			result = strcasecmp(address1,address2);
		}
	}
	return result;
}

