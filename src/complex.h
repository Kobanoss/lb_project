#ifndef COMPLEX_H
#define COMPLEX_H
#include <ostream>

class complex {
private:
    double real;
    double imag;

public:
    complex(double real, double imag): real(real), imag(imag)  {}

    double getReal() {
        return this->real;
    }

    double getImag(){
        return this->imag;
    }

    complex operator+(complex second) const {
        return complex{this->real + second.real, this->imag + second.imag};
    }

    complex operator-(complex second) const {
        return complex{this->real - second.real, this->imag - second.imag};
    }

    complex operator*(complex second) const {
        return complex{(this->real * second.real) - (this->imag * second.imag),
                             (this->real * second.imag) + (second.real * this->imag)};
    }

    complex operator/(complex second) const {
        return complex{((this->real * second.real) - (this->imag * second.imag))
                             / (second.real * second.real + second.imag * second.imag),
                             ((this->imag * second.real) + (this->real * second.imag))
                             / (second.real * second.real + second.imag * second.imag)};
    }

    friend std::ostream &operator<<(std::ostream &stream, const complex object) {
        stream << "Complex(" << object.real << ',' << object.imag << ')' << std::endl;

        return stream;
    }
};
#endif // COMPLEX_H
