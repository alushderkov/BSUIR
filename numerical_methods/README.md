# Numerical Methods

This section contains materials on numerical methods, including theoretical background, exercises, and their implementation using Wolfram Mathematica.

## 1. Errors in Numerical Methods (Errors.pdf)

Numerical computations are always accompanied by errors, which can be classified as follows:

- **Absolute error**:  

  $` \Delta x = |x_{\text{num}} - x_{\text{exact}}| `$

- **Relative error**:  

  $` \delta x = \frac{|x_{\text{num}} - x_{\text{exact}}|}{|x_{\text{exact}}|} `$

- **Rounding and accumulated errors**, which occur during iterative calculations.

## 2. Numerical Differentiation and Integration (Differentiation_and_Integration.pdf)

Numerical differentiation allows for the approximate computation of derivatives, for example, using finite differences:

- **Forward difference formula**:  

  $` f'(x) \approx \frac{f(x+h) - f(x)}{h} `$

- **Central difference formula** (more accurate):  

  $` f'(x) \approx \frac{f(x+h) - f(x-h)}{2h} `$

Numerical integration is used to approximate definite integrals:

- **Rectangle method**:  

  $` I \approx \sum_{i=0}^{n-1} f(x_i) \Delta x `$

- **Trapezoidal rule**:  

  $` I \approx \frac{h}{2} \sum_{i=0}^{n} (f(x_i) + f(x_{i+1})) `$

- **Simpson's rule (parabolic approximation)**:  

  $` I \approx \frac{h}{3} \sum_{i=0}^{n} (f(x_i) + 4f(x_{i+1}) + f(x_{i+2})) `$

## 3. Interpolation and Approximation (Interpolation_and_Approximation.pdf)

To approximate functions, interpolation polynomials are used:

- **Lagrange polynomial**:  

  $` P_n(x) = \sum_{i=0}^{n} y_i \prod_{\substack{j=0 \\ j\neq i}}^{n} \frac{x - x_j}{x_i - x_j} `$

- **Least squares method** for function approximation:  

  $` S = \sum_{i=1}^{n} (y_i - P(x_i))^2 \to \min `$

## 4. Solving Nonlinear Equations (Nonlinear_Equations.pdf)

Numerical methods allow for finding the roots of equations $` f(x) = 0 `$

- **Bisection method (dichotomy)**: iterative interval halving.

- **Newton’s method (tangents)**:  

  $` x_{n+1} = x_n - \frac{f(x_n)}{f'(x_n)} `$

## 5. Numerical Methods for Solving ODEs (Ordinary_Differential_Equations.pdf)

Numerical methods allow solving differential equations of the form $` y' = f(x, y) `$

- **Euler’s method**:  

  $` y_{n+1} = y_n + h f(x_n, y_n) `$

- **Runge-Kutta method (4th order)**:  

  $` y_{n+1} = y_n + \frac{h}{6} (k_1 + 2k_2 + 2k_3 + k_4) `$

  where  

  $` k_1 = f(x_n, y_n), \quad k_2 = f(x_n + \frac{h}{2}, y_n + \frac{h}{2} k_1), \quad \dots `$

## 6. Solving Systems of Linear Equations (SLE.pdf)

Methods for solving systems of equations $` Ax = b `$, where $` A `$ is the coefficient matrix, and $` x `$ is the vector of unknowns:

- **Gaussian elimination** — sequential elimination of unknowns.

- **Jacobi method**:  

  $` x_i^{(k+1)} = \frac{1}{a_{ii}} \left( b_i - \sum_{\substack{j=1 \\ j\neq i}}^{n} a_{ij} x_j^{(k)} \right) `$

## 7. General Information on Numerical Methods (General.docx)

This section provides an introduction to numerical methods, their practical applications, and fundamental mathematical principles.<br>
___

## Implementation in Wolfram Mathematica

Each section includes examples and implementations in **Wolfram Mathematica**, allowing for hands-on exploration of numerical methods. These implementations demonstrate the practical application of theoretical concepts and provide tools for solving real-world computational problems.
