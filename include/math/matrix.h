#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>

namespace math {

template <typename T>
class Matrix {
  static_assert(std::is_arithmetic_v<T>, "Matrix can only be used with arithmetic types.");

  private:
    constexpr double static kMinLimit = 1e-10;

    std::vector<std::vector<T> > data_;

    static void approximate(std::vector<std::vector<T>>& matrix);

  public:
    explicit Matrix(size_t rows = 1, size_t cols = 1, T initialValue = T());

    Matrix(Matrix &&other) noexcept;

    Matrix(const Matrix &other);

    ~Matrix() = default;

    Matrix transpose() const;

    Matrix inverse();

    void resize(size_t rows, size_t cols, T initialValue = T());

    void clear();

    std::vector<T> &operator[](size_t row);

    Matrix &operator=(Matrix &&other) noexcept;

    Matrix &operator=(const Matrix &other);

    Matrix operator*(const Matrix &other);

    Matrix operator*(const T &scalar);

    Matrix &operator*=(const Matrix &other);

    Matrix &operator*=(const T &scalar);

    Matrix operator/(const T &scalar);

    Matrix &operator/=(const T &scalar);

    Matrix operator+(const Matrix &other);

    Matrix operator+(const T &scalar);

    Matrix &operator+=(const Matrix &other);

    Matrix &operator+=(const T &scalar);

    Matrix operator-(const Matrix &other);

    Matrix operator-(const T &scalar);

    Matrix &operator-=(const Matrix &other);

    Matrix &operator-=(const T &scalar);

    bool operator==(const Matrix &other) const;

    bool operator!=(const Matrix &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Matrix &obj);

    [[nodiscard]] std::pair<size_t, size_t> size() const;

    T get(size_t row, size_t col) const;

    void set(size_t row, size_t col, T value);
};
}

#endif
