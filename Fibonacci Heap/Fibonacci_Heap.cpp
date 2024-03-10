#include <iostream>
#include <unordered_map>
#include <string>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstdio>

using namespace std;


struct heap_node {
	
	int key;
	int value;
	int rank;
	bool marked;
	heap_node* parent;
	heap_node* child;
	heap_node* left;
	heap_node* right;

	heap_node(int _key, int _val) { key = _key; value = _val; rank = -1; marked = false; parent = nullptr; 
	left = nullptr; right = nullptr; child = nullptr; }
};


class fibonacci_heap {

private:
	heap_node* max_node;
	int numOfnodes; 

void link(heap_node* y, heap_node* x) {

    y->left->right = y->right;
    y->right->left = y->left;
    
    if (x->child != nullptr) {
		x->child->left->right = y;
		y->left = x->child->left;
		x->child->left = y;
		y->right = x->child;
    } else {
		x->child = y;
		y->right = y;
		y->left = y;
     }
    y->parent = x;
    x->rank = x->rank + 1;
    y->marked = false;

    return;
}


void consolidate() {
	heap_node** A, ** rootlist;
  	int d, rootSize;
    
  	int max_rank = log2(numOfnodes) + 1;
   	A = new heap_node*[max_rank+2]; 
    
    fill_n(A, max_rank + 2 , nullptr);
    
    heap_node* w = max_node;
    rootSize = 0;
    heap_node* next = w;
    do
      {
	rootSize++;
	next = next->right;
      } while ( next != w );
    rootlist = new heap_node*[rootSize];
	for ( int i = 0; i < rootSize; i++ )
      {
	rootlist[i] = next;
	next = next->right;
      }

	for(int i=0; i < rootSize; i++) {

		w = rootlist[i];
		heap_node* x = w;
		int d = x->rank;

		while ( A[d] != nullptr ) {
	    
	    heap_node* y = A[d];
	
	    if (y->key > x->key) {
		
		heap_node* temp = x;
		x = y;
		y = temp;
	     }
		
		link(y,x);
	    
	    A[d] = nullptr;
	    d++;
	  }
	  A[d] = x;
	}

	max_node = nullptr;

	for (int i = 0;i < max_rank +2; i++) {
		
		if ( A[i] != nullptr ) {
	    
	    	if ( max_node == nullptr ) {
				max_node = A[i]->left = A[i]->right = A[i];
	      	} else
	    {
		
			max_node->left->right = A[i];
			A[i]->left = max_node->left;
			max_node->left = A[i];
			A[i]->right = max_node;
		
			if (A[i]->key > max_node->key) {
		    
		    	max_node = A[i]; }
	     	}
	  	}
    }
    delete [] A;
    delete [] rootlist;
}


void cut(heap_node* x, heap_node* y) {

	if ( x->right == x ) {
		y->child = nullptr;
    } else {

	x->right->left = x->left;
	x->left->right = x->right;
	
	if (y->child == x) {
	    y->child = x->right;
		}
    }

    y->rank = y->rank - 1;
    
    max_node->right->left = x;
    x->right = max_node->right;
    max_node->right = x;
    x->left = max_node;
    

    x->parent = nullptr;
    x->marked = false;
}

void cascadingCut(heap_node* y) {
	heap_node* z = y->parent;
	if (z != nullptr) {
		
	if (y->marked == false) {
	    y->marked = true;
	} else {
	    
	    cut(y,z);
	    cascadingCut(z);
		}
	}
}

public:

fibonacci_heap() {
	
	max_node = nullptr;
	numOfnodes = 0;
}

void insert(heap_node* x) {

	x->rank = 0;
    x->parent = nullptr;
    x->child = nullptr;
    x->marked = false;
    
    if (max_node == nullptr) {
		max_node = x->left = x->right = x;
    } else {
	
	max_node->left->right = x;
	x->left = max_node->left;
	max_node->left = x;
	x->right = max_node;
	
	if (x->key > max_node->key) {
	    max_node = x;
	  }
    }
    numOfnodes++;
}

bool isEmpty() {
    return max_node == nullptr; 
}


heap_node* extractMax() {
    heap_node* max = max_node;
    heap_node** childlist;

    if (max != nullptr) {
    	heap_node* child = max->child;
    	if (child != nullptr) {
    		childlist = new heap_node*[max->rank];

    		heap_node* sibling = child;
    		for (int i = 0;i < (int)max->rank;i++) {
				childlist[i] = sibling;
				sibling = sibling->right;
	      	}
	      	for (int i=0; i < (int)max->rank ;i++) {
				sibling = childlist[i];
				max_node->left->right = sibling;
				sibling->left = max_node->left;
				max_node->left = sibling;
				sibling->right = max_node;
				sibling->parent = nullptr;
		      }
	    		delete [] childlist;
	  		}
	  	max->right->left = max->left;
	  	max->left->right = max->right;

	  	if ( max == max->right ) { max_node = nullptr;
	  	} else {
	    	max_node = max->right;
	    
	    	consolidate();
	  	}

		numOfnodes--;	
	 }
		return max;
}


heap_node* findMax() {
    return max_node;
}


void increaseKey(heap_node* x, int k) {
    if (x->key > k) {
    	cout << "new key is smaller than current key" << endl;
    	return;
    }

    x->key = k;
    heap_node* y = x->parent;
    if(y != nullptr && x->key > y->key) {
    	cut(x,y);
    	cascadingCut(y);
    }
    if (x->key > max_node->key) {
    	max_node = x;
    }
}

void Delete(heap_node* x) {
	increaseKey(x, INT_MAX);
	heap_node* node = extractMax();
	delete node;
}


void delete_tree(heap_node* root) {
	if(!root) return;

	heap_node *cur = root;
    while(true) {
	
	if (cur->left && cur->left != root) {
	    
	    heap_node *temp = cur;
	    cur = cur->left;
	    if (temp->child)
	      	delete_tree(temp->child);
	    
	    delete temp;
	} else {

	    if (cur->child)
	      delete_tree(cur->child);
	    delete cur;
	    break;
	 	}
    }
}

void clear() {
	delete_tree(max_node);
	max_node = nullptr;
	numOfnodes = 0;
}

void print_tree(heap_node* z) {

	heap_node* c = z->child;
	cout << "(" << z->key << "," << z->value << ") -> ";

	if (c != nullptr) {
	do {
		if (c->parent == z) {
			cout << "(" << c->key << "," << c->value << ")";
		} c = c->right;
	} while (c != z->child);
	}
	cout << endl;
}


void print_children(heap_node* z) {
	
	if (z->child == nullptr) return;

	print_tree(z);
}


void print() {
	if (max_node == nullptr) {
		cout << "The Heap is empty" << endl;
		return;
	}

	vector<heap_node*> rootlist;
	heap_node* w = max_node;
	rootlist.push_back(max_node);
	w = w->right;
	while(w != max_node) {
		rootlist.push_back(w);
		w = w->right;
	}

	heap_node *z, *c, *s;
	for (int i = 0; i < rootlist.size(); i++) {
		z = rootlist[i];
		cout << "Tree " << i+1 << ": ";
		print_tree(z);
		c = z->child;
		while (c != nullptr) {

			s = c;
			do {

				print_children(s);
				s = s->right;

			} while (s != c);
		c = c->child;	
		}
	}
}

heap_node*& get_max_node() {
        return max_node;
}

void set_n(int n) {
	numOfnodes = n;
}	

int get_n() {
	return numOfnodes;
}

};



class max_priority_queue {
	
private:
	
	unordered_map<int, heap_node*> nodes;
public:


	fibonacci_heap* make_heap() {
		fibonacci_heap* new_heap = new fibonacci_heap();
		return new_heap;
	}

	bool is_empty(fibonacci_heap* heap) {
		return heap->isEmpty();
	}

	void insert(fibonacci_heap* heap, int key, int value) {
		heap_node* x = new heap_node(key, value);
		nodes[value] = x;
		heap->insert(x);
	}

	int extract_max(fibonacci_heap* heap) {
		heap_node* z = heap->extractMax();
		int val = z->value;
		delete z;
		nodes[val] = nullptr;
		return val;
	}

	int find_max(fibonacci_heap* heap) {
		return heap->findMax()->value;
	}

	void increase_key(fibonacci_heap* heap, int value, int new_key) {
		heap_node* node = nodes[value];
		heap->increaseKey(node, new_key);
	}

	void delete_node(fibonacci_heap* heap, int value) {
		heap_node* node = nodes[value];
		heap->Delete(node);
	}

	fibonacci_heap* meld(fibonacci_heap* heap1, fibonacci_heap* heap2) {
		heap_node* max1 = heap1->findMax(); 
		heap_node* max2 = heap2->findMax();
		
		if (!max1) return heap1;
		if (!max2) return heap2;

		fibonacci_heap* new_heap = new fibonacci_heap;
		
		heap_node* max = max1;

		max1->right->left = max2->left;
		max2->left->right = max1->right;
		max1->right = max2;
		max2->left = max1;

		if (max1->key > max2->key) {
			max = max1;
		} else { max = max2; }

		new_heap->get_max_node() = max;

		new_heap->set_n(heap1->get_n() + heap2->get_n());
		return new_heap;
	} 

	void print(fibonacci_heap* heap) {
		heap->print();
	}

void test() {
	fibonacci_heap* heap = make_heap();

	int values[] = {31, -19, 42, 2, -4, 7, -17, 28, 14, -1, 36, 10, 4};
	int size = sizeof(values)/sizeof(int);

	for (int i = 0; i < size; ++i)
	{
		insert(heap, values[i], i+1);
	}
	cout << "After First inserts: \n\n";
	print(heap);
	cout << "\nextract_max consolidates the trees and forms the following tree\n";
	cout << "\nthe max value we extract: "; 

	cout << extract_max(heap) << endl;
	print(heap);
	cout << "\nThe current max value: ";
	cout << find_max(heap) << endl;

	cout << "\nincreasing key of value 6 to 29\n";
	increase_key(heap,6, 29);
	print(heap);

	cout << "\nincreasing key of value 13 to 64\n";
	increase_key(heap, 13, 60);
	print(heap);

	cout << "\ndeleting (-4,5)\n";
	delete_node(heap, 5);
	print(heap);

	cout << "\ndeleting (14,9)\n";
	delete_node(heap, 9);
	print(heap);

	cout << "\nWe create a second heap\n" << endl;

	fibonacci_heap* heap2 = make_heap();
	int values2[] = {64, -23, 3, -5, 12, 22, 39};
	int size2 = sizeof(values2)/sizeof(int);

	for (int i = 0; i < size2; ++i)
	{
		insert(heap2, values2[i], i+20);
	}
	cout << "\nsecond heap after only inserts\n" << endl;
	print(heap2);
	cout << "\nMelding the 2 heaps\n" << endl;
	fibonacci_heap* heap3 = meld(heap,heap2); 
	print(heap3);
	cout << "\nconsolidating this heap with extract max\n the current max: " << endl;
	cout << extract_max(heap3) << endl;
	print(heap3);

	cout << "\n PASSED \n";

	heap->clear();
	heap2->clear();
	
}

};




int main() {

	//freopen("out.txt", "w", stdout);

	max_priority_queue pq;

	pq.test();

	return 0;
}