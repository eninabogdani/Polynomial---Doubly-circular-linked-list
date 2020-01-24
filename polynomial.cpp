// Implementation file for the Polynomial class.

#include "polynomial.h"

// Default constructor
// Constructs an empty polynomial with a dummy header of coefficient 0 and power 0
Polynomial::Polynomial() {
    initialize();
}

// Constructor
// Takes a Polynomial p as parameter, deep copies it into current Polynomial
Polynomial::Polynomial(const Polynomial& p) {
    initialize();  
    *this = p; // Use overloaded assignment operator to deep copy
}

// Destructor
// Deletes all terms and head of the current object
Polynomial::~Polynomial() {
    clear();
    delete head;
}

// Overloaded operator<<
// Prints the current object in the format Cn * x^n + Cn-1 * X^n-1 + ... C1 * X + C0
// If the coefficient is 1, it is omitted
// If the current object is empty, prints Null
ostream& operator<<(ostream &output, const Polynomial& p) {
    if (p.size == 0) {
        cout << "Null";
        return output;
    }

    bool isFirst = true;
    Polynomial::Term* current = p.head->next;
    while (current != p.head) {
        double coefficient = current->coeff;
        int degree = current->power;

        // Print operator
        // First term is not preceded by a + or - sign, all subsequent terms are
        if (coefficient >= 0 && !isFirst) {
            cout << " + ";
        }

        // Print sign of coefficient
        if (coefficient < 0 && isFirst) {
            cout << "-";
        } else if (coefficient < 0) {
            cout << " - ";
        }
        
        // Print coefficient
        if (coefficient != 1 && coefficient != -1) {
            // Get coefficient's absolute value in order to avoid duplicate - signs
            cout << (coefficient < 0 ? -coefficient : coefficient);
        } else if ((coefficient == 1 || coefficient == -1) && degree == 0) {
            cout << "1";
        }

        // Print variable
        if (degree > 0) {
            cout << "x";
        }

        // Print power
        if (degree > 1) {
                cout << "^" << degree;
        }

        isFirst = false;
        current = current->next;
    }

    return output;
}

// Returns highest degree of the current object
// Returns -1 if the current object is empty
int Polynomial::degree() const {
    Term* current = head->next;
    int maxDegree = -1;
    while (current != head) {
        if (current->power > maxDegree) {
            maxDegree = current->power;
        }

        current = current->next;
    }

    return maxDegree;
}

// Takes a power as a parameter
// Power cannot be a negative value
// Returns the coefficient of the term with the given power
// Returns 0.0 when the power is not found
double Polynomial::coefficient(const int power) const {
    Term* current = head->next;
    while (current != head) {
        if (current->power == power) {
            return current->coeff;
        }

        current = current->next;
    }

    return 0.0;
 }

// Takes a coefficient and a power as parameters
// Replaces the coefficient of the term with the given power with the new coefficient
// The given power cannot be a negative value
// If there is no term with the given power, insert a new term with the given coefficient and power
// If the given coefficient is 0, removes any term with the given power
bool Polynomial::changeCoefficient(const double newCoefficient, const int power) {
    if (power < 0) {
        cout << "Please provide a positive power" << endl;
        return false;
    }

    Term* current = head->next;
    Term* posToInsert = head;
    while (current != head) {
        if (current->power == power) {
            if (newCoefficient != 0.0) {
                // If powers are equal and coefficient is not 0, update coefficient
                current->coeff = newCoefficient;
                return true;
            } else {
                // If coefficient is 0, delete term
                return remove(current);
            }
        }

        // Else if the power doesn't exist in the object, insert new term
        // Insert new term and maintain Polynomial order of degrees
        if (current->power < power && posToInsert == head) {
            posToInsert = current;
        }

        current = current->next;
    }

    return insert(posToInsert, newCoefficient, power); 
}

// Overloaded operator+
// Takes a Polynomial p as parameter
// Adds p to the current object, and returns a new Polynomial object with this value
Polynomial Polynomial::operator+(const Polynomial& p) const { 
    Polynomial sum;
    Term* pCurrent = p.head->next;
    while (pCurrent != p.head) {
        int pPower = pCurrent->power;
        double pCoeff = pCurrent->coeff;
        double myCoeff = coefficient(pPower); // This is zero if not found, which is fine
        double coeffSum = myCoeff + pCoeff;
        sum.insert(sum.head, coeffSum, pPower); // Insert handles zero coefficients
        pCurrent = pCurrent->next;
    }

    // Loop over current object, if term found with power not present in p, insert it into sum
    Term* current = head->next;
    while (current != head) {
        int currPower = current->power;
        double currCoeff = current->coeff;
        double coeffInP = p.coefficient(currPower);
        if (coeffInP == 0.0) {
            sum.changeCoefficient(currCoeff, currPower);
        }
        current = current->next;
    }

    return sum;
}

// Overloaded operator-
// Takes a Polynomial p as parameter
// Subtracts p from the current object, and returns a new Polynomial object with this value
Polynomial Polynomial::operator-(const Polynomial& p) const {
    Polynomial difference;
    Term* pCurrent = p.head->next;
    while (pCurrent != p.head) {
        int pPower = pCurrent->power;
        double pCoeff = pCurrent->coeff;
        double myCoeff = coefficient(pPower); // This is zero if not found, which is fine
        double coeffDiff = myCoeff - pCoeff;
        difference.insert(difference.head, coeffDiff, pPower); // Insert handles zero coefficients
        pCurrent = pCurrent->next;
    }
    
    // Loop over current object, if term found with power not present in p, insert it into difference
    Term* current = head->next;
    while (current != head) {
        int currPower = current->power;
        double currCoeff = current->coeff;
        double coeffInP = p.coefficient(currPower);
        if (coeffInP == 0.0) {
            difference.changeCoefficient(currCoeff, currPower);
        }
        current = current->next;
    }

    return difference;
}

// Overloaded operator==
// Takes a Polynomial p as parameter, compares it to the current object
// Returns true if the they are equal, false otherwise
bool Polynomial::operator==(const Polynomial& p) const {
    if (size != p.size) {
        return false;
    }
    
    Term* pCurrent = p.head->next;
    while (pCurrent != p.head) {
        int pPower = pCurrent->power;
        double pCoeff = pCurrent->coeff;
        double myCoeff = coefficient(pPower); // Handles missing power - coefficient = 0.0
        if (pCoeff != myCoeff) {
            return false;
        }

        pCurrent = pCurrent->next;
    }

    return true;
}

// Overloaded operator!=
// Takes a Polynomial p as parameter, compares it to the current object
// Returns true if the they are equal, false otherwise
bool Polynomial::operator!=(const Polynomial& p) const {
    return !(*this == p);
}

// Overloaded operator=
// Takes a Polynomial p as parameter,
// If the given object is equal to the current object, immediately returns current object
// Otherwise, sets the current object equal to it, and returns the current object
Polynomial& Polynomial::operator=(const Polynomial& p) {
    if (&p == this) {
        return *this;
    }

    clear();
    int index;
    Term* node;
    for (index = 0, node = p.head->next; node != p.head; node = node->next, index++) {
        // We insert before the head, so at the end, of the list each time
        insert(head, node->coeff, node->power);
    }

    return *this;
}

// Overloaded operator+=
// Takes a Polynomial p as parameter, adds it to the current object
// Returns the current object
Polynomial& Polynomial::operator+=(const Polynomial& p) {
    Polynomial sum = *this + p;
    *this = sum;
    return *this;
}

// Overloaded operator-=
// Takes a Polynomial p as parameter, adds it to the current object
// Returns the current object
Polynomial& Polynomial::operator-=(const Polynomial& p) {
    Polynomial diff = *this - p;
    *this = diff;
    return *this;
}

// Takes a pointer to a position, a coefficient and a power as parameters
// Precondition: pos is a valid position in our list
// Inserts a new term with the given coefficient and power before pos
// Returns true if insertion was successful, false otherwise
bool Polynomial::insert(Term* pos, const double newCoefficient, const int power) {
    if (pos == NULL || power < 0 || newCoefficient == 0.0) {
        return false;
    }
    
    Term* newNode = new Term;
    newNode->coeff = newCoefficient;
    newNode->power = power;
    Term* prevNode = pos->prev;
    newNode->next = pos;
    newNode->prev = prevNode;
    pos->prev = newNode;
    prevNode->next = newNode;
    size++;
    return true;
}

// Takes a pointer to a position parameter
// Preconditioon: pos is a valid position in our list
// Deletes the term at the given position
// Returns true if deletion was successful, false otherwise
bool Polynomial::remove(Term* pos) {
    if (pos == NULL || pos == head) {
        return false;
    }
    
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    delete pos;
    pos = NULL;
    size--;
    return true;
}

// Clears all terms of the current object if its size is greater than 0
void Polynomial::clear() {
    while (size > 0) {
        remove(head->next);
    }
}

// Initializes an empty Polynomial with a dummy header
void Polynomial::initialize() {
    size = 0;
    head = new Term;
    head->coeff = 0;
    head->power = 0;
    head->prev = head;
    head->next = head;
}
