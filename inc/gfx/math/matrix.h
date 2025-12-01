#pragma once

#include <functional>

namespace gfx
{

template <typename T, int rows, int cols>
class Matrix
{

public:

    Matrix() {}

    Matrix(std::initializer_list<std::initializer_list<T>> array) 
    { 
        int r = 0;
        for (const auto& row : array) 
        {
            int c = 0;
            for (const auto& val : row) 
            {
                data[r][c] = val;
                ++c;
            }
            ++r;
        }
    }

    static Matrix<T, rows, cols> identity()
    {
        Matrix<T, rows, cols> result;
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                result(r, c) = (r == c) ? T(1) : T(0);
            }
        }
        return result;
    }

    T& operator()(int r, int c) 
    { 
        return data[r][c]; 
    }

    const T& operator()(int r, int c) const 
    { 
        return data[r][c]; 
    }

    Matrix<T, rows, cols> operator+(const Matrix<T, rows, cols>& other) const
    {
        Matrix<T, rows, cols> result;
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                result(r, c) = data[r][c] + other(r, c);
            }
        }
        return result;
    }

    Matrix<T, rows, cols> operator-(const Matrix<T, rows, cols>& other) const
    {
        Matrix<T, rows, cols> result;
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                result(r, c) = data[r][c] - other(r, c);
            }
        }
        return result;
    }

    Matrix<T, rows, cols> operator*(T scalar) const
    {
        Matrix<T, rows, cols> result;
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                result(r, c) = data[r][c] * scalar;
            }
        }
        return result;
    }

    Matrix<T, rows, cols> operator/(T scalar) const
    {
        Matrix<T, rows, cols> result;
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                result(r, c) = data[r][c] / scalar;
            }
        }
        return result;
    }

    Matrix<T, cols, rows> transpose() const
    {
        Matrix<T, cols, rows> result;
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                result(c, r) = data[r][c];
            }
        }
        return result;
    }

    Matrix<T, rows, cols> apply(std::function<T(T)> func) const
    {
        Matrix<T, rows, cols> result;
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                result(r, c) = func(data[r][c]);
            }
        }
        return result;
    }

    template <int cols2>
    Matrix<T, rows, cols2> operator*(const Matrix<T, cols, cols2> &other) const
    {
        Matrix<T, rows, cols2> result;
        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols2; ++c)
            {
                result(r, c) = T();
                for (int k = 0; k < cols; ++k)
                {
                    result(r, c) += data[r][k] * other(k, c);
                }
            }
        }
        return result;
    }


private:

    T data[rows][cols];

};

using Matrix3x3d = Matrix<double, 3, 3>;
using Matrix3x1d = Matrix<double, 3, 1>;
}
