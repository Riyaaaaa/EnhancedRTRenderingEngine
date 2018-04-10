#pragma once

#include <cmath>

template <class T, std::size_t dim>
struct _Matrix;

using Matrix = _Matrix<float, 4>;

template <class T, std::size_t dim>
struct _Vector {
    float operator[](unsigned int idx) const {
        return _elems[idx];
    }
    float& operator[](unsigned int idx) {
        return _elems[idx];
    }
private:
    T _elems[dim];
};

template<std::size_t dim>
using Vector = _Vector<float, dim>;

template<class T>
struct _Vector<T, 2> {
    T x, y;

    _Vector()
        : x(0), y(0)
    {}

    _Vector(T xx, T yy)
        : x(xx), y(yy)
    {}

    float distance(const _Vector& v) const {
        return sqrtf(powf(v.x - this->x, 2) + powf(v.y - this->y, 2));
    }

    _Vector operator+(const _Vector& v) const {
        return _Vector(x + v.x, y + v.y);
    }

    _Vector operator-(const _Vector &v) const {
        return _Vector(this->x - v.x, this->y - v.y);
    }

    _Vector operator-() const {
        return _Vector(-this->x, -this->y);
    }

    _Vector operator*(float s) const {
        return _Vector(this->x * s, this->y * s);
    }

    _Vector operator/(float s) const {
        return _Vector(this->x / s, this->y / s);
    }

    bool operator<(const _Vector& v) const {
        return (x == v.x) ? y < v.y : x < v.x;
    }

    bool operator>(const _Vector& v) const {
        return (x == v.x) ? y > v.y : x > v.x;
    }

    bool operator==(const _Vector& v) const {
        return x == v.x && y == v.y;
    }

    bool operator!=(const _Vector& v) const {
        return x != v.x || y != v.y;
    }

    _Vector& operator-=(const _Vector& v) {
        this->x -= v.x;
        this->y -= v.y;
        return *this;
    }

    _Vector& operator+=(const _Vector& v) {
        this->x = this->x + v.x;
        this->y = this->y + v.y;

        return *this;
    }

    _Vector& operator*=(float s) {
        this->x *= s;
        this->y *= s;
        return *this;
    }

    _Vector& operator/=(float s) {
        this->x /= s;
        this->y /= s;
        return *this;
    }
};

using Vector2D = _Vector<float, 2>;
template<class T>
using _Vector2D = _Vector<T, 2>;

template<class T>
struct _Vector<T, 3> {
    T x, y, z;

    _Vector() {}
    constexpr _Vector(T x, T y, T z) : x(x), y(y), z(z) {}

    template<class U>
    _Vector operator-(const _Vector<U, 3> &v) const {
        return _Vector{
            static_cast<T>(this->x - v.x),
            static_cast<T>(this->y - v.y),
            static_cast<T>(this->z - v.z)
        };
    }

    template<class U>
    _Vector operator+(const _Vector<U, 3> &v) const {
        return _Vector{
            static_cast<T>(this->x + v.x), 
            static_cast<T>(this->y + v.y),
            static_cast<T>(this->z + v.z)
        };
    }

    template<class U>
    operator _Vector<U, 3>() {
        return _Vector<U, 3>(
            static_cast<U>(x),
            static_cast<U>(y),
            static_cast<U>(z)
            );
    }

    _Vector operator-() const {
        return _Vector{ -this->x, -this->y, -this->z };
    }

    _Vector operator*(float s) const {
        return _Vector{ this->x * s, this->y * s, this->z * s };
    }

    _Vector& operator+=(const _Vector& v) {
        this->x = this->x + v.x;
        this->y = this->y + v.y;
        this->z = this->z + v.z;
        return *this;
    }

    float Length() const {
        return std::sqrtf(std::powf(x, 2) + std::powf(y, 2) + std::powf(z, 2));
    }

    void Normalize() {
        float l = this->Length();
        this->x /= l;
        this->y /= l;
        this->z /= l;
    }
};

using Vector3D = _Vector<float, 3>;
template<class T>
using _Vector3D = _Vector<T, 3>;

template <class T>
struct _Vector<T, 4> {
    _Vector() {}
    T operator[](unsigned int idx) const {
        const T* ptr = &x;
        return *(ptr + idx);
    }

    T& operator[](unsigned int idx) {
        T* ptr = &x;
        return *(ptr + idx);
    }

    _Vector operator*(const Matrix& mat) {
        _Vector ret(0,0,0,0);

        for (int i = 0; i < 4; i++) {
            ret.x += (*this)[i] * mat[i][0];
        }

        for (int i = 0; i < 4; i++) {
            ret.y += (*this)[i] * mat[i][1];
        }

        for (int i = 0; i < 4; i++) {
            ret.z += (*this)[i] * mat[i][2];
        }

        for (int i = 0; i < 4; i++) {
            ret.w += (*this)[i] * mat[i][3];
        }

        return ret;
    }

    constexpr _Vector(T x, T y, T z, T w = 1) : x(x), y(y), z(z), w(w) {}
    constexpr _Vector(const _Vector<T, 3> vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(1) {}
    T x, y, z, w;

    static constexpr _Vector Zero() { return _Vector(0, 0, 0, 0); }
};

using Vector4D = _Vector<float, 4>;
template<class T>
using _Vector4D = _Vector<T, 4>;

template <class T, std::size_t dim>
struct _Matrix {
    const _Vector<T, dim>& operator[](unsigned int idx) const {
        return _elems[idx];
    }

    _Vector<T, dim>& operator[](unsigned int idx) {
        return _elems[idx];
    }

private:
    _Vector<T, dim> _elems[4];
};
