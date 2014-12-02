#include "answer10.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

struct YelpDataBST {
    char * name;
    int loc;
    int rev1;
    int rev2;
    const char * p1;
    const char * p2;
    
    struct YelpDataBST * left;
    struct YelpDataBST * right;
};

struct YelpDataBST * create_leaf(char *, int, int, int, const char* , const char*);
struct YelpDataBST * insert_tree(struct YelpDataBST *, struct YelpDataBST *);
struct YelpDataBST * tree_search_name(char *, struct YelpDataBST *);
char * * explode(const char *, const char *, int);
void destroy_exp(char * *);
int compare_locations(const void *a, const void *b);
int compare_reviews(const void *a, const void *b);

struct YelpDataBST* create_business_bst(const char* businesses_path, const char* reviews_path) {
    FILE * fpb = fopen(businesses_path, "r");
    FILE * fpr = fopen(reviews_path, "r");
    
    if(fpb == NULL || fpr == NULL){
        if(!fpb)
        { fprintf(stderr, "Failed to open file '%s'\n", businesses_path);}
        if(!fpr)
        { fprintf(stderr, "Failed to open file '%s'\n", reviews_path);}
        return NULL;
    }
    
    struct YelpDataBST * tree = NULL;
    char str[2048];
    char * * exstr = NULL;
    char str_r[10240];
    char * * exstr_r = NULL;
    char * b_id = NULL;
    char * b_name = NULL;
    char * id = NULL;
    int loc, rev1, rev2;
    loc = 0;
    rev2 = 0;
    while(fgets(str, 2048, fpb) != NULL){
        exstr = explode(str, "\t",7);
        b_id = exstr[0];
        b_name = exstr[1]; //Input1
        
        fseek(fpr, rev2, SEEK_SET);
        rev1 = ftell(fpr); //Input3
        fgets(str_r, 10240, fpr);
        exstr_r = explode(str_r, "\t\n", 7);
        id = exstr_r[0];
        fseek(fpr, rev1, SEEK_SET);
        while(strcmp(id, b_id) == 0 && !feof(fpr)){
            destroy_exp(exstr_r);
            fgets(str_r, 10240, fpr);
            exstr_r = explode(str_r, "\t\n", 7);
            id = exstr_r[0];
            if(strcmp(id, b_id) == 0){
                rev2 = ftell(fpr); //Input4
            }
        }
        tree = insert_tree(tree, create_leaf(b_name, loc, rev1, rev2, businesses_path, reviews_path));
        loc = ftell(fpb); //Input2
        destroy_exp(exstr);
        destroy_exp(exstr_r);
    }
    fclose(fpb);
    fclose(fpr);
    return tree;
}

struct Business* get_business_reviews(struct YelpDataBST* bst, char* name, char* state, char* zip_code){
    FILE * fpb = fopen(bst -> p1, "r");
    FILE * fpr = fopen(bst -> p2, "r");
    if(fpb == NULL || fpr == NULL){
        if(!fpb)
        { fprintf(stderr, "Failed to open file '%s'\n", bst -> p1);}
        if(!fpr)
        { fprintf(stderr, "Failed to open file '%s'\n", bst -> p2);}
        return NULL;
    }
    
    struct Business * result = NULL;
    struct YelpDataBST * possible_node = tree_search_name(name, bst);
    if(possible_node == NULL)
    { fclose(fpb); fclose(fpr);return NULL; }
    
    char str[2048];
    int i = 0;
    int num_i = i;
    uint32_t arr_j[1024];
    uint32_t j = 0;
    
    //Get the name
    fseek(fpb, possible_node -> loc, SEEK_SET);
    fgets(str, 2048, fpb);
    char * * exstr1 = explode(str, "\t", 7);
    destroy_exp(exstr1);
    
    //Get how many blocks needed for locs and revs
    while(possible_node != NULL){
        fseek(fpb, possible_node -> loc, SEEK_SET);
        fgets(str, 2048, fpb);
        exstr1 = explode(str, "\t", 7);
        char * b1_state = exstr1[4];
        char * b1_zip = exstr1[5];
        int ct = 0;
        int cd = 0;
        
        if(state == NULL && zip_code == NULL) {cd = 1;}
        else if(state == NULL && (strcasecmp(zip_code, b1_zip) == 0)) {cd = 1;}
        else if(zip_code == NULL && (strcasecmp(state, b1_state) == 0)){cd = 1;}
        else if(state != NULL && zip_code != NULL && (strcasecmp(state, b1_state) == 0) && (strcasecmp(zip_code, b1_zip) == 0)) {cd = 1;}
        
        if(cd){
            ct = 1;
            int bytes = possible_node -> rev1;
            while(bytes < possible_node -> rev2){
                fseek(fpr, bytes, SEEK_SET);
                char storer[10240];
                fgets(storer, 10240, fpr);
                bytes = ftell(fpr);
                j++;
            }
        }
        possible_node = possible_node -> left;
        possible_node = tree_search_name(name, possible_node);
        if(ct){
            arr_j[i] = j;
            i++;
            num_i = i;
        }
        destroy_exp(exstr1);
        j = 0;
    }
    
    if(i == 0)
    { fclose(fpb); fclose(fpr); return NULL;}
    
    //Malloc for locs and revs
    struct Location * locs = malloc(i * sizeof(struct Location)); //Input2
    struct Review * revs = NULL;
    i = 0;
    possible_node = tree_search_name(name, bst);
    while(possible_node != NULL){
        fseek(fpb, possible_node -> loc, SEEK_SET);
        fgets(str, 2048, fpb);
        exstr1 = explode(str, "\t", 7);
        char * b_ad = exstr1[2]; //Locs 1
        char * b_city = exstr1[3];//Locs 2
        char * b_state = exstr1[4];//Locs 3
        char * b_zip = exstr1[5];//Locs 4
        int ct2 = 0;
        int ind = 0;
        
        int cd1 = 0;
        if(state == NULL && zip_code == NULL){cd1 = 1;}
        else if(state == NULL && strcasecmp(zip_code, b_zip) == 0){cd1 = 1;}
        else if(zip_code == NULL && strcasecmp(state, b_state) == 0){cd1 = 1;}
        else if(state != NULL && zip_code != NULL && strcasecmp(state, b_state) == 0 && strcasecmp(zip_code, b_zip) == 0){cd1 = 1;}
        if(cd1){
            ct2 = 1;
            //Locs input (revs)
            revs = malloc(arr_j[i] * sizeof(struct Review));
            int bytes = possible_node -> rev1;
            while(bytes < possible_node -> rev2){
                fseek(fpr, bytes, SEEK_SET);
                char str_rev[10240];
                fgets(str_rev, 10240, fpr);
                char * * exp_revs = explode(str_rev, "\t\n", 7);
                //Create revs
                revs[ind].text = strdup(exp_revs[5]);
                revs[ind++].stars = (uint8_t)(exp_revs[1][0] - '0');
                bytes = ftell(fpr);
                destroy_exp(exp_revs);
            }
        }
        possible_node = possible_node -> left;
        possible_node = tree_search_name(name, possible_node);
        if(ct2){
            //Create locs
            qsort(revs, arr_j[i], sizeof(struct Review), compare_reviews);
            locs[i].address = strdup(b_ad);
            locs[i].city = strdup(b_city);
            locs[i].state = strdup(b_state);
            locs[i].zip_code = strdup(b_zip);
            locs[i].reviews = revs;
            locs[i].num_reviews = arr_j[i];
            i++;
        }
        destroy_exp(exstr1);
    }
    
    //Create the result
    result = malloc(sizeof(struct Business));
    result -> name = strdup(name);
    qsort(locs, num_i, sizeof(struct Location), compare_locations);
    result -> locations = locs;
    result -> num_locations = num_i;
    fclose(fpb);
    fclose(fpr);
    return result;
}

void destroy_business_bst(struct YelpDataBST* bst){
    if(bst == NULL)
    { return; }
    destroy_business_bst(bst -> left);
    destroy_business_bst(bst -> right);
    free(bst -> name);
    free(bst);
}

void destroy_business_result(struct Business* b){
    free(b -> name);
    int ind, ind2;
    for(ind = 0; ind < b -> num_locations; ind++){
        free(b -> locations[ind].address);
        free(b -> locations[ind].city);
        free(b -> locations[ind].state);
        free(b -> locations[ind].zip_code);
        for(ind2 = 0; ind2 < b -> locations[ind].num_reviews; ind2++){
            free(b -> locations[ind].reviews[ind2].text);
        }
        free(b -> locations[ind].reviews);
    }
    free(b -> locations);
    free(b);
}


/******************************Insert Functions******************************/
struct YelpDataBST * create_leaf(char * n, int a, int b, int c, const char * x, const char * y){
    struct YelpDataBST * leaf = malloc(sizeof(struct YelpDataBST));
    leaf -> name = strdup(n);
    leaf -> loc = a;
    leaf -> rev1 = b;
    leaf -> rev2 = c;
    leaf -> p1 = x;
    leaf -> p2 = y;
    leaf -> left = NULL;
    leaf -> right = NULL;
    return leaf;
}

struct YelpDataBST * insert_tree(struct YelpDataBST * root, struct YelpDataBST * node){
    if(root == NULL)
    { return node; }
    if(node == NULL)
    { return root; }
    if(strcmp(node -> name, root -> name) <= 0)
    { root -> left = insert_tree(root -> left, node); }
    else if(strcmp(node -> name, root -> name) > 0)
    { root -> right = insert_tree(root -> right, node); }
    return root;
}

/******************************Search Functions******************************/
struct YelpDataBST * tree_search_name(char * name, struct YelpDataBST * root){
    if(root == NULL)
    { return NULL; }
    if(strcasecmp(name, root -> name) == 0)
    { return root; }
    if(strcasecmp(name, root -> name) < 0)
    { return tree_search_name(name, root -> left); }
    return tree_search_name(name, root -> right);
}

/******************************Explode Function******************************/
char * * explode(const char * str, const char * delims, int ct){
    int Ind = 0;
    int ind = 0;
    int last = 0;
    int l;
    char * * arr = malloc(ct * sizeof(char*));
    
    for(ind = 0; str[ind] != '\0'; ind++){
        if(strchr(delims,str[ind]) || (str[ind] == '\0')){
            l = 1 + ind - last;
            arr[Ind] = (char *)malloc(l);
            memcpy(arr[Ind], str + last, l - 1);
            *(arr[Ind] + ind - last) = '\0';
            last = ind + 1;
            Ind++;
        }
    }
    
    l = 1 + ind - last;
    arr[Ind] = malloc(l * sizeof(char));
    memcpy(arr[Ind], str + last, l - 1);
    * (arr[Ind] + l - 1) = '\0';
    return arr;
}

void destroy_exp(char * * a){
    int i = 0;
    for( ; i < 7; ++i)
    { free(a[i]);}
    free(a);
}

/******************************QSORT Functions******************************/
int compare_locations(const void *a, const void *b){
    struct Location * loc1 = (struct Location *)a;
    struct Location * loc2 = (struct Location *)b;
    int compare_result = strcasecmp(loc1 -> state, loc2 -> state);
    if(compare_result != 0) { return compare_result;}
    compare_result = strcasecmp(loc1 -> city, loc2 -> city);
    if(compare_result != 0) { return compare_result;}
    compare_result = strcasecmp(loc1 -> address, loc2 -> address);
    return compare_result;
}

int compare_reviews(const void *a, const void *b){
    struct Review * rev1 = (struct Review *)a;
    struct Review * rev2 = (struct Review *)b;
    int compare_result = rev1 -> stars - rev2 -> stars;
    if(compare_result != 0){ return -compare_result;}
    compare_result = strcasecmp(rev1 -> text, rev2 -> text);
    return compare_result;
}
