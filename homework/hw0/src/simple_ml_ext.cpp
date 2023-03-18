#include <cmath>
#include <iostream>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

void softmax_regression_epoch_cpp(const float *X, const unsigned char *y,
                                  float *theta, size_t m, size_t n, size_t k,
                                  float lr, size_t batch)
{
    /**
     * A C++ version of the softmax regression epoch code.  This should run a
     * single epoch over the data defined by X and y (and sizes m,n,k), and
     * modify theta in place.  Your function will probably want to allocate
     * (and then delete) some helper arrays to store the logits and gradients.
     *
     * Args:
     *     X (const float *): pointer to X data, of size m*n, stored in row
     *          major (C) format
     *     y (const unsigned char *): pointer to y data, of size m
     *     theta (float *): pointer to theta data, of size n*k, stored in row
     *          major (C) format
     *     m (size_t): number of examples
     *     n (size_t): input dimension
     *     k (size_t): number of classes
     *     lr (float): learning rate / SGD step size
     *     batch (int): SGD minibatch size
     *
     * Returns:
     *     (None)
     */

    /// BEGIN YOUR CODE
    for (size_t i = 0; i < m; i += batch) {
        const float         *X_batch = X + i * n;
        const unsigned char *y_batch = y + i;

        float Z[batch][k];
        memset(Z, 0, batch * k * sizeof(float));
        // Z = X_batch * theta
        for (size_t j = 0; j < batch; j++) {
            for (size_t l = 0; l < k; l++) {
                for (size_t o = 0; o < n; o++) {
                    Z[j][l] += X_batch[j * n + o] * theta[o * k + l];
                }
            }
        }
        // Z = exp(Z)
        for (size_t j = 0; j < batch; j++) {
            for (size_t l = 0; l < k; l++) {
                Z[j][l] = exp(Z[j][l]);
            }
        }
        // Normalize Z
        for (size_t j = 0; j < batch; j++) {
            float sum = 0;
            for (size_t l = 0; l < k; l++) {
                sum += Z[j][l];
            }
            for (size_t l = 0; l < k; l++) {
                Z[j][l] /= sum;
            }
        }
        // Minus 1 for y
        for (size_t j = 0; j < batch; j++) {
            Z[j][y_batch[j]] -= 1;
        }
        // theta -= lr * X_batch^T * Z / batch
        for (size_t j = 0; j < n; j++) {
            for (size_t l = 0; l < batch; l++) {
                for (size_t o = 0; o < k; o++) {
                    theta[j * k + o] -= lr * X_batch[l * n + j] * Z[l][o]
                                      / static_cast<float>(batch);
                }
            }
        }
    }
    /// END YOUR CODE
}

/**
 * This is the pybind11 code that wraps the function above.  It's only role is
 * wrap the function above in a Python module, and you do not need to make any
 * edits to the code
 */
PYBIND11_MODULE(simple_ml_ext, m)
{
    m.def(
        "softmax_regression_epoch_cpp",
        [](py::array_t<float, py::array::c_style>         X,
           py::array_t<unsigned char, py::array::c_style> y,
           py::array_t<float, py::array::c_style> theta, float lr, int batch) {
        softmax_regression_epoch_cpp(
            static_cast<const float *>(X.request().ptr),
            static_cast<const unsigned char *>(y.request().ptr),
            static_cast<float *>(theta.request().ptr), X.request().shape[0],
            X.request().shape[1], theta.request().shape[1], lr, batch);
        },
        py::arg("X"), py::arg("y"), py::arg("theta"), py::arg("lr"),
        py::arg("batch"));
}
