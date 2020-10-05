#include <iostream>
#include <vector>
#include <chrono>
#include <open
class Matrix2x2 {
public:

    Matrix2x2 () = default;

    Matrix2x2 (long long int a11, long long int a12, long long int a21, long long int a22) {
        matrix = {{a11,a12},{a21,a22}};
    }

    Matrix2x2 operator * (const Matrix2x2 &m2) {
        Matrix2x2 res;
        long long int a11, a12, a21, a22;
        a11 = matrix[0][0] * m2.matrix[0][0] + matrix[0][1] * m2.matrix[1][0];
        a12 = matrix[0][0] * m2.matrix[0][1] + matrix[0][1] * m2.matrix[1][1];
        a21 = matrix[1][0] * m2.matrix[0][0] + matrix[1][1] * m2.matrix[1][0];
        a22 = matrix[1][0] * m2.matrix[0][1] + matrix[1][1] * m2.matrix[1][1];

        res.matrix = {{a11,a12},{a21,a22}};

        return res;
    }

    std::vector<std::vector<long long int>>& GetMatrixVector() {
        return matrix;
    }

private:
    std::vector<std::vector<long long int>> matrix = {{1,1},{1,0}};
};





Matrix2x2 MatrixPowerN(Matrix2x2 & matr, int n) {
    Matrix2x2 resMatr(1, 0, 0, 1);
    while (n != 0) {
        if (n % 2 == 0) {
            matr = matr * matr;
            n /= 2;
        }
        else {
            resMatr = resMatr * matr;
            --n;
        }
    }
    return resMatr;
}

float NFibNumber (int num) {
    Matrix2x2 ForRes;
    ForRes = MatrixPowerN(ForRes, num);


//    std::cout << "{0,1} = " << ForRes.GetMatrixVector()[0][1] << std::endl;
//    std::cout << "{1,0} = " << ForRes.GetMatrixVector()[1][0] << std::endl;

    return ForRes.GetMatrixVector()[0][1];
}

int main() {
    int  number;
    std::cout << "Enter Fib Index : ";
    std::cin >> number;
    std::cout << "\n\n";

    g doublelon aa = 18446744073709551616;

    auto start = std::chrono::steady_clock::now();
    int res = NFibNumber(number);
    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    std::cout << "Result : " << res << " in " << elapsed_ms.count() << " ns \n";
    return 0;
}
