#include <iostream>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include <sstream>
using namespace std;

// VNT Code starts at line 235

template <class T> class SA;
template <class T> ostream& operator<<(ostream& os, SA<T> s);
template <class T> class SafeMatrix;
template <class T> ostream& operator<<(ostream& os, SafeMatrix<T> s);

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

// 2D Safe Array code 

template <class T> class SafeMatrix{
  private:
  int lower_row, higher_row, lower_col, higher_col;
  SA< SA<T> > matrix;

public:

  SafeMatrix(){}

  //1 parameter to determine the size of the matrix
  SafeMatrix(int size){
    lower_row = 0;
    higher_row = size-1;
    lower_col = 0;
    higher_col = size-1;
    SA< SA<T> >row(size);
    for(int i=0; i<size; i++){
      row[i] = SA<T>(size);
    }
    matrix = row;
  }

  //2 parameters where r represents rows
  //c represents the columns of matrix
  //similar to 1 parameter code
  SafeMatrix(int r, int c){
    if(r<=0 || c<=0){
      printf("Error in bounds");
      exit(1);
    }
    lower_row = 0;
    higher_row = r-1;
    lower_col = 0;
    higher_col = c-1;
    SA< SA<T> >row(r);
    for(int i=0; i<r; i++){
      row[i] = SA<T>(c);
    }
  }

  //4 parameters to control all 4 row/column parameters
  SafeMatrix(int l_r, int h_r, int l_c, int h_c){
    if ((h_r - l_r + 1) <= 0){
      printf("Error in bounds definition\n");
      exit(1);
    }
    lower_row = l_r;
    higher_row = h_r;
    lower_col = l_c;
    higher_col = h_c;
    SA< SA<T> >row(l_r, h_r);
    for(int i=l_r; i<=h_r; i++){
      row[i] = SA<T>(l_c, h_c);
    }
    matrix = row;
  }

  ~SafeMatrix(){}

  SA<T> &operator[](int i){
    if(i<lower_row || i>higher_row){
      cout<< i << " is out of range" << endl;
      exit(1);
    }
    return matrix[i];
  }

  //matrix multiplication
  //columns of 1st matrix must equal rows of 2nd matrix
  //eg 4x5 * 5x4
  SafeMatrix operator*(SafeMatrix & s){
    int col_size = higher_col - lower_col + 1;
    int row_size = s.higher_row - s.lower_row + 1;
    if(col_size != row_size){
      printf("Error: Columns is not equal with S's rows!\n");
      exit(1);
    }
    SafeMatrix<T> product(lower_row, higher_row, s.lower_col, s.higher_col);
    for(int i=product.lower_row; i<=product.higher_row; i++){
      for(int j=product.lower_col; j<=product.higher_col; j++){
        for(int k=0; k<=higher_col-lower_col; k++){
          if(k==0){
            product.matrix[i][j] = matrix[i][k+lower_col] * s.matrix[k+lower_row][j];
          }
          else{
            product.matrix[i][j] += matrix[i][k+lower_col] * s.matrix[k+lower_row][j];
          }
        }
      }
    }
    return product;
  }

  int getRows(){
    return higher_row - lower_row + 1;
  }

  int getCols(){
    return higher_col - lower_col + 1;
  }

  SA<T>* operator+(int i){
    return &matrix[i];
  }
  
  friend ostream& operator<< <T>(ostream& os, SafeMatrix<T> s);
};

template <class T> ostream& operator<<(ostream& os, SafeMatrix<T> s){
  for(int i=s.lower_row; i<=s.higher_row; i++){
    for(int j=s.lower_col; i<=s.higher_col;j++){
      os<s.matrix[i][j] << " ";
    }
    os<<endl;
  }
  return os;
}

void printMatrix(int **matrix, int row, int col){
  for(int i=0; i<row; i++){
    for(int j=0; j<col; j++){
      cout<<matrix[i][j]<<" ";
    }
    cout<<"\n";
  }
}


// VNT Class 

class VNT{
  public:
    SafeMatrix<int> vnt;
    int x, y, temp_x, temp_y;

    VNT(int a, int b){
      vnt = SafeMatrix<int>(a, b);
      x = a-1;
      y = b-1;
      temp_x = 0;
      temp_y = 0;
      for(int i=0; i<a; i++){
        for(int j=0; j<b; j++){
          vnt[i][j] = 0;
        }
      }
    }

    friend ostream & operator<<(ostream & os, VNT &a){
      for(int i=a.x; i!=-1; i--){
        for(int j=a.y; j!=-1; j--){
          if(a.vnt[i][j] != 0){
            cout<<a.vnt[i][j]<<" ";
          }
        }
        cout<<endl;
      }
      return os;
    }

    void add(int a){
      if(temp_y < y){
        temp_x++;
      }
      if(temp_x < x){
        printf("ERROR: This is full\n");
        return;
      }
      else{
        temp_y = 0;
        vnt[temp_x][temp_y] = a;

        for(int i=0; i<=x; i++){
          for(int j=0; j<=x; j++){
            if(vnt[i][temp_y] > vnt[j][temp_y]){
              swap(vnt[i][temp_y], vnt[j][temp_y]);
            }
          }
        }
        for(int i=0; i<=y; i++){
          for(int j=0; j<=y; j++){
            if(vnt[temp_x][i] > vnt[temp_y][j]){
              swap(vnt[temp_x][i], vnt[temp_x][j]);
            }
          }
        }
        temp_y++;
        return;
      }
    }

    int getMin(){
      for(int i=x; i!=-1; i--){
        for(int j=y; j!=-1; j--){
          if(vnt[i][j]!=0){
            return vnt[i][j];
          }
        }
      }
    }
    
    static void sort(int arr[], int size){
      for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
          if(arr[i]<arr[j]){
            swap(arr[i], arr[j]);
          }
        }
      }
    }

    bool find(int val){
      for(int i=0; i<=x; i++){
        for(int j=0; j<=y; j++){
          if(vnt[i][j]==val){
            return true;
          }
          else{
            return false;
          }
        }
      }
    }

};

int main(){
  string line;
  int x, y, size;
  char str[100];

  printf("Please input file name: ");
  cin >> str;
  ifstream infile(str);
  if(infile.is_open()){
    while(getline(infile, line)){
      infile >> x;
      infile >> y;
      infile >> size;
      int *array = new int[size];
      for(int i=0; i<size; i++){
        infile >> array[i];
      }
      printf("Original array: \n");
      for(int i=0; i<size; i++){
        cout<<array[i]<<" ";
      }
      cout<<endl;
      printf("Very Neat Table: \n");
      VNT::sort(array,size);
      for(int i=0; i<size; i++){
        if(i<0){
          printf("ERROR: Array size can't be negative\n");
          exit(1);
        }
        cout<<array[i]<<" ";
      }
      cout<<endl;
    }
    infile.close();
  }
  else{
    printf("ERROR: File does not exist\n");
    exit(1);
  }
  return 0;
} 