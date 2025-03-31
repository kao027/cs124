#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <cassert>
#include <chrono>
#include <climits>

typedef std::vector<std::vector<int>> matrix;
int CROSSOVER_POINT = 50;

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


matrix strassenMultiply(matrix &arr1, matrix &brr1){
    assert(arr1.size() == brr1.size());
    assert(arr1[0].size() == brr1[0].size());
    int n = arr1.size();
    matrix ans(n, std::vector<int>(n));

    if (n <= CROSSOVER_POINT) {
        return gradeSchoolMultiply(arr1, brr1);
    }

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
    matrix help = addMatrices(f,h,-1);
    matrix p1 = strassenMultiply(a, help);

    help = addMatrices(a,b);
    matrix p2 = strassenMultiply(help,h);

    help = addMatrices(c,d);
    matrix p3 = strassenMultiply(help,e);

    help = addMatrices(g,e,-1);
    matrix p4 = strassenMultiply(d, help);

    help = addMatrices(a,d);
    matrix help2 = addMatrices(e,h);
    matrix p5 = strassenMultiply(help,help2);

    help = addMatrices(b,d,-1);
    help2 = addMatrices(g,h);
    matrix p6 = strassenMultiply(help,help2);

    help = addMatrices(c,a,-1);
    help2 = addMatrices(e,f);
    matrix p7 = strassenMultiply(help,help2);

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


void generateFile(int d, const std::string &fileName) {
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

/*void runTests(const std::vector<int>& dimens, std::vector<int> crossovers, const std::string& outputFilename) {
    int upperBound = INT_MAX;
    std::ofstream outfile(outputFilename);  
    if (!outfile) {
        std::cerr << "Error opening output file!\n";
        return;
    }

    for (int cross : crossovers) {
        bool fastStrassen = true;
        outfile << "Testing with CROSSOVER_POINT = " << cross << "\n";

        CROSSOVER_POINT = cross;

        for (int dimen : dimens) {
            generateFile(dimen, "test.txt");
            matrix A(dimen, std::vector<int>(dimen));
            matrix B(dimen, std::vector<int>(dimen));
            generateMatrices(dimen, A, B, "test.txt");

            outfile << "dimen: " << dimen << "x" << dimen << "\n";

            auto start = std::chrono::high_resolution_clock::now();
            matrix gradeSchoolResult = gradeSchoolMultiply(A, B);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> naiveTime = end - start;
            outfile << "Grade-School Time: " << naiveTime.count() << " s\n";

            start = std::chrono::high_resolution_clock::now();
            matrix strassenResult = strassenMultiply(A, B);
            end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> strassenTime = end - start;
            outfile << "Strassen Time: " << strassenTime.count() << " s\n";
            outfile << "---------------------------------\n";
            if (strassenTime.count() > naiveTime.count()) {
                fastStrassen = false;
            }
        }
        if (fastStrassen) {
            upperBound = cross;
            break;
        }
    }

    std::cout << "Upper Bound on Crossover: " << upperBound << std::endl;
    outfile.close();  
    std::cout << "Test results written to " << outputFilename << std::endl;
}*/


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

    generateMatrices(dimen, A, B, filename);
    matrix gradeSchoolResult = gradeSchoolMultiply(A, B);
    for (int i = 0; i < dimen; i++) {
        std::cout << gradeSchoolResult[i][i] << std::endl;
    }


    // std::vector<int> testSizes = {4, 8, 16, 32, 64, 128, 256};
    // std::vector<int> crossoverValues = {8, 13, 16, 32};

    //runTests(testSizes, crossoverValues, "output.txt");
    //return 0;

    // generateFile(dimen, "experiments.txt");

    // std::cout << "Matrix A" << std::endl;
    // printMatrix(A);
    // std::cout << "Matrix B" << std::endl;
    // printMatrix(B);

    // Timing Grade-School Multiplication
    // auto start = std::chrono::high_resolution_clock::now();
    // matrix gradeSchoolResult = gradeSchoolMultiply(A, B);
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> gradeSchoolTime = end - start;
    // std::cout << "Grade-School Multiplication Time: " << gradeSchoolTime.count() << " seconds" << std::endl;

    // // // Timing Strassen Multiplication
    // start = std::chrono::high_resolution_clock::now();
    // matrix strassenResult = strassenMultiply(A, B);
    // end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> strassenTime = end - start;
    // std::cout << "Strassen Multiplication Time: " << strassenTime.count() << " seconds" << std::endl;

}
