// Created by Anderson on 11/15/2023.

/**
 * @file Util.h
 * @brief Declaration of the Util class template for N-dimensional vectors and related operations.
 */

#ifndef CASTR_VECTOR_H
#define CASTR_VECTOR_H

#include "../imports.h"

/**
 * @brief Template class for N-dimensional vectors and related operations.
 * @tparam T The data type of the vector elements.
 * @tparam N The size or dimensionality of the vector.
 */
template <typename T, std::size_t N>
struct Vector {
    // Constructors

    Vector();
    Vector(std::initializer_list<T> initList);
    Vector(Vector &v);
    explicit Vector(T initList[N]);

    // Scalar Operations
    inline Vector operator+(T s);
    inline Vector operator-(T s);
    inline Vector operator*(T s);
    inline Vector operator/(T s);

    // Element-wise Operations
    inline Vector operator+(Vector v);
    inline Vector operator-(Vector v);
    inline Vector operator*(Vector v);
    inline Vector operator/(Vector v);

    // Element-wise Assignment Operations
    inline Vector operator+=(Vector v);
    inline Vector operator-=(Vector v);
    inline Vector operator*=(Vector v);
    inline Vector operator/=(Vector v);

    // Array Operations
    inline T& operator[](std::size_t index);
    inline T sum();

    // Util Operations
    inline T magnitude();

    /**
     * @brief Converts the vector to a string for easy printing.
     * @return The string representation of the vector.
     */
    std::string toString();

private:
    // Util elements
    T elements[N];

    // Size of vector
    std::size_t size = N;
};

template<typename T, std::size_t N>
std::string Vector<T, N>::toString() {
    std::string str;
    for (T elem: elements) {
        str += std::to_string(elem) + ' ';
    }

    str.pop_back();

    return '[' + str + ']';
}

//////////////////////////
// Scalar Operations
//////////////////////////

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator+(T s) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] + s;
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator-(T s) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] - s;
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator*(T s) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] * s;
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator/(T s) {
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
Vector<T, N> Vector<T, N>::operator+(Vector<T, N> v) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] + v[i];
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator-(Vector v) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] - v[i];
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator*(Vector v) {
    T newElements[N];

    for (size_t i = 0; i < N; i++) {
        newElements[i] = (*this)[i] * v[i];
    }

    return Vector<T, N>(newElements);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator/(Vector v) {
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
Vector<T, N> Vector<T, N>::operator+=(Vector v) {
    for (size_t i = 0; i < N; i++) {
        this->elements[i] = (*this)[i] + v[i];
    }

    return (*this);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator-=(Vector v) {
    for (size_t i = 0; i < N; i++) {
        this->elements[i] = (*this)[i] - v[i];
    }

    return (*this);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator*=(Vector v) {
    for (size_t i = 0; i < N; i++) {
        this->elements[i] = (*this)[i] * v[i];
    }

    return (*this);
}

template<typename T, std::size_t N>
Vector<T, N> Vector<T, N>::operator/=(Vector v) {
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
// Util Operations
////////////////////

template<typename T, std::size_t N>
T Vector<T, N>::magnitude() {
    // Get sum of square of each element
    T squareSum = ((*this) * (*this)).sum();

    return sqrt(squareSum);
}


///////////////
// Constructors
///////////////

template<typename T, std::size_t N>
Vector<T, N>::Vector() {
    for (size_t i = 0; i < N; i++) {
        this->elements[i] = 0;
    }
}

template<typename T, std::size_t N>
Vector<T, N>::Vector(std::initializer_list<T> initList) {
    size_t i = 0;
    for (auto& element : initList) {
        this->elements[i++] = element;
    }
}

template<typename T, std::size_t N>
Vector<T, N>::Vector(Vector &v) {
    for (size_t i = 0; i < N; i++) {
        (*this)[i] = v[i];
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

/**
 * @brief Typedef for a 2D integer vector.
 */
typedef Vector<int, 2> vInt2d;

/**
 * @brief Typedef for a 2D double vector.
 */
typedef Vector<double, 2> vDouble2d;

typedef Vector<int, 3> vInt3d;
typedef Vector<double, 3> vDouble3d;

/**
 * @brief Rotates a 2D double vector by a specified angle in degrees.
 * @param v The 2D vector to be rotated.
 * @param degrees The angle of rotation in degrees.
 * @return The rotated 2D vector.
 */
vDouble3d rotate2D(vDouble3d &v, double degrees);
vDouble3d perp2D(vDouble3d &v);

#endif //CASTR_VECTOR_H
