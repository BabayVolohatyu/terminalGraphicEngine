#include <math/matrix.h>

using namespace math;


template<typename T>
void Matrix<T>::approximate(std::vector<std::vector<T>> &matrix) {
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            if (std::abs(data_[i][j]) <= kMinLimit) { data_[i][j] = 0; }
        }
    }
}

template<typename T>
Matrix<T>::Matrix(size_t rows, size_t cols, T initialValue) {
    data_.resize(rows, std::vector<T>(cols, initialValue));
}

template<typename T>
Matrix<T>::Matrix(Matrix &&other) noexcept
: data_(std::move(other.data_)){}

template<typename T>
Matrix<T>::Matrix(const Matrix &other)
: data_(other.data_) {}

template<typename T>
Matrix<T> Matrix<T>::transpose() const {
    Matrix result(data_[0].size(), data_.size());
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            result[j][i] = data_[i][j];
        }
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::inverse() {
    size_t n = data_.size();
    if (n != data_[0].size()) {
        throw std::invalid_argument("Matrix must be square to compute inverse.");
    }

    // Create an augmented data_ [A | I]
    Matrix augmented(n, 2 * n);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            augmented[i][j] = data_[i][j];
            augmented[i][j + n] = (i == j) ? 1 : 0; // Identity data_
        }
    }

    // Perform Gaussian elimination
    for (size_t i = 0; i < n; i++) {
        // Find the pivot row
        size_t pivot = i;
        for (size_t j = i + 1; j < n; j++) {
            if (std::abs(augmented[j][i]) > std::abs(augmented[pivot][i])) {
                pivot = j;
            }
        }

        // Check for singular data_
        if (augmented[pivot][i] == 0) {
            throw std::invalid_argument("Matrix is singular and cannot be inverted.");
        }

        // Normalize the pivot row
        T pivotValue = augmented[i][i];
        for (size_t j = 0; j < 2 * n; j++) {
            augmented[i][j] /= pivotValue;
        }

        // Eliminate other rows
        for (size_t j = 0; j < n; j++) {
            if (j != i) {
                T factor = augmented[j][i];
                for (size_t k = 0; k < 2 * n; k++) {
                    augmented[j][k] -= factor * augmented[i][k];
                }
            }
        }
    }

    // Extract the inverse from the augmented data_
    Matrix inverse(n, n);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            inverse[i][j] = augmented[i][j + n];
        }
    }
    approximate(augmented.data_);
    return inverse;
}

template<typename T>
void Matrix<T>::resize(size_t rows, size_t cols, T initialValue) {
    data_.resize(rows, std::vector<T>(cols, initialValue));
}

template<typename T>
void Matrix<T>::clear() {
    data_.clear();
}

template<typename T>
std::vector<T> & Matrix<T>::operator[](size_t row) {
    return data_[row];
}

template<typename T>
Matrix<T> & Matrix<T>::operator=(Matrix &&other) noexcept {
    if (this != &other) {
        data_ = std::move(other.data_);
    }
    return *this;
}

template<typename T>
Matrix<T> & Matrix<T>::operator=(const Matrix &other) {
    if (this != &other) {
        data_ = other.data_;
    }
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix &other) {
    size_t numberOfRows = data_.size(); // number of rows of the first data_
    size_t numberOfColumns = other.size().second; // number of columns of the second data_
    size_t n = data_[0].size(); //columns of first data_
    //check if multiplication is available
    if (n != other.size().first) {
        throw std::invalid_argument("Matrix dimensions don`t match");
    }

    Matrix result(numberOfRows, numberOfColumns); //result data_

    for (size_t i = 0; i < numberOfRows; i++) {
        for (size_t j = 0; j < numberOfColumns; j++) {
            for (size_t k = 0; k < n; k++) {
                result.data_[i][j] += data_[i][k] * other.data_[k][j];
            }
        }
    }

    approximate(result.data_);
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const T &scalar) {
    Matrix result(data_.size(), data_[0].size());
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            result.data_[i][j] = data_[i][j] * scalar;
        }
    }

    approximate(result.data_);
    return result;
}

template<typename T>
Matrix<T> & Matrix<T>::operator*=(const Matrix &other) {
    // Check if multiplication is available
    if (data_[0].size() != other.size().first) {
        throw std::invalid_argument("Matrix dimensions don't match");
    }

    // Create a temporary data_ to store the result
    Matrix result(data_.size(), other.size().second);

    // Perform data_ multiplication
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < other.size().second; j++) {
            result.data_[i][j] = 0; // Initialize the result element to 0
            for (size_t k = 0; k < data_[0].size(); k++) {
                result.data_[i][j] += data_[i][k] * other.data_[k][j];
            }
        }
    }

    approximate(result.data_);
    // Assign the result back to the current data_
    data_ = std::move(result.data_);

    return *this;
}

template<typename T>
Matrix<T> & Matrix<T>::operator*=(const T &scalar) {
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            data_[i][j] *= scalar;
        }
    }

    approximate(data_);
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const T &scalar) {
    if (scalar == 0.0) {
        throw std::invalid_argument("Cannot divide by zero");
    }
    Matrix result(data_.size(), data_[0].size());
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            result.data_[i][j] = data_[i][j] / scalar;
        }
    }

    approximate(result.data_);
    return result;
}

template<typename T>
Matrix<T> & Matrix<T>::operator/=(const T &scalar) {
    if (scalar == 0.0) {
        throw std::invalid_argument("Cannot divide by zero");
    }
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            data_[i][j] /= scalar;
        }
    }

    approximate(data_);
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix &other) {
    // Check if dimensions match
    if (data_.size() != other.size().first || data_[0].size() != other.size().second) {
        throw std::invalid_argument("Matrix dimensions don't match");
    }

    Matrix result(data_.size(), data_[0].size()); // Result data_

    // Perform element-wise addition
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            result.data_[i][j] = data_[i][j] + other.data_[i][j];
        }
    }

    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const T &scalar) {
    Matrix result(data_.size(), data_[0].size());
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            result.data_[i][j] = data_[i][j] + scalar;
        }
    }
    return result;
}

template<typename T>
Matrix<T> & Matrix<T>::operator+=(const Matrix &other) {
    // Check if dimensions match
    if (data_.size() != other.size().first || data_[0].size() != other.size().second) {
        throw std::invalid_argument("Matrix dimensions don't match");
    }

    // Perform element-wise addition and assign to the current data_
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            data_[i][j] += other.data_[i][j];
        }
    }

    return *this;
}

template<typename T>
Matrix<T> & Matrix<T>::operator+=(const T &scalar) {
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            data_[i][j] += scalar;
        }
    }
    return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix &other) {
    // Check if dimensions match
    if (data_.size() != other.size().first || data_[0].size() != other.size().second) {
        throw std::invalid_argument("Matrix dimensions don't match");
    }

    Matrix result(data_.size(), data_[0].size()); // Result data_

    // Perform element-wise subtraction
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            result.data_[i][j] = data_[i][j] - other.data_[i][j];
        }
    }

    return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const T &scalar) {
    Matrix result(data_.size(), data_[0].size());
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            result.data_[i][j] = data_[i][j] - scalar;
        }
    }
    return result;
}

template<typename T>
Matrix<T> & Matrix<T>::operator-=(const Matrix &other) {
    // Check if dimensions match
    if (data_.size() != other.size().first || data_[0].size() != other.size().second) {
        throw std::invalid_argument("Matrix dimensions don't match");
    }

    // Perform element-wise subtraction and assign to the current data_
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            data_[i][j] -= other.data_[i][j];
        }
    }

    return *this;
}

template<typename T>
Matrix<T> & Matrix<T>::operator-=(const T &scalar) {
    for (size_t i = 0; i < data_.size(); i++) {
        for (size_t j = 0; j < data_[0].size(); j++) {
            data_[i][j] -= scalar;
        }
    }
    return *this;
}

template<typename T>
bool Matrix<T>::operator==(const Matrix &other) const {
    return data_ == other.data_;
}

template<typename T>
bool Matrix<T>::operator!=(const Matrix &other) const {
    return !(*this == other);
}

template<typename T>
std::pair<size_t, size_t> Matrix<T>::size() const {
    return {data_.size(), data_[0].size()};
}

template<typename T>
T Matrix<T>::get(size_t row, size_t col) const {
    if (row >= data_.size() || col >= data_[0].size()) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data_[row][col];
}

template<typename T>
void Matrix<T>::set(size_t row, size_t col, T value) {
    if (row >= data_.size() || col >= data_[0].size()) {
        throw std::out_of_range("Matrix indices out of range");
    }
    data_[row][col] = value;
}




