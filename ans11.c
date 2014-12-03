#include <stdio.h>
#include <stdlib.h>
#include "answer11.h"

typedef unsigned char bits;

typedef struct bitstream_bst
{
    FILE * file;
    bits curr; //Current byte inspected
    bits bitpos; //Index of Bit in the byte curr to be inspected (0-7 are the only values the code will allow)
    
} Bitstream;

HuffNode * huff_unity(HuffNode * h1, HuffNode * h2);
StackNode * StackNode_create(HuffNode * t);
bits readbits(Bitstream * bs, size_t num);

/**
 * Create a new, properly intialized HuffNode
 */
HuffNode * HuffNode_create(int value)
{
    HuffNode * h;
    
    if ((h = malloc(sizeof(HuffNode))) != NULL)
    {
        h -> value = value;
        h->left = NULL;
        h->right = NULL;
    }
    return h;
}

/**
 * Destroy a tree, including all sub-children. Must handle NULL values safely.
 */
void HuffNode_destroy(HuffNode * tree)
{
    if (tree != NULL)
    {
        HuffNode_destroy(tree->left);
        HuffNode_destroy(tree->right);
        free(tree);
    }
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
    Stack * s =  malloc(sizeof(Stack));
    if (s != NULL)
        s->head = NULL;
    return s;
}

/**
 * Frees all memory associated with the stack.
 * Don't forget that you must free the entire contained linked-list.
 * Also, you must safely do nothing if stack == NULL.
 */
void Stack_destroy(Stack * stack)
{
    if (stack == NULL)
        return;
    while(!Stack_isEmpty(stack))
    {
        
        //pop item off stack, then destroy it
        //this frees that Stacknode
        HuffNode_destroy(Stack_popFront(stack));
        
    }
    free(stack);
    
}
/**
 * Returns TRUE (something other than zero) if the stack is empty.
 */
int Stack_isEmpty(Stack * stack)
{
    return stack->head == NULL;
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
    HuffNode * h = NULL;
    StackNode * newhead = NULL;
    if (stack != NULL)
    {
        if (!Stack_isEmpty(stack))
        {
            h = (stack->head) -> tree;
            newhead = (stack->head)->next;
            free(stack->head);
        }
        stack->head = newhead;
    }
    
    return h;
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
    StackNode * sn = StackNode_create(tree);
    sn->next = stack->head;
    stack->head = sn;
    
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
    HuffNode * h1, * h2, * unihuff;
    //pop front 2 nodes
    h1 = Stack_popFront(stack);
    h2 = Stack_popFront(stack);
    //combine huffman trees into single (called it unihuff)
    unihuff = huff_unity(h1, h2);
    //Push noode onto stack
    Stack_pushFront(stack, unihuff);
}

// ---------------------------------------------------- Reading HuffTree headers

/**
 * Read a Huffman Coding Tree (in text format) from 'fp'.
 */
HuffNode * HuffTree_readTextHeader(FILE * fp)
{
    Stack * stack = Stack_create();
    stack->head = NULL;
    HuffNode * ret = NULL;
    
    char op, ch;
    //Load from file
    while (!feof(fp))
    {
        op = fgetc(fp);
        if (op == '0') //poppopcombine
        {
            if (stack->head->next == NULL) //One tree left
            {
                break;
                
            }
            else
                Stack_popPopCombinePush(stack);
            
        }
        else if (op == '1')
        {
            //New node/ tree
            //push to stack
            ch = fgetc(fp);
            Stack_pushFront(stack, HuffNode_create(ch));
        }
    }
    
    ret =  Stack_popFront(stack);
    Stack_destroy(stack);
    return ret;
}

/**
 * Read a Huffman Coding Tree (in binary format) from 'fp'.
 * You will need to (conceptually) read a file a bit at a time. See the README
 * for hints on how to do this.
 */
HuffNode * HuffTree_readBinaryHeader(FILE * fp)
{
    Stack * stack = Stack_create();
    stack->head = NULL;
    HuffNode * ret = NULL;
    bits b;
    
    Bitstream * bs = malloc(sizeof(Bitstream));
    bs-> file = fp;
    bs->bitpos = 0;
    
    while(!feof(bs->file))
    {
        //read 1 bit
        b = readbits(bs, 1);
        //if 0, ppcb
        if (b == 0)
        {
            
            if (stack->head->next == NULL) //ONE TREE LEFT
                break;
            else
                Stack_popPopCombinePush(stack);
            
        }
        //if 1 , read 8 byte
        else if (b == 1) //b == 1
        {
            char c = readbits(bs, 8);
            Stack_pushFront(stack, HuffNode_create(c));
        }
        
        
    }
    ret =  Stack_popFront(stack);
    Stack_destroy(stack);
    free(bs);
    return ret;
}

bits readbits(Bitstream * bs, size_t num)
{
    
    bits result = 0x00; //RESULT of
    bits b; //used to get data from fread.
    bits mask = 0x01<<7; //mask to get MOST SIGNIFICANT BIT
    size_t ind = 0; //Counter
    
    for (ind = 0; ind < num; ind++) //for each bits requested
    {
        if (bs->bitpos == 0) //End of byte, read in a new byte
        {
            fread(& b, sizeof(bits), 1, bs->file);
            bs->curr = b;
        }
        /* append another (next bit in byte) bit to the result
         
         
         works like this:
         right side is obvious the result is left shifted to keep the existing bits (when result == 0x00, it doesn't do anything)
         rigt side gets the byte, and then shifts it by the increment from 0 to 7 ( so it will get the MSB ind = 0 all the way to LSB
         ind = 7). 
         Since Mask is most significant bit of 1, we down shift it another 7to make that single MSB the LSB. 
         other ways to do it , this is my way. ANd then increment the bit pos, and modulo it by 8 so it can only be 0-7 values ( since we only deal with 1 byte)
         */
        result = result << 1 | ((mask & (bs->curr)<<(bs->bitpos)))>>7;
        bs->bitpos = (bs->bitpos + 1) % 8;
    
    }
    return result;
}


StackNode * StackNode_create(HuffNode * t)
{
    StackNode * sn = malloc(sizeof(StackNode));
    if (sn != NULL)
    {
        sn->tree = t;
        sn->next = NULL;
    }
    return sn;
}

HuffNode * huff_unity(HuffNode * h1, HuffNode * h2)
{
    HuffNode * uni = HuffNode_create((h1!=NULL?h1->value:0) + (h2!=NULL?h2->value:0));
    if (h1 == NULL)
        return h2;
    else if (h1 != NULL && h1 == NULL)
        return h1;
    
    if (h1 < h2 && uni != NULL)
    {
        uni->left = h1;
        uni->right = h2;
    }
    else if (uni != NULL)
    {
        uni->left = h2;
        uni->right = h1;
    }
    return uni;
}

