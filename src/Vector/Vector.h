//
// Created by Anderson on 11/15/2023.
//

#ifndef CASTR_VECTOR_H
#define CASTR_VECTOR_H

#include "../imports.h"

template <typename T, std::size_t N>
struct Vector {
    // Constructors
    inline Vector() = default;
    inline Vector(std::initializer_list<T> initList);
    inline explicit Vector(T initList[N]);

    // Scalar Operations
    inline Vector operator+(T &s);
    inline Vector operator-(T &s);
    inline Vector operator*(T &s);
    inline Vector operator/(T &s);

    // Element wise Operations
    inline Vector operator+(Vector &v);
    inline Vector operator-(Vector &v);
    inline Vector operator*(Vector &v);
    inline Vector operator/(Vector &v);

    // Element Wise Assignment Operations
    inline Vector operator+=(Vector &v);
    inline Vector operator-=(Vector &v);
    inline Vector operator*=(Vector &v);
    inline Vector operator/=(Vector &v);

    // Array Operations
    inline T& operator[](size_t index);
    inline T sum();

    // Vector Operations
    inline T magnitude();
    inline Vector normalize();
    inline Vector orthogonal();
    inline T dotProduct(Vector &v);

private:
    // Vector elements
    T elements[N];

    // Size of vector
    std::size_t size = N;
};

//////////////////////////
// Scalar Operations
//////////////////////////

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator+(T &s) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] + s;
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator-(T &s) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] - s;
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator*(T &s) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] * s;
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator/(T &s) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] / s;
    }

    return Vector<T, N>(newElements);
}


//////////////////////////
// Element Wise Operations
//////////////////////////

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator+(Vector<T, N> &v) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] + v[i];
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator-(Vector &v) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] - v[i];
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator*(Vector &v) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] * v[i];
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator/(Vector &v) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] / v[i];
    }

    return Vector<T, N>(newElements);
}


////////////////////////
// Assignment Operations
////////////////////////

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator+=(Vector &v) {
    for (size_t i = 0; i < N; i++) {
        this->elements[i] = (*this)[i] + v[i];
    }

    return (*this);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator-=(Vector &v) {
    for (size_t i = 0; i < N; i++) {
        this->elements[i] = (*this)[i] - v[i];
    }

    return (*this);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator*=(Vector &v) {
    for (size_t i = 0; i < N; i++) {
        this->elements[i] = (*this)[i] * v[i];
    }

    return (*this);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator/=(Vector &v) {
    for (size_t i = 0; i < N; i++) {
        this->elements[i] = (*this)[i] / v[i];
    }

    return (*this);
}


///////////////////
// Array Operations
///////////////////

template<typename T, std::size_t N>
T& Vector<T, N>::operator[](size_t index) {
    if (index >= this->size) {
        throw std::invalid_argument("Element index out of range");
    }

    return this->elements[index];
}

template<typename T, std::size_t N>
T Vector<T, N>::sum() {
    T sum = 0;

    for (size_t i = 0; i < this->size; i++) {
        sum += (*this)[i];
    }

    return sum;
}


////////////////////
// Vector Operations
////////////////////

template<typename T, std::size_t N>
T Vector<T, N>::magnitude() {
    // Get sum of square of each element
    T squareSum = ((*this) * (*this)).sum();

    return sqrt(squareSum);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::normalize() {
    T mag = (this->magnitude());

    return Vector<T, N>((*this) / mag);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::orthogonal() {
    return Vector();
}

template<typename T, std::size_t N>
T Vector<T, N>::dotProduct(Vector &v) {
    return ((*this) * v).sum();
}


///////////////
// Constructors
///////////////

template<typename T, std::size_t N>
Vector<T, N>::Vector(std::initializer_list<T> initList) {
    if (initList.size() != N){
        throw std::invalid_argument("Incorrect number of elements provided");
    }

    size_t i = 0;
    for (auto& element : initList) {
        this->elements[i++] = element;
    }
}

template<typename T, std::size_t N>
Vector<T, N>::Vector(T initList[N]) {
    for (size_t i = 0; i < N; i++) {
        this->elements[i] = initList[i];
    }
}


///////////
// Typedefs
///////////

typedef Vector<int, 2> vInt2d;
typedef Vector<double, 2> vDouble2d;
#endif //CASTR_VECTOR_H
