// https://www.luogu.org/problemnew/show/P4035
// [JSOI2008]球形空间产生器

#include <bits/stdc++.h>
using namespace std;

class GaussianElimination {
public:
    // note this class owns A and X
    const int N;
    double **A;
    double *X;

    static constexpr double EPSLON = 1e-7;

    GaussianElimination(int N_): N(N_) {
        alloc();
    }

    ~GaussianElimination() {
        dealloc();
    }

    bool solve() {
        for (int k = 0; k < N; k++) {
            // find principal
            double principal = fabs(A[k][k]);
            int i0 = k;
            for (int i = k; i < N; i++) {
                if (fabs(A[i][k]) > principal) {
                    principal = fabs(A[i][k]);
                    i0 = i;
                }
            }
            // swap rows
            for (int j = 0; j < N; j++) {
                swap(A[k][j], A[i0][j]);
            }
            swap(X[k], X[i0]);

            if (fabs(A[k][k]) < EPSLON) return false;

            for (int i = k+1; i < N; i++) {
                A[i][k] = A[i][k] / A[k][k];
                for (int j = k+1; j < N; j++) {
                    A[i][j] = A[i][j] - A[i][k] * A[k][j];
                }
                X[i] = X[i] - A[i][k] * X[k];
            }
        }

        // backward step
        for (int i = N-1; i >= 0; i--) {
            for (int j = i+1; j < N; j++) {
                X[i] = X[i] - A[i][j] * X[j];
            }
            X[i] = X[i] / A[i][i];
        }
        return true;
    }

private:
    void alloc() {
        A = new double*[N];
        for (int i = 0; i < N; i++) {
            A[i] = new double[N]{};
        }
        X = new double[N]{};
    }

    void dealloc() {
        for (int i = 0; i < N; i++) {
            delete[] A[i];
        }
        delete[] A;
        delete[] X;
    }
};

double coord[12][12];

int main() {
    int n;
    scanf("%d", &n);
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%lf", &coord[i][j]);
        }
    }

    GaussianElimination ge(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double x1 = coord[i+1][j];
            double x0 = coord[i][j];
            ge.A[i][j] = 2 * (x1 - x0);
            ge.X[i] += (x1*x1 - x0*x0);
        }
    }
    ge.solve();

    for (int i = 0; i < n-1; i++) {
        printf("%.3f ", ge.X[i]);
    }
    printf("%.3f", ge.X[n-1]);

    return 0;
}
