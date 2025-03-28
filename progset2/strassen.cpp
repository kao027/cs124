#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <cassert>

int dimen = 4;

typedef std::vector<std::vector<int>> matrix;

void printMatrix(matrix m){
    for (int r = 0; r < m.size(); r++){
       for (int c = 0; c < m[r].size(); c++){
            std::cout << m[r][c] << " ";
        }
       std::cout << std::endl;
    }
    std::cout << std::endl;
}

void generateMatrices(int d, matrix &a, matrix &b, const char* filename){
    std::ifstream infile(filename);
    int entry;
    for (int r = 0; r < d; r++){
        for (int c = 0; c < d; c++){
            if (!(infile >> entry)) {
                std::cerr << "eof";
                exit(1);
            }
            a[r][c] = entry;
        }
    }
    for (int r = 0; r < d; r++){
        for (int c = 0; c < d; c++){
            if (!(infile >> entry)) {
                std::cerr << "eof" << std::endl;
                exit(1);
            }
            b[r][c] = entry;
        }
    }
    infile.close();
}

matrix addMatrices(matrix a, matrix b, int sign = 1){ //
    //sign == 1 means add; sign == -1 means subtract
    assert(sign==1 || sign==-1);
    int d = a.size();
    matrix ans(d, std::vector<int>(d));
    for (int r = 0; r < d; r++){
        for (int c = 0; c < d; c++){
            ans[r][c] = a[r][c] + (b[r][c] * sign);
        }
    }
    return ans;
}

matrix gradeSchoolMultiply(matrix arr1, matrix brr1){
    assert(arr1.size() == brr1.size());
    assert(arr1[0].size() == brr1[0].size());
    int n = arr1.size();
    matrix ans(n, std::vector<int>(n));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            for (int k = 0; k < n; k++){
                ans[i][j] += arr1[i][k] * brr1[k][j];
            }
        }
    }
    return ans;
}


matrix strassenMultiply(matrix arr1, matrix brr1){
    assert(arr1.size() == brr1.size());
    assert(arr1[0].size() == brr1[0].size());
    int n = arr1.size();
    matrix ans(n, std::vector<int>(n));

    if (n == 1){
        ans[0][0] = arr1[0][0] * brr1[0][0];
        return ans;
    }
    //split up
    n = n/2;
    
    //initializing submatrices

    matrix a(n, std::vector<int>(n));
    matrix b(n, std::vector<int>(n));
    matrix c(n, std::vector<int>(n));
    matrix d(n, std::vector<int>(n));

    matrix e(n, std::vector<int>(n));
    matrix f(n, std::vector<int>(n));
    matrix g(n, std::vector<int>(n)); 
    matrix h(n, std::vector<int>(n));

    // creating the submatrices; after : is submatrix from lecture notes
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            a[i][j] = arr1[i][j];     // top left of A:A
            b[i][j] = arr1[i][j+n];   // top right of A:B
            c[i][j] = arr1[i+n][j];   // bottom left of A:C
            d[i][j] = arr1[i+n][j+n]; // bottom right of A:D

            e[i][j] = brr1[i][j];     // top left of B:E
            f[i][j] = brr1[i][j+n];   // top right of B:F
            g[i][j] = brr1[i+n][j];   // bottom left of B:G
            h[i][j] = brr1[i+n][j+n]; // bottom right of B:H
        }
    }
    matrix p1 = strassenMultiply(a, addMatrices(f,h,-1));
    matrix p2 = strassenMultiply(addMatrices(a,b),h);
    matrix p3 = strassenMultiply(addMatrices(c,d),e);
    matrix p4 = strassenMultiply(d, addMatrices(g,e,-1));
    matrix p5 = strassenMultiply(addMatrices(a,d),addMatrices(e,h));
    matrix p6 = strassenMultiply(addMatrices(b,d,-1),addMatrices(g,h));
    matrix p7 = strassenMultiply(addMatrices(c,a,-1),addMatrices(e,f));

    //RESULTS
    matrix topleft = addMatrices(addMatrices(p4,p2,-1),addMatrices(p5,p6));
    matrix topright = addMatrices(p1,p2);
    matrix bottomleft = addMatrices(p3,p4);
    matrix bottomright = addMatrices(addMatrices(p1,p3,-1),addMatrices(p5,p7));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            ans[i][j] = topleft[i][j];
            ans[i][j+n] = topright[i][j];
            ans[i+n][j] = bottomleft[i][j];
            ans[i+n][j+n] = bottomright[i][j];
        }
    }

    return ans;
}

int main(){
    matrix A(dimen, std::vector<int>(dimen));
    matrix B(dimen, std::vector<int>(dimen));
    generateMatrices(dimen, A, B, "inputpractice.txt");
    
    /*std::cout << "Matrix A" << std::endl;
    printMatrix(A);
    std::cout << "Matrix B" << std::endl;
    printMatrix(B);
    */
    matrix result = strassenMultiply(A, B);
    //matrix result = gradeSchoolMultiply(A,B);
    std::cout << "A *  B = " << std::endl;
    printMatrix(result);

}
