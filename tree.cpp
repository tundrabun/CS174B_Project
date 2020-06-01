#include <iostream>
#include <vector>

#define NODESPACE 4

struct node {
    std::vector<int> data;
    std::vector<node*> pointers;
    
    bool leaf;
    int num;

} *root = NULL, *newnode = NULL, *x = NULL;

node* createTree(){
    newnode = new node;
    newnode->data = std::vector<int>(4);
    newnode->pointers = std::vector<node*>(5);
    newnode->leaf = true;
    newnode->num = 0;

    for(int i = 0; i < NODESPACE; i++)
        newnode->pointers[i] = NULL;

    return newnode;
}

int split(node* n, int i) {
    int j, middle;
    node *new1, *new3, *child;

    new3 = createTree();
    //new3->leaf = true;

    if(i == -1) {
        middle = n->data[2];
        n->num -= 1;

        new1 = createTree();
        new1->leaf = false;
        n->leaf = true;

        for(int j = 3; j < NODESPACE; j++) {
            new3->data[j-3] = n->data[j];
            new3->pointers[j-3] = n->pointers[j];
            new3->num += 1;

            n->data[j] = 0;
            n->num -= 1;
        }
        for(int j = 0; j < NODESPACE+1; j++) 
            n->pointers[j] = NULL;

        new1->data[0] = middle;
        new1->pointers[new1->num] = n;
        new1->pointers[new1->num + 1] = new3;
        new1->num += 1;
        root = new1;
    }
    else {
        child = n->pointers[i];
        middle = child->data[2];
        child->data[2] = 0;
        child->num -= 1;

        for(int j = 3; j < NODESPACE; j++) {
            new3->data[j-3] = child->data[j];
            new3->num += 1;

            child->data[j] = 0;
            child->num -= 1;
        }
        n->pointers[i+1] = child;
        n->pointers[i+1] = new3;
    }

    return middle;
}

void sortTree( std::vector<int> arr, int n ) {
    for(int i = 0; i<n; i++) {
        for(int j = i; j<n; j++){
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
        if(x->leaf && x->num == NODESPACE) {
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

                if( (x->pointers[j])->num == NODESPACE) {
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
    std::cout << std::endl;
    int count;
    for( count = 0; count < n->num; count++) {
        if(n->leaf == false) { printTree(n->pointers[count]);}
        std::cout << " " << n->data[count];
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
    std::cout << "Hello World" << std::endl;

    printTree(root);
}