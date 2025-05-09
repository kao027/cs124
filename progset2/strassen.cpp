#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <cassert>
#include <chrono>
#include <climits>

typedef std::vector<std::vector<int>> matrix;

int CROSSOVER_POINT = 35;

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
                std::cerr << "Error: Not enough data in file." << std::endl;
                exit(1);
            }
            a[r][c] = entry;
        }
    }
    for (int r = 0; r < d; r++){
        for (int c = 0; c < d; c++){
            if (!(infile >> entry)) {
                std::cerr << "Error: Not enough data in file." << std::endl;
                exit(1);
            }
            b[r][c] = entry;
        }
    }
    infile.close();
}

matrix addMatrices(matrix &a, matrix &b, int sign = 1){ //
    //sign == 1 means add; sign == -1 means subtract
    assert(sign==1 || sign==-1);
    int d = a.size();
    //matrix ans(d, std::vector<int>(d));
    for (int r = 0; r < d; r++){
        for (int c = 0; c < d; c++){
            //ans[r][c] = a[r][c] + (b[r][c] * sign);
            a[r][c] += (b[r][c] * sign);
        }
    }
   // return ans;
   return a;
}

matrix gradeSchoolMultiply(matrix arr1, matrix brr1){
    //assert(arr1.size() == brr1.size());
    //assert(arr1[0].size() == brr1[0].size());
    int n = arr1.size();
    matrix ans(n, std::vector<int>(n));

    for (int i = 0; i < arr1.size(); i++){
        for (int j = 0; j < brr1[0].size(); j++){
            for (int k = 0; k < n; k++){
                ans[i][j] += arr1[i][k] * brr1[k][j];
            }
        }
    }
    return ans;
}


matrix strassenMultiply(const matrix &arr1, const matrix &brr1){
    assert(arr1.size() == brr1.size());
    assert(arr1[0].size() == brr1[0].size());
    int n = arr1.size();
    matrix ans(n, std::vector<int>(n));

    if (n == 1){
        ans[0][0] = arr1[0][0] * brr1[0][0];
        return ans;
    }
    
    if (n <= CROSSOVER_POINT) {
        std::cout << "hit crossover" << endl;
        return gradeSchoolMultiply(arr1, brr1);
    }

    if (n % 2 == 1){
        int padNum = n + 1;
        matrix pad1(padNum, std::vector<int>(padNum));
        matrix pad2(padNum, std::vector<int>(padNum));
        for (int i = 0; i < arr1.size(); i++){
            for (int j = 0; j < arr1[0].size(); j++){
                pad1[i][j] = arr1[i][j];
                pad2[i][j] = brr1[i][j];
            }
        }
        matrix padResult = strassenMultiply(pad1, pad2);
        matrix fixedResult(n, std::vector<int>(n));
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                fixedResult[i][j] = padResult[i][j];
            }
        }
        return fixedResult;
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
    //help because of 
    matrix dcsum(n, std::vector<int>(n));
    matrix ehsum(n, std::vector<int>(n));
    matrix bsubd(n, std::vector<int>(n));
    matrix gsube(n, std::vector<int>(n));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            dcsum[i][j] = d[i][j]+c[i][j];
            ehsum[i][j] = e[i][j]+h[i][j];
            bsubd[i][j] = b[i][j]-d[i][j];
            gsube[i][j] = g[i][j]-e[i][j];
        }
    }


    matrix p3 = strassenMultiply(dcsum,e);
    matrix p4 = strassenMultiply(d, gsube);
    matrix p7 = strassenMultiply(addMatrices(c,a,-1),addMatrices(e,f));
    matrix p5 = strassenMultiply(addMatrices(d,a),ehsum);
    matrix p1 = strassenMultiply(a, addMatrices(f,h,-1));
    matrix p2 = strassenMultiply(addMatrices(a,b),h);
    matrix p6 = strassenMultiply(bsubd,addMatrices(g,h));
   

    //RESULTS

    matrix bottomleft(n, std::vector<int>(n));
    
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            bottomleft[i][j] = p3[i][j] + p4[i][j];
        }
    }
    p6 = addMatrices(p6, p5);
    p4 = addMatrices(p4,p2,-1);
    matrix topleft = addMatrices(p6, p4);
    matrix topright = addMatrices(p2,p1);
   
    p5 = addMatrices(p5,p3,-1);
    p7 = addMatrices(p7, p1);

    matrix bottomright = addMatrices(p7, p5);
    
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


void generateFile(int d, const char* &fileName) {
    int size = d; 

    // Seed the random number generator
    std::srand(std::time(0));

    // Open the file to write the generated 2D array
    std::ofstream outFile(fileName);

    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    // Generate 2d^2 numbers
    for(int i = 0; i < 2; ++i){
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int randomValue = std::rand() % 2; // Randomly generate 0 or 1
                outFile << randomValue << " ";
            }
        }
        outFile << std::endl;
    }

    // Close the file
    outFile.close();
    std::cout << "Random 2D array has been written to " << fileName << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./strassen 0 dimension inputfile" << std::endl;
        return 1;
    }

    int flag = std::stoi(argv[1]);  // Read flag (currently unused)
    int dimen = std::stoi(argv[2]);     // Read dimension
    const char* filename = argv[3]; // Read input file name

    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error: Unable to open input file " << filename << std::endl;
        return 1;
    }
    
    matrix A(dimen, std::vector<int>(dimen));
    matrix B(dimen, std::vector<int>(dimen));

    generateFile(dimen, filename);
    generateMatrices(dimen, A, B, filename);
    
    //matrix gradeSchoolResult = gradeSchoolMultiply(A, B);
    /*for (int i = 0; i < dimen; i++) {
        std::cout << gradeSchoolResult[i][i] << std::endl;
    }*/


    // std::vector<int> testSizes = {4, 8, 16, 32, 64, 128, 256};
    // std::vector<int> crossoverValues = {8, 13, 16, 32};

    //runTests(testSizes, crossoverValues, "output.txt");
    //return 0;

    // generateFile(dimen, "experiments.txt");

    // std::cout << "Matrix A" << std::endl;
    // printMatrix(A);
    // std::cout << "Matrix B" << std::endl;
    // printMatrix(B);

    //Timing Grade-School Multiplication
    auto start = std::chrono::high_resolution_clock::now();
    matrix gradeSchoolResult = gradeSchoolMultiply(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> gradeSchoolTime = end - start;
    std::cout << "Grade-School Multiplication Time: " << gradeSchoolTime.count() << " seconds" << std::endl;

    // // // Timing Strassen Multiplication
    start = std::chrono::high_resolution_clock::now();
    matrix strassenResult = strassenMultiply(A, B);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> strassenTime = end - start;
    std::cout << "Strassen Multiplication Time: " << strassenTime.count() << " seconds" << std::endl;

    assert(gradeSchoolResult == strassenResult);

}
