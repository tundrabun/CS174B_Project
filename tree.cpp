#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#define ORDER 5

struct node {
    int* data;
    node** pointers;
    
    bool leaf;
    int num;

} *root = NULL, *newnode = NULL, *x = NULL;

node* createTree(){
    newnode = new node;
    newnode->data = new int[ORDER];
    newnode->pointers = new node*[ORDER + 1];
    newnode->leaf = true;
    newnode->num = 0;

    for(int i = 0; i < ORDER+1; i++)
        newnode->pointers[i] = NULL;

    return newnode;
}


int split(node* n, int i) {
    int j, middle;
    node *left, *right, *child;

    right = createTree();
    right->leaf = true;

    if(i == -1) {
        middle = n->data[2];
        n->data[2] = 0;
        n->num -= 1;

        left = createTree();
        left->leaf = false;
        n->leaf = true;

        for(int j = 3; j < ORDER; j++) {
            right->data[j-3] = n->data[j];
            right->pointers[j-3] = n->pointers[j];
            right->num += 1;

            n->data[j] = 0;
            n->num -= 1;
        }
        for(int j = 0; j < ORDER+1; j++) 
            n->pointers[j] = NULL;

        left->data[0] = middle;
        left->pointers[left->num] = n;
        left->pointers[left->num + 1] = right;
        left->num += 1;
        root = left;
    }
    else {
        child = n->pointers[i];
        middle = child->data[2];
        child->data[2] = 0;
        child->num -= 1;

        for(int j = 3; j < ORDER; j++) {
            right->data[j-3] = child->data[j];
            right->num += 1;

            child->data[j] = 0;
            child->num -= 1;
        }
        std::cout << "t1: " << child->data[0] << " " << child->data[1] << std::endl <<"t2: " << right->data[0]<< " " << right->data[1] << std::endl;
        
        n->pointers[i+2] = child;
        n->pointers[i+2] = right;
        
    }

    return middle;
}


void sortTree( int* arr, int n ) {
    for(int i = 0; i<n; i++) {
        for(int j = i; j<=n; j++){
            int temp;
            if( arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    return;
}


void insert( int i ) {
    int temp;
    int j;
    x = root;

    if( x == NULL) {
        root = createTree();
        x = root;
    } 
    else {
        if(x->leaf && x->num == ORDER) {
            temp = split(x, -1);
            x = root;

            for(j = 0; j < (x->num); j++) {
                if( i > x->data[j] && i < x->data[j+1]) {
                    j++;
                    break;
                }
                else if( i < x->data[0] ) break;
                else continue;
            }
            x = x->pointers[j];
        }
        else {
            while(!(x->leaf)) {
                for(j = 0; j < x->num; j++){
                    if( i > x->data[j] && i < x->data[j+1]) {
                        j++;
                        break;
                    }
                    else if( i < x->data[0] ) break;
                    else continue;
                }

                if( (x->pointers[j])->num == ORDER) {
                    temp = split(x,j);
                    x->data[x->num] = temp;
                    x->num += 1;
                    continue;
                }
                else {x = x->pointers[j];}
            }
        }
    }

    x->data[x->num] = i;
    sortTree(x->data, x->num);
    x->num += 1;
    return;
}



void printTree(node* n) {
    std::cout<<std::endl;
    int count;

    if(!n) return;

    for( count = 0; count < n->num; count++) {
        if(n->leaf == false)
            printTree(n->pointers[count]);

        std::cout << n->data[count] << " ";
    }
    if( n->leaf == false) { printTree(n->pointers[count]);}
    std::cout << std::endl;
}


node* searchHelper(node* n, int s) {
    int j;
    for(j=0; j < n->num; j++) {
        if(n->leaf == false) { searchHelper(n->pointers[j],s);}

        if(n->data[j] == s) return n;
    }

    if(n->leaf == false) { searchHelper(n->pointers[j],s);}

    return NULL;
}

node* search( int i ) {
    int j;
    node* temp = root;

    return searchHelper(root, i);
}

int main() {

    
    srand( time(NULL) );
    for(int i = 0; i < 50; i++) {
        int temp = rand()%100 + 1;
        std::cout << "Inserting number " << i+1 << " which is " << temp << std::endl;
        insert(temp);
        printTree(root);
    }

    std::cout << "Printing Tree" << std::endl;
    std::cout << "test:" << root->pointers[0]->data[0] << std::endl;
    printTree(root);
}