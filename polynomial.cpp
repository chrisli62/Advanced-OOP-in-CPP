//Christopher Li
//Make sure the input file is named input.txt
//and output file is named output.txt

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

ostringstream toOutput;

class Nodes{

//declare variables
private:
  Nodes *next;
  int coefficient, exponent;

//set and get methods and default constructor 
public:
  Nodes(){}; //default constructor
  void setNext(Nodes* nxtPtr){
    next = nxtPtr;
  }
  void setCoeff(int coeff){
    coefficient = coeff;
  }
  void setExp(int exp){
    exponent = exp;
  }
  int getCoeff(){
    return coefficient;
  }
  int getExp(){
    return exponent;
  }
  Nodes* getNext(){
    return next;
  }
};

class PolynomialList{

private:
  Nodes* head;

public:
  PolynomialList(){
    Nodes *newNode = new Nodes();
    newNode->setCoeff(0);
    newNode->setExp(0);
    newNode->setNext(NULL);
    head = newNode;
  };

  //destructor class
  ~PolynomialList(){};

  void append(int coeff, int exp){
    if(coeff == 0){
      exp = 0;
    }
    Nodes *newNode = new Nodes();
    newNode->setCoeff(coeff);
    newNode->setExp(exp);
    newNode->setNext(NULL);

    Nodes* p = head->getNext();
    while(p != NULL){
      //if exponents are equal add their coefficients and return
      if(p->getExp() == exp){
        p->setCoeff(p->getCoeff() + coeff);
        return;
      }
      p = p->getNext();
    }
    p = head;
    while(p->getNext() != NULL){
      if(newNode->getExp() > p->getNext()->getExp()){
        newNode->setNext(p->getNext());
        p->setNext(newNode);
        return;
      }
      p = p->getNext();
    }
    p->setNext(newNode);
  }

  string print(){
    string poly = "";
    Nodes* p = head->getNext();
    while(p != NULL){
      if(p->getCoeff() == 0){
        poly += "";
      }
      else if(p->getCoeff() == 1){
        poly += "(x^" + convertInt(p->getExp()) + ")";
      }
      else if(p->getExp() == 1){
        poly += "(" + convertInt(p->getCoeff()) + "x)";
      }
      else{
        poly += "(" + convertInt(p->getCoeff()) + "x^" + convertInt(p->getExp()) + ")";
      }
      if(p->getNext() != NULL && p->getCoeff() != 0){
        poly += " + ";
      }
      p = p->getNext();
    }
    return poly;
  }

  string convertInt(int number){
    stringstream ss;
    ss << number;
    return ss.str();
  }
};

void Processor(string str1, string str2){
  PolynomialList list1, list2, Add, Sub, Mult;
  int coeff1, exp1, coeff2, exp2;

  stringstream str(str1);
  while(str >> coeff1 >> exp1){
    stringstream nxtstr(str2);
    while(nxtstr >> coeff2 >> exp2){
      Mult.append(coeff1*coeff2, exp1+exp2);
    }
    list1.append(coeff1, exp1);
    Add.append(coeff1, exp1);
    Sub.append(coeff1, exp1);
  }

  stringstream nxtstr(str2);
  while(nxtstr >> coeff1 >> exp1){
    list2.append(coeff1, exp1);
    Add.append(coeff1, exp1);
    Sub.append(-coeff1, exp1);
  }

  toOutput << "The inputs:\n" << list1.print() << "\n" << list2.print() << endl;
  toOutput << "The outputs:\n" << Add.print() << "\n" << Sub.print() << "\n" << Mult.print() << endl;
}


int main() {
  ifstream readFile;
  //using sample file on blackboard
  string line1, line2;
  //truncate the output.txt file to clear all text
  ofstream outputClear("output.txt");
  outputClear.close();

  printf("-Computing-\n");
  readFile.open("input.txt");
  ofstream outputFile("output.txt");
  if(readFile.is_open()){
    //as long as there are lines to be read this will
    //continue the while loop
    while(readFile.good()){
      getline(readFile, line1);
      getline(readFile, line2);
      Processor(line1, line2);
    }
  }
  outputFile << toOutput.str();
  readFile.close();
  outputFile.close();
  printf("-Complete-");
  return 0;
}