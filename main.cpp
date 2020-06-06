#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>

#define CAPACITY 6
using namespace std;
struct Record{
    int key;
    int rid;
};

bool compare_record(Record a, Record b){ 
    return a.key < b.key;
} 

struct Node{
	Node(){
        keys = new int[CAPACITY];
        rids = new int[CAPACITY];
        children_ptrs = new Node*[CAPACITY+1];
        for (int i = 0; i < CAPACITY+1; i++) children_ptrs[i] = NULL;
        prev = NULL;
        next = NULL;
        parent = NULL;
        size = 0;
    }
    bool is_leaf;
	int *keys;
    int *rids;
    int size;
	Node** children_ptrs;
    Node* prev;
    Node* next;
    Node* parent;
};

class BpTree{
public:
    BpTree(){root = NULL; height = 0;}
    void insert(int data, int rid);
    void insert_internal_key(int key, Node* parent, Node* new_node);
    int search(int data);
    void print(Node* root);
    void printall(Node* root);
    void printnode(Node* node);
    void BulkLoad(Record* data, int size, double fill_factor);
    Node* LinkUp(Node* node);
    void print_ll_starting_bottom();
    void printll(Node* node);
    Node* firstnode();
    bool Only1Link(Node* node);
    Node* nextleaf(Node* node);
    Node* root;
    int height;
};
Node* BpTree::LinkUp(Node* node){
    Node* head_ = NULL;
    Node* tail_ = NULL;
    Node* itor = node;
    while (itor){
        Node* temp = new Node();
        for(int i = 0 ; i < CAPACITY + 1; i++){
            if (itor){
                itor->parent = temp;
                temp->children_ptrs[i] = itor;
                if (i > 0){
                    temp->keys[i-1] = itor->keys[0];
                    temp->size++;
                }
                itor = itor->next;
            } else {
                break;
            }
        }
        if (head_ == NULL){
            head_ = temp;
            tail_ = temp;
            temp->is_leaf = false;
        } else{
            temp->is_leaf = false;
            temp->prev = tail_;
            tail_->next = temp;
            tail_ = temp;
        }
    }
    return head_;
}

bool BpTree::Only1Link(Node* head_node){
    if (head_node->next==NULL){
        return true;
    }
    return false;
}


void BpTree::BulkLoad(Record* data, int size, double fill_factor){
    sort(data, data+size, compare_record);
    int items_per_node = fill_factor/100*CAPACITY;
    //cout<< "Items per node: "<< items_per_node<<endl;
    int num_links = size/items_per_node;
    //cout<< "links needed: "<< num_links<<endl;
    int i = 0;
    Node* head_ = NULL;
    Node* tail_ = NULL;
    while(i < size){
        Node* temp = new Node();
        for (int j = 0; j < items_per_node; j++){
            if (i < size){
                temp->keys[j]=data[i].key;
                temp->rids[j]=data[i].rid;
                temp->size++;
            }
            i++;
        }
        if (head_ == NULL){
            head_ = temp;
            tail_ = temp;
            temp->is_leaf = true;
        } else{
            temp->is_leaf = true;
            temp->prev = tail_;
            tail_->next = temp;
            tail_ = temp;
        }
    }
    Node* temp_ = LinkUp(head_);
    while (Only1Link(temp_)==false){
        temp_ = LinkUp(temp_);
    }
    root = temp_;
}



Node* BpTree::nextleaf(Node* node){
    return NULL;
}

Node* BpTree::firstnode(){
    Node* cursor = root;
    while (cursor->is_leaf == false){
        cursor = cursor->children_ptrs[0];
    }
    return cursor;
}
void BpTree::printll(Node* node){
    Node* itor = node;
    while(itor){
        printnode(itor);
        itor = itor->next;
        cout<<"-> ";
    }
}

void BpTree::print_ll_starting_bottom(){
    Node* itor = firstnode();
    while(itor){
        printnode(itor);
        itor = itor->next;
        cout<<"-> ";
    }
}
void BpTree::printnode(Node* node){
    for (int i = 0; i < node->size; i++){
        cout << node->keys[i] << " ";
    }
}

void BpTree::print(Node* root){
    
    Node* cursor = root;
    if(cursor==NULL) return;
    for(int i = 0; i < cursor->size+1; i++)
    {
        print(cursor->children_ptrs[i]);
    }
    if(cursor->is_leaf){
        for(int i = 0; i < cursor->size; i++)
        {
            cout<<cursor->keys[i]<<" ";
        }
        cout<<"\n";
        return;
    }
}

void BpTree::insert(int data, int rid){
    //cout<< "INSERTING NEW KEY: "<<data<<"-------------------------------------------------"<<endl;
    if (root == NULL){
        //cout<< "INSERTING AT EMPTY ROOT"<< endl;
        root = new Node();
        root->keys[0] = data;
        root->rids[0] = rid;
        root->is_leaf = true;
        root->size = 1;
        //cout << "Inserted at root: " <<data<<endl;
        return;
    }
    // root not empty
    Node* current = root;
    while (current->is_leaf == false){
        for (int i = 0; i < current->size; i++){
            if (data < current->keys[i]){
                current = current->children_ptrs[i];
                break;
            }
            if (i == current->size - 1){
                current = current->children_ptrs[i+1];
                break;
            }
        }
    }
    // Visitor is where key is supossed to be
    if (current->size < CAPACITY){
        //cout<< "INSERTING AT LEAF LESS THAN FULL CAPACITY"<< endl;
        int pos = 0;
        while (data > current->keys[pos] && pos < current->size) pos++;
        for (int i = current->size; i > pos; i--){
            current->keys[i] = current->keys[i-1];
            current->rids[i] = current->rids[i-1];
        }
        current->keys[pos] = data;
        current->rids[pos] = rid;
        current->size++;
        //cout<< "Inserted at leaf no overflow:" << data<<endl;
        //cout<<endl<<"current tree:"<<endl;
        //printall(root);
        //cout<<"End current tree"<<endl<<endl;
        return;
    }
    // overflow, need to split
    Node* new_node = new Node();
    new_node->parent = current->parent;
    new_node->next = current->next;
    current->next = new_node;
    //look where the split should happen
    int pos = 0;
    //cout<< "Need to split to insert " <<data<< " data:";
    for(int i = 0; i< current->size ; i++){
        //cout<< current->keys[i] << " ";
    }
    //cout<<endl;
    while (data > current->keys[pos] && pos < current->size) pos++;
    //cout<< "pos to split on: " << pos << endl;  
    int long_node_keys[CAPACITY + 1];
    int long_node_rids[CAPACITY + 1];
    for (int i = 0; i < pos; i++){
        long_node_keys[i] = current->keys[i];
        long_node_rids[i] = current->rids[i];
    }
    long_node_keys[pos] = data;
    long_node_rids[pos] = rid;
    for (int i = pos+1; i < CAPACITY + 1; i++){
        long_node_keys[i] = current->keys[i-1];
        long_node_rids[i] = current->rids[i-1];
    }
    for(int i = 0; i< CAPACITY + 1 ; i++){
        //cout<< long_node_keys[i] << " ";
    }
    //cout<<endl;
    new_node->is_leaf = true;
    //check if total size is even or odd
    int new_size;
    new_size = (CAPACITY+1)/2;
    //cout<<"new size: "<<new_size<<endl;
    new_node->size = new_size;
    current->size = CAPACITY+1 - new_size;

    for (int i = 0; i < current->size; i++){
        current->keys[i] = long_node_keys[i];
        current->rids[i] = long_node_rids[i];
    }
    //cout<<"Left node data: ";
    for(int i = 0; i< current->size ; i++){
        //cout<< current->keys[i] << " ";
    }
    //cout<<endl;
    for (int i = 0; i < new_node->size; i++){
        new_node->keys[i] = long_node_keys[i + current->size];
        new_node->rids[i] = long_node_rids[i + current->size] ;
    }
     //cout<<"Right node data: ";
    for(int i = 0; i< new_node->size ; i++){
        //cout<< new_node->keys[i] << " ";
    }
    
    //cout<<endl;
    if (current == root){
        //cout<< "INSERTING AT SPLIT ROOT"<< endl;
        Node* new_root = new Node();
        new_root->keys[0] = new_node->keys[0];
        new_root->children_ptrs[0] = current;
        new_root->children_ptrs[1] = new_node;
        new_root->size = 1;
        new_root->is_leaf = false;
        root = new_root;
        new_node->parent = root;
        current->parent = root;
        //cout<<endl<<"current tree:"<<endl;
        //printall(root);
        //cout<<"End current tree"<<endl<<endl;

        return;
    }
    // it is not the root, need to send key up
    
    insert_internal_key(new_node->keys[0],current->parent,new_node);
    
}
void BpTree::printall(Node* current){
    Node* cursor = current;
    if(cursor==NULL) return;
    if (cursor->is_leaf){
        //cout<< "LEAF:";
    } else {
        //cout<< "NOT LEAF: ";
    }
    for(int i = 0; i < cursor->size; i++)
    {
        cout<<cursor->keys[i]<<" ";
    }
    cout<<"\n";

    for(int i = 0; i < cursor->size+1; i++)
    {
        printall(cursor->children_ptrs[i]);
    }
}


void BpTree::insert_internal_key(int key, Node* node_to_insert_in, Node* new_right_node){
        //cout<<"INSERTING_INTERNAL_KEY: " << key << " AT:"<<endl;
    for (int i = 0; i< node_to_insert_in->size ;i++){
        //cout<< node_to_insert_in->keys[i]<< " ";
    }
    //cout<< "\nSIZE: " << node_to_insert_in->size<<endl;

    Node* current = node_to_insert_in;

    //print(root);
    //cout<<endl;
    //cout<<"key to insert:: "<< key<<" in non leaf node :" << endl;
    for (int i = 0; i<current->size; i++){
        //cout << current->keys[i]<< " ";
    }
    
    ////cout<<endl<<current->size<<CAPACITY<<endl;
    
    if (current->size < CAPACITY){
        //cout<< "INSERTING INTERNAL AT LESS THAN FULL CAPACITY"<< endl;
         if (1){
            
             //exit(1);
             }   
        
        int pos = 0;
        
        while (key > current->keys[pos] && pos < current->size) pos++;
       
        for (int i = current->size; i > pos; i--){
            current->keys[i] = current->keys[i-1];  // no rids stored in middle nodes
        }
        
        for (int i = current->size + 1; i > pos+1; i--){
            current->children_ptrs[i]= current->children_ptrs[i-1];
        }
        
        current->keys[pos] = key;
        
        current->size++;
        current->children_ptrs[pos + 1] = new_right_node;
        //cout<<endl<<"current tree:"<<endl;
        //printall(root);
        //cout<<"End current tree"<<endl<<endl;
        
        
        return;
        
    }
    //cout<< "INSERTING INTERNAL AND SPLITTING"<<endl;
    Node* new_internal_node = new Node();

    new_internal_node->parent = current->parent;
    int long_node_keys[CAPACITY + 1];
    Node* long_node_children_ptrs[CAPACITY + 2];
    for (int i = 0; i< current->size; i++){
        long_node_keys[i] = current->keys[i];
    }

    for (int i = 0; i< current->size + 1;i++){
        long_node_children_ptrs[i] = current->children_ptrs[i];
    }
    int pos = 0;                                                                                                                                        
    while (key > long_node_keys[pos] && pos < current->size) pos++;
    //cout<<"POSITION TO INSERT:"<< pos<<endl;
    int size_long = CAPACITY + 1;
    for (int i = size_long -1 ; i > pos; i--){
        long_node_keys[i] = long_node_keys[i-1];
    }
    for (int i = size_long;i > pos + 1; i--){
        long_node_children_ptrs[i] = long_node_children_ptrs[i-1];
    }
    long_node_keys[pos] = key;
    long_node_children_ptrs[pos+1] = new_right_node;

    //for (int i = 0; i < size_long; i++){
    //    //cout << long_node_keys[i]<< " ";
    //}
    //cout<<endl;
    //for (int i = 0; i < size_long+1; i++){
    //    //cout << long_node_children_ptrs[i]->keys[0]<< " ";
    //}
    //cout<<endl;
    new_internal_node->is_leaf = false;
    bool cap_is_even;
    bool cap_is_odd;
    if (CAPACITY % 2==0){
        cap_is_even = true;
        cap_is_odd= false;
        //cout<<"EVEM";
    } else{
        cap_is_even = false;
        cap_is_odd= true;
    }
    int left_node_size;
    int right_node_size;
    if (cap_is_even){
        left_node_size = CAPACITY/2;
        right_node_size = left_node_size;
    } else {
        left_node_size = CAPACITY/2 +1;
        right_node_size = CAPACITY - left_node_size;
    }
    //cout<<endl<<"LS: "<<left_node_size<< "RS: "<< right_node_size<<endl;

    for (int i = 0;i < left_node_size;i++){
        current->keys[i] = long_node_keys[i];
        current->children_ptrs[i] = long_node_children_ptrs[i];
        if (i == left_node_size - 1){
            current->children_ptrs[i+1] = long_node_children_ptrs[i+1];
        }
    }
    for (int i = 0; i < right_node_size; i++){
        
        new_internal_node->keys[i] = long_node_keys[i + left_node_size + 1];
        new_internal_node->children_ptrs[i] = long_node_children_ptrs[i + left_node_size + 1];
        new_internal_node->children_ptrs[i]->parent = new_internal_node;
        if (i == right_node_size -1){
            new_internal_node->children_ptrs[i + 1] = long_node_children_ptrs[i + left_node_size + 1 + 1];
            new_internal_node->children_ptrs[i + 1]->parent = new_internal_node;
        }

    }
    current->size = left_node_size;
    new_internal_node->size = right_node_size;


    int removed_middle_key = long_node_keys[left_node_size]; 

    
    //cout << "long node to split:"<<key<<endl;
    for (int i = 0; i < CAPACITY + 1; i++){
        //cout<< long_node_keys[i] << " ";
    }
    //cout<<endl;
    //cout << "----------------------new split left internal node:"<<endl;
    
    for (int i = 0; i < current->size; i++){
        //cout<< current->keys[i] << " ";
    }
    //cout<<endl;
    //cout << "-----------------------new split right internal node:"<<endl;    
    for (int i = 0; i < new_internal_node->size; i++){
        //cout<< new_internal_node->keys[i] << " ";
    }
    //cout<<endl;
    //cout<<"----------"<<endl;
    //cout<<endl;
    //exit(1);
    if (current == root){
        //cout<< "INSERTING INTERNAL ATTT SPLIT ROOT------------------------------------- "<< endl;
        Node* newroot = new Node();
        newroot->keys[0] = removed_middle_key;
        newroot->children_ptrs[0] = current;
        newroot->children_ptrs[1] = new_internal_node;
        newroot->is_leaf = false;
        newroot->size = 1;
        root = newroot;
        new_internal_node->parent = root;
        current->parent = root;
        //printall(root);
        return;
    }
    insert_internal_key(removed_middle_key,current->parent,new_internal_node);
    
}


int main(){
    BpTree tree;
    fstream file;
    file.open("input_data.txt");
    Record data[1000000];
    int size = 0;
    while(file >> data[size].key >> data[size].rid){
        //tree.insert(data[size].key,data[size].rid);
        size++;
    }
    tree.BulkLoad(data, size,75);
    tree.print(tree.root);
    return 0;
}
