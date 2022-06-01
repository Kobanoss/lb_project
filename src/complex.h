#ifndef COMPLEX_H
#define COMPLEX_H
#include <ostream>
#include <QString>

class complex {
private:
    double real;
    double imag;

public:
    complex() {};

    complex(double real, double imag): real(real), imag(imag)  {}

    complex(std::string str) {
        auto data = QString::fromStdString(str);

        QString sign = "+";
        auto pos_first = data.lastIndexOf('+');

        if (pos_first == -1) {
            pos_first = data.lastIndexOf('-');
            sign = "-";
        }

        real =  data.left(pos_first).toDouble();
        imag = data.mid(pos_first + (sign=="+"?1:0), data.length() - pos_first - 1 - (sign=="+"?1:0)).toDouble();
    }

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
        double real_part = ((this->real * second.real) - (this->imag * second.imag))/ (second.real * second.real + second.imag * second.imag);

        double imag_part = ((this->imag * second.real) + (this->real * second.imag))/ (second.real * second.real + second.imag * second.imag);

        return complex{real_part,imag_part};
    }

    friend std::ostream &operator<<(std::ostream &stream, const complex object) {
        stream << "Complex(" << object.real << ',' << object.imag << ')' << std::endl;

        return stream;
    }
};
#endif // COMPLEX_H
