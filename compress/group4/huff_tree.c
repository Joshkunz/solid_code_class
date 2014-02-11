/*
 * Huff tree model.
 *
 * Jonathon McDonald, Lynn Gao, Taylor Stapleton
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huff_tree.h"
#include "huff_io.h"

// stores the bit code result from traversing the tree 
char bit_code[255] = {0};

/*
 * Traverses the Huffman tree and assigns 0 or 1 to 
 * children to create the bit code for a char
 */
char * traverse_tree(struct tree_node * tree, char * bit_code)
{  
	
  char temp[255] = ""; //worst case height for huffman tree  
  if(tree->left != NULL) {    
      strcpy(temp, bit_code);
      strcat(temp, "0");
      traverse_tree(tree->left, temp);
    }
  if(tree->right != NULL) { 
      strcpy(temp, bit_code);
      strcat(temp,"1");
      traverse_tree(tree->right, temp);
    }  
  bit_code = temp; // store bit code in bit_code var
  return bit_code;
}

void print_huff(struct tree_node * temp, char * code)
{
	if(temp->left == NULL && temp->right == NULL){
		printf("%s\n", code);
		return;
	}
	int length = strlen(code);
	char left_code[255];
	char right_code[255];
	strcpy(left_code, code);
	strcpy(right_code, code);
	left_code[length] = '0';
	left_code[length+1] = '\0';
	right_code[length] = '1';
	right_code[length+1] = '\0';
	print_huff(temp->left, left_code);
	print_huff(temp->right, right_code);
}



void traverse_pq(struct pq_node * node)
{
    while (node != NULL) {
        printf("node->priority: %d ", node->priority);
        if (node->content != NULL)
            if (node->content->current != -1)
                printf("node->char: %c\n", (char)node->content->current);
            else 
                printf("empty node\n");
        else
            printf("node->content is null\n");

        node = node->next;
    }
}



/*
 * Compares two Huffman nodes using weight
 */
int compare_to(struct tree_node* left, struct tree_node* right)
{
    if (left->weight < right->weight) {
        return -1; // if right greater than left
    }
    else if (left->weight > right->weight) {
        return 1; // if left greater than right
    } else {
        // use a tie breaker if nodes have equal weights
        if (left->current < right->current) {
            return -1; // left node goes before
        } else if (left->current > right->current) {
            return 1; // right node goes before
        } else {
            return 0; // nodes are assumed equal, this should never happen
        }      
    }
}


/*
 * Returns the leftmost leaf for inorder sort 
 */
int get_leftmost (struct tree_node *tn)
{
    while (tn->left != NULL) {
        tn = tn->left;
    }
    return tn->current;
}


/* 
 * Add a node to the priority queue
 */
struct pq_node * enqueue (struct pq_node *head, struct pq_node *p)
{
    struct pq_node* prev = NULL; // represents this pq we are adding to  
    struct pq_node* current = head;

    if (current->priority < p->priority) {
        p->next = current;
        return p;
    }

    while (current != NULL) {
        if (current->priority < p->priority) {
            break;
        }

        prev = current;
        current = current->next;
    }

    prev->next = p;
    p->next = current;
    return head;
}


/*
 * Remove a node from the priority queue 
 */
tree_node * dequeue (struct pq_node *head)
{
    tree_node * ret = head->content;
    return ret;
}


/*
 * Prints the characters of nodes in a huffman tree
 */
void print_tree (struct tree_node head)
{
	 // didn't print the code code on CADE machines?
    if (head.current != -1)
        printf("%c\n", (char)head.current);

    if (head.left != NULL)
        print_tree((*head.left));

    if (head.right != NULL)
        print_tree((*head.right));
	
}


/* 
 * Creates a priority queue (pq_node) of nodes in the Huff tree (tree_node)
 */
struct pq_node * make_pq (struct frequency table[])
{
    int i;
    int node_count = 0; // keep track of the number of nodes in the tree for length of huffman tree
    struct pq_node *head = NULL; // start of pq
    struct pq_node *prev = NULL; // prev initially NULL  
  
    //get all character frequencies in file 
    // and add to priority queue
    for (i = 0; i < 256; i++) {    
        node_count = node_count + table[i].count;
        tree_node * new_tnode = new_tree_node(NULL, NULL, NULL, 
            table[i].count, (int)table[i].character);

        struct pq_node * current = new_pq_node(new_tnode->weight, NULL, 
            new_tnode);

        if (head == NULL)
            head = current;
        else 
            prev->next = current;   
        

        prev = current;    
    }

    return head;
}


/*
 * Builds a Huffman tree for each character in terms of bit codes
 */
struct tree_node build_tree (struct pq_node * pq)
{    
    struct pq_node *head = pq;
    struct tree_node * root = NULL;

    // start building tree
    while (head->next != NULL) {
        // grab 2 smallest nodes 
        struct tree_node *lt = dequeue(head);
        head = head->next;
        struct tree_node *rt = dequeue(head);
        head = head->next;

        // create a new node with smallest nodes as children
        struct tree_node *pt = new_tree_node (NULL, lt, rt, 
            lt->weight + rt->weight, -1);

        if (head == NULL) {
            root = pt;
            break;
        }

        // associate children with parent
        lt->parent = pt;
        rt->parent = pt;

        // enqueue new node to priority queue
        struct pq_node *new_node = new_pq_node(pt->weight, NULL, pt);
        head = enqueue(head, new_node); // SEG FAULTS HERE
    }
    
    if (root == NULL) {
        // when the loop ends, set remaining node as tree at root
        root = dequeue(head);  
    }
    
    return (*root);
}


/*
 * Initializes a new priority queue node in the heap.
 */
struct pq_node * new_pq_node (int priority, struct pq_node * next, 
    tree_node * content)
{
    char *space = malloc(sizeof(struct pq_node));
    struct pq_node * node = (struct pq_node *)space;

    node->priority = priority;
    node->next = next;
    node->content = content;

    return node;
}


/*
 * Initializes a new tree node in the heap.
 */
tree_node * new_tree_node (tree_node *parent, tree_node *left, tree_node *right,
    int weight, int current)
{
    char *space = malloc(sizeof(tree_node));
    tree_node * node = (tree_node *)space;

    node->parent = parent;
    node->left = left;
    node->right = right;
    node->weight = weight;
    node->current = current;

    return node;
}

/* 
 * Returns the number of total leaves in the tree
 */
int check_rep(tree_node * parent)
{
  if(parent == NULL) {
    return 0;
  }
  else if(parent->left == NULL && parent->right == NULL) {
    return 1;
  }
  else {
    return (check_rep(parent->left)+check_rep(parent->right));
  }
  return 0;  
}

void compress(char * filename, struct frequency table[])
{
  // get frequency table
  build_table(filename, table);
  //build huffman tree
  struct pq_node * queue;
  struct tree_node tree;
  queue = make_pq(table);
  tree = build_tree(queue);
  //generate huffman codes for chars
  
  //write the compressed file
  FILE * outfile = fopen(filename, "w");
  if(is_huff(outfile, filename)){
    return;
  }
  else{
    //printf("%s\n", filename);
  }
  
  write_encoding(outfile);
  //for each content in original file, output huffman code for content
  //close the compressed file
  
}

void write_encoding(FILE * outfile)
{
  // if file doesn't exist
  if(outfile == NULL) {
    printf("File does not exist \n");
    exit(255);
  }
  // write magic number 0-3 HUFF
  // write length field 4-11
  // write compression table 12-
  // write compressed data + padding
}

