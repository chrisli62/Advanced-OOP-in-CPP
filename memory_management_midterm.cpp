#include <iostream>
#include <stdlib.h>

using namespace std;

// Memory Management Midterm
// Name: Christopher Li
// Block Class Code starts at line 107

template <class T> class SA;
template <class T> ostream& operator<<(ostream& os, SA<T> s);

// Using safe array code provided via
// https://venus.cs.qc.cuny.edu/~waxman/780/safe%20array.pdf

template <class T> class SA{
private:
  int low, high;
  T *p;
public:

  // default constructor
  // allows for writing things like SA a;

  SA(){low=0; high=-1;p=NULL;}


  // 2 parameter constructor lets us write
  // SA x(10,20);

  SA(int l, int h){
    if((h-l+1)<=0){
      cout<< "constructor error in bounds definition"<<endl;
      exit(1);
    }
    low=l;
    high=h;
    p=new T[h-l+1];
  }


  // single parameter constructor lets us
  // create a SA almost like a "standard" one by writing
  // SA x(10); and getting an array x indexed from 0 to 9

  SA(int i){
    low=0; high=i-1;
    p=new T[i];
  }
  // copy constructor for pass by value and
  // initialization

  SA(const SA & s){
    int size=s.high-s.low+1;
    p=new T[size];
    for(int i=0; i<size; i++)
      p[i]=s.p[i];
    low=s.low;
    high=s.high;
  }
  // destructor

  ~SA(){
    delete [] p;
  }
  //overloaded [] lets us write
  //SA x(10,20); x[15]= 100;

  T & operator[](int i){
    if(i<low || i>high){
      cout<< "index "<<i<<" out of range"<<endl;
      exit(1);
    }
    return p[i-low];
  }
 
  // overloaded assignment lets us assign
  // one SA to another
  SA & operator=(const SA & s){
    if(this==&s)return *this;
    delete [] p;
    int size=s.high-s.low+1;
    p=new T[size];
    for(int i=0; i<size; i++)
      p[i]=s.p[i];
    low=s.low;
    high=s.high;
    return *this;
  }

 // overloads << so we can directly print SAs
  friend ostream& operator<<(ostream& os, SA s);
};

template <class T> ostream& operator<<(ostream& os, SA<T> s){
  int size=s.high-s.low+1;
  for(int i=0; i<size; i++)
  cout<<s.p[i]<<endl;
  return os;
};

// Block Class

class Block {
  private:
    int totalBlocks;
    int freeBlocks;
    int avgBlockSize;
    int remainingBlocks;
    
  public:

    //Default Constructor
    struct Node {
      int size;
      bool tag; // Containing part of address of data fetched from main memory
      Node *next;
      union {
        Node *prev; 
        Node *rep;
      };
    };

    Node *freeList;

    // Note: similar structure to the SA class
    // default constructor
    // Must initialize the block then allocate the memory
    Block() {
      initialize(1000);
      allocate(1000, freeList);
    }

    Block(int size) {
      if(size < 0) {
        cout << "ERROR: Size cannot be a negative number";
        return;
      }
      initialize(size);
      allocate(size, freeList);
    }

    // Using "int size=s.high-s.low+1;" and "p=new T[size];"
    Block(int low, int high, int size) {
      if(((high-low+1) * size) < 0) {
        cout << "ERROR: SA Failure";
        return;
      }
      initialize((high-low+1) * size);
      allocate((high-low+1) * size, freeList);
    }

    //destructor
    ~Block(){}

    void initialize(int memSize) {
      // Create an array for all the nodes
      Node memory[memSize];
      Node *startAddr = &memory[0];
      memory[0].tag = 0;
      memory[0].size = memSize;
      memory[0].next = startAddr;
      memory[0].prev = startAddr;
      // Setting up last node
      memory[memSize-1].tag = 0;
      memory[memSize-1].rep = startAddr;
      
      freeList = startAddr;
    }

    Node* allocate(int n, Node *p) {
      p = freeList->next;
      // Pointer null or pointer to size is less than n
      if(p == NULL || (p->size) < n) {
        return 0;
      }
      if((p->size) >= n) {
        int sizeDiff = (p->size) - n;
        p->size = sizeDiff;
        freeList = p;
        p = p + sizeDiff;
        p->tag = 1;
        p->size = n;
        return p;
      }
      // Point to next node
      p = p->next;
      return p;
    }

    void freeBlock(Node *p) {
      int n = p->size;
      // Check if previous or next are empty to see if need to create a free block
      if((p->prev)->tag == 1) {
        p->tag = 0;
        p->prev = freeList;
        p->next = freeList->next;
        freeList->next = p;
      }
      else if((p->prev)->tag == 0) {
        (p->prev)->size += n;
	p = p->prev;
      }
      else if((p->next)->tag == 1) {
        p->size = n + p->size;
        p->tag = 0;
        if(freeList == p + n){
          freeList = p;
        }
      }
      else {
        freeList = p->prev;
      }
    }

    void simulate(int low, int high, int size) {
      for(int i=0; i<100; i++) {
        Block(low, high, size);
      }
    }
};

int main(){
  cout << "This compiles";
  return 0;
} 