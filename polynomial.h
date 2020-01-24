// Header file for the Polynomial class.
// A polynomial is an expression of more than two algebraic terms, containg different powers of the same variable.
// Terms are ordered in descending order based on the term's power.
// Terms with a 0 coefficient are not included in the polynomial.
// Negative powers are invalid and produce an error message.

#ifndef POLYNOMIAL_H                                   
#define POLYNOMIAL_H
#include <iostream>
#include <string>
using namespace std;

class Polynomial {

  // Overloaded <<: prints Cn * x^n + Cn-1 * X^n-1 + ... C1 * X + C0
  friend ostream& operator<<( ostream &output, const Polynomial& p );

  // Constructor: the default is a 0-degree polynomial with 0.0 coefficient

 public:
  // Member functions
  Polynomial( );  // Default constructor
  Polynomial( const Polynomial& p );  // Constructor that copies given Polynomial p
  ~Polynomial( ); // Desctructor
  int degree( ) const; // Returns the degree of a polynomial
  double coefficient( const int power ) const; 
                 // Returns the coefficient of the x^power term
  bool changeCoefficient( const double newCoefficient, const int power );
                 // Replaces the coefficient of the x^power term

  // Arithmetic operators
  Polynomial operator+( const Polynomial& p ) const;  // Adds two Polynomial objects
  Polynomial operator-( const Polynomial& p ) const;  // Subtracts two Polynomial objects

  // Boolean comparison operators
  bool operator==( const Polynomial& p ) const;       // Is object == parameter?
  bool operator!=( const Polynomial& p ) const;       // Is object != parameter?

  // Assignment operators
  Polynomial& operator=( const Polynomial& p );       // Sets current object equal to parameter
  Polynomial& operator+=( const Polynomial& p );      // Current object += parameter
  Polynomial& operator-=( const Polynomial& p );      // Current object -= parameter
  
 private:
  struct Term {     // A term on the sparse polynomial
    double coeff;   // The coefficient of each term
    int power;      // The degree of each term
    Term *prev;     // A pointer to the previous higher term
    Term *next;     // A pointer to the next lower term
  };
  int size;         // # of terms in the sparse polynomial
  Term *head;       // A pointer to the doubly-linked circular list of
                    // sparse polynomial

  // Inserts a Term with the given coefficient and power in the current Polynomial at the given position
  bool insert( Term* pos, const double newCoefficient, const int power );
  // Removes a Term from the current Polynomial at the given position
  bool remove( Term* pos );                                          
  // Removes all terms in the current Polynomial     
  void clear();
  // Initializes an empty Polynomial
  void initialize();
};

#endif
