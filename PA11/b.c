#include <stdio.h>
#include <stdlib.h>
#include "answer11.h"

typedef struct bitstream_bst
{
	FILE * file;
	unsigned char curr; //Current byte inspected
	unsigned char bitpos; //Index of Bit in the byte curr to be inspected (0-7 are the only values the code will allow)

} Bitstream;

unsigned char readbits(Bitstream * bs, size_t num);

/**
 * Create a new, properly intialized HuffNode
 */
HuffNode * HuffNode_create(int value)
{
	HuffNode * huff;
	huff = malloc(sizeof(HuffNode)); 
	if (huff == NULL) return NULL;
	else {
		huff->value = value;
		huff->left = NULL;
		huff->right = NULL;
	}
	return huff;
}

/**
 * Destroy a tree, including all sub-children. Must handle NULL values safely.
 */
void HuffNode_destroy(HuffNode * tree)
{
	if (tree == NULL) return;
	HuffNode_destroy(tree->left);
	HuffNode_destroy(tree->right);
	free(tree);
}

// ----------------------------------------------------------------------- Stack

/**
 * We need a "Stack" to build the tree structure that is used to decode
 * a Huffman encoding. Stacks are very simple to implement with linked lists.
 * We must implement the following functions (given below). Each of these
 * functions should only be a few lines of code.
 *
 * (1) Stack_create();              // Allocate a brand new stack.
 * (2) Stack_destroy(stack);        // Clean up memory for the stack.
 * (3) Stack_isEmpty(stack);    // TRUE iff (if and only if) the stack is empty.
 * (4) Stack_pushFront(stack, tree); // Push a tree onto the stack.
 * (6) Stack_popFront(stack);  // Remove a tree from the stack and return it.
 *
 * Altogether, these six functions should be around 40 lines of code.
 *
 */

/**
 * Returns a pointer to a new empty stack struct
 */
Stack * Stack_create()
{
	Stack * st =  malloc(sizeof(Stack));
	if (st == NULL) return NULL;
	else {
		st->head = NULL;
		return st;
	}
}

/**
 * Frees all memory associated with the stack.
 * Don't forget that you must free the entire contained linked-list.
 * Also, you must safely do nothing if stack == NULL.
 */
void Stack_destroy(Stack * stack)
{
	if (stack != NULL) {	
		while(!Stack_isEmpty(stack))
		{    
			HuffNode_destroy(Stack_popFront(stack));    
		}
		free(stack);
	}
	else {
		return;
	}
}

/**
 * Returns TRUE (something other than zero) if the stack is empty.
 */
int Stack_isEmpty(Stack * stack)
{
	if (stack->head == NULL) return 1;
	return 0;
}

/**
 * Pop the front (top) 'value' (i.e. Huffman tree) from the stack.
 *
 * More precisely, this function must do three things:
 * (1) Save the value (i.e. Huffman tree) of the head node of the stack's list
 * (2) Remove the head node of the stack's list, freeing it.
 * (3) Return the value (i.e. Huffman tree) saved in (1).
 */
HuffNode * Stack_popFront(Stack * stack)
{
	HuffNode * huff = NULL;
	StackNode * new = NULL;
	if (stack == NULL) return NULL;
	if (Stack_isEmpty(stack) != 0) stack->head = new;
	else {   
		huff = (stack->head)->tree;
		new = (stack->head)->next;
		free(stack->head);
		stack->head = new;
	}

	return huff;
}

/**
 * Push a 'value' (i.e. Huffman tree) onto the front (top) of the stack.
 *
 * More precisely, this function must do two things:
 * (1) Create a new StackNode with 'tree' for its tree.
 * (2) Push that new StackNode onto the front of the stack's list.
 */
void Stack_pushFront(Stack * stack, HuffNode * tree)
{
	StackNode * st = malloc(sizeof(StackNode));
	if (st == NULL) printf("error!");
	else {
		st->tree = tree;
		st->next = stack->head;
		stack->head = st;
	}
}

// -------------------------------------------------------- Pop Pop combine Push

/**
 * This function helps simplify building a Huffman Coding Tree from the header
 * information. It takes a stack as input. As a precondition, you can assume
 * that the stack has at least two nodes. This function pops the front (top)
 * two nodes, combines them into a single node, and pushes the new node back
 * onto the stack. See Huffman_Coding.pdf to understand conceptually how this
 * should be done.
 */
void Stack_popPopCombinePush(Stack * stack)
{
	HuffNode * huff1, * huff2, * newhuff;

	huff1 = Stack_popFront(stack);
	huff2 = Stack_popFront(stack);

	if (huff1 == NULL && huff2 == NULL) newhuff = NULL;
	else if (huff1 == NULL)	newhuff = huff2;
	else if (huff2 == NULL) newhuff = huff1;
	else {
		int valuesum = 0;
		valuesum += huff1->value + huff2->value;
		newhuff = HuffNode_create(valuesum);
		if (newhuff != NULL) {
			if (huff1 < huff2) {
				newhuff->left = huff1;
				newhuff->right = huff2;
			}
			else {
				newhuff->left = huff2;
				newhuff->right = huff1;
			}
		}
		else {
			newhuff->left = NULL;
			newhuff->right = NULL;
		}
	}
	Stack_pushFront(stack, newhuff);
}

// ---------------------------------------------------- Reading HuffTree headers

/**
 * Read a Huffman Coding Tree (in text format) from 'fp'.
 */
HuffNode * HuffTree_readTextHeader(FILE * fp)
{
	HuffNode * readt = NULL;
	Stack * stack = Stack_create();

	char p;
	char ch;
	while (!feof(fp))
	{
		p = fgetc(fp);
		if (p == '1') {
			ch = fgetc(fp);
			Stack_pushFront(stack, HuffNode_create(ch));
		}
		else if (p == '0')
		{
			if ((stack->head)->next != NULL) Stack_popPopCombinePush(stack);
			else {
				readt = Stack_popFront(stack);
				Stack_destroy(stack);
				return readt;
			}
		}
	}
	readt = Stack_popFront(stack);
	Stack_destroy(stack);
	return readt;
}

/**
 * Read a Huffman Coding Tree (in binary format) from 'fp'.
 * You will need to (conceptually) read a file a bit at a time. See the README
 * for hints on how to do this.
 */
HuffNode * HuffTree_readBinaryHeader(FILE * fp)
{
	Stack * stack = Stack_create();
	HuffNode * readt = NULL;
	unsigned char bi = 0x00;
	unsigned char current;
	unsigned char pos = 0;
	unsigned char m = 0x01<<7;
	int ind;

	while(!feof(fp))
	{
		//read 1 bit
		//bi = readbits(bs, 1);
		if (pos == 0) fread(&current, sizeof(unsigned char), 1, fp);
		bi = bi<< 1 |((m & (current)<<(pos)))>>7; 
		pos = (pos + 1) % 8;
		//if 0, ppcb
		if (bi == 0)
		{

			if ((stack->head)->next == NULL) //ONE TREE LEFT
				break;
			else
				Stack_popPopCombinePush(stack);

		}
		//if 1 , read 8 byte
		else if (bi == 1) //bi == 1
		{
			unsigned char ch = 0x00;
			for (ind = 0; ind < 8; ind++) {
				if (pos == 0) fread(&current, sizeof(unsigned char), 1, fp);
				ch <<= 1; 
				ch |= ((m & (current)<<(pos)))>>7;
				pos = (pos + 1) % 8;
			}
			Stack_pushFront(stack, HuffNode_create(ch));
		}
	}
	readt = Stack_popFront(stack);
	Stack_destroy(stack);
	return readt;
}

unsigned char readbits(Bitstream * bs, size_t num)
{
	unsigned char result = 0x00; //RESULT of
	unsigned char bi; //used to get data from fread.
	unsigned char mask = 0x01<<7; //mask to get MOST SIGNIFICANT BIT
	size_t ind = 0; //Counter

	for (ind = 0; ind < num; ind++) //for each bits requested
	{
		if (bs->bitpos == 0) //End of byte, read in a new byte
		{
			fread(& bi, sizeof(unsigned char), 1, bs->file);
			bs->curr = bi;
		}
		result = result << 1 | ((mask & (bs->curr)<<(bs->bitpos)))>>7;
		bs->bitpos = (bs->bitpos + 1) % 8;

	}
	return result;
}

