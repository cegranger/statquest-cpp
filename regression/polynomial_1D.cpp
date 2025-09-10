/*
From Hackerrank
The Problem:
    Charlie wants to buy office space and has conducted a survey of the area. 
    He has quantified and normalized various office space features, and mapped 
    them to values between 0 and 1. Each row in Charlie's data table contains 
    these feature values followed by the price per square foot. However, some 
    prices are missing. Charlie needs your help to predict the missing prices.
    The prices per square foot are approximately a polynomial function of the 
    features, and you are tasked with using this relationship to predict the 
    missing prices for some offices.

    The prices per square foot, are approximately a polynomial function of the 
    features in the observation table. This polynomial always has an order less 
    than 4.

Input Format:
    Read from input.txt

    The first line contains two space separated integers, F and N. Over here, F
    is the number of observed features. N is the number of rows for which 
    features as well as price per square-foot have been noted. This is followed 
    by a table with F+1 columns and N rows with each row in a new line and each 
    column separated by a single space. The last column is the price per square 
    foot. The table is immediately followed by integer T followed by T rows 
    containing F space-separated columns.

Constraints:
    1 <= F <= 5
    5 <= N <= 100
    1 <= T <= 100
    0 <= Price Per Square Foot <= 10^6 0 <= Factor Values <= 1

Output Format:
    Answer can be found in output.txt
    T lines. Each line 'i' contains the predicted price for the 'i'th test case 
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

struct InputValues{
    // Variables from the problem 
    int F; // number of features between 1 and 5
    int N; // number of samples in the dataset between 5 and 100
    int T; // number of predictions to make between 1 and 100
    std::vector<std::vector<double>> features; // [N, F] values between 0 and 1
    std::vector<double> prices; // [N] values between 0 and 10E6
    std::vector<std::vector<double>> tests; // [N, F] values between 0 and 1
};

InputValues read_input_file(std::string filepath){
    // Variables from the problem 
    int F = 1; // number of features between 1 and 5
    int N = 5; // number of samples in the dataset between 5 and 100
    int T = 1; // number of predictions to make between 1 and 100
    std::vector<std::vector<double>> features(N, std::vector<double> (F)); // values between 0 and 1
    std::vector<double> prices(N); // values between 0 and 10E6
    std::vector<std::vector<double>> tests(T, std::vector<double> (F)); // values between 0 and 1

    std::string line;
    std::string delimeter = " ";
    std::ifstream inputfile (filepath);

    if (inputfile.is_open()) {

        // read the first line for F and N
        std::getline(inputfile, line);
        auto position = line.find(delimeter);
        F = std::stoi(line.substr(0, position));
        N = std::stoi(line.substr(position, line.size()));
        std::cout << F << " " << N << "\n";


        // resize the features and prices vectors
        features.resize(N);
        // Then resize each inner vector
        for (auto& row : features) {
            row.resize(F);
        }
        prices.resize(N);

        // read the features and prices
        for (int i = 0; i < N; i++){
            std::getline(inputfile, line);

            // read features values
            for (int j = 0; j < F; j++){
                auto end_pos = line.find(delimeter);
                features[i][j] = std::stof(line.substr(0, end_pos));
                line.erase(0, end_pos+1);
                std::cout << features[i][j] << " ";
            }

            // read price value
            prices[i] = std::stof(line);
            std::cout << prices[i] << "\n";
        }

        // read the number of predictions to make
        std::getline(inputfile, line);
        T = std::stoi(line);        
        std::cout << T << "\n";

        // resize the test vector
        tests.resize(T);
        for (auto& row : tests) {
            row.resize(F);
        }

        // read the test set
        for (int i = 0; i < T; i++){
            std::getline(inputfile, line);
            // read features values for the test
            for (int j = 0; j < F; j++){
                auto end_pos = line.find(delimeter);
                tests[i][j] = std::stof(line.substr(0, end_pos));
                line.erase(0, end_pos+1);
                std::cout << tests[i][j] << " ";
            }
                std::cout <<  "\n";
        }
        // close the file
        inputfile.close();
    } else {
        std::cout << "Unable to open file" << "\n";
    }

    return {F, N, T, features, prices, tests};
}

std::vector<std::vector<double>> invert_matrix(std::vector<std::vector<double>> input_matrix){
    std::vector<std::vector<double>> augmented_matrix = input_matrix;
    std::vector<std::vector<double>> inverted_matrix;

    // check if input is square
    if (input_matrix.size() == input_matrix[0].size()){
        int N = input_matrix.size();
        // resize the result matrix
        inverted_matrix.resize(N);
        // then resize each inner vector
        for (int i = 0; i < N; i++) {
            augmented_matrix[i].resize(N*2);
            inverted_matrix[i].resize(N);
        }

        if (N == 2) { // Direct formula: [[d,-b],[-c,a]] / det -> det: ad - bc
            double det = input_matrix[0][0] * input_matrix[1][1] - input_matrix[0][1] * input_matrix[1][0];
            if (abs(det) < 1e-10) {  // no div by zero plox
                std::cout << "det of 2x2 matrix is smaller than 1e-10...";
                exit(0);
            }
            inverted_matrix[0][0] = input_matrix[1][1] / det;
            inverted_matrix[0][1] = -input_matrix[0][1] / det;
            inverted_matrix[1][0] = -input_matrix[1][0] / det;
            inverted_matrix[1][1] = input_matrix[0][0] / det;
        } else if (N == 3) { // Adjugate method with cofactors it's piss
            std::vector<std::vector<double>> cofactor(N, std::vector<double> (N));
            double det = 0;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    // Get the 2x2 submatrix (excluding row i and col j)
                    double submatrix[2][2];
                    int sub_i = 0;
                    for (int row = 0; row < N; row++) {
                        if (row == i) continue;
                        int sub_j = 0;
                        for (int col = 0; col < N; col++) {
                            if (col == j) continue;
                            submatrix[sub_i][sub_j] = input_matrix[row][col];
                            sub_j++;
                        }
                        sub_i++;
                    }
                    
                    // Compute 2x2 determinant
                    double minor = submatrix[0][0] * submatrix[1][1] - submatrix[0][1] * submatrix[1][0];
                    
                    // Apply cofactor sign odd = -1
                    cofactor[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * minor;

                    // Calculate determinant using first row (i == 0)
                    if (i == 0) {
                        det += input_matrix[0][j] * cofactor[0][j];
                    } else if (i == 1 && std::abs(det) < 1e-10) {
                        std::cout << "Handle error - matrix not invertible...";
                        exit(0);
                    }                  
                }
            }
            
            // transpose to get adjugate and /det for inverse 
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    inverted_matrix[i][j] = cofactor[j][i] / det;
                }
            }

        } else { // Gauss-Jordan with backward elimination (and ideally partial pivoting) but not today
            // then fill identity matrix part
            for (int i = 0; i < N; i++) {
                augmented_matrix[i][i+N] = 1;
            }
            
            // forward elimination | column wise
            for (int i = 0; i < N;  i++) {
                // make top entry into a one | normalize row wise
                double pivot = augmented_matrix[i][i]; // the diagonal
                for (int j = i; j < N*2; j++) { // should i start from 0 always ? i dont think so.
                    augmented_matrix[i][j] = augmented_matrix[i][j] / pivot;
                } 
                // then use the normalized row to eliminate below the pivot
                for (int j = i + 1; j < N; j++) {
                    double factor = augmented_matrix[j][i];
                    for (int k = i; k < N*2; k++) {
                        augmented_matrix[j][k] -= factor * augmented_matrix[i][k];
                    }
                }
            }
            
            // backward elimination
            for (int i = N-1; i >= 0; i--) {
                for (int k = i - 1; k >= 0; k--) { // not sure about the i - 1...
                    double factor = augmented_matrix[k][i];
                    for (int j = i; j < N*2; j++) {
                        augmented_matrix[k][j] -= factor * augmented_matrix[i][j];
                    }
                }
            }
            
            // extract inverse from right side
            for (int i = 0; i < N; i++) {
                for (int j = N; j < N*2; j++) {
                    inverted_matrix[i][j-N] = augmented_matrix[i][j];
                }
            }
        }

    } else {
        std::cout << "input matrix is not square...";
        exit(0);
    }

    return inverted_matrix;
}


int main(){
   
    // get the input.txt file
    std::string filepath = "input.txt"; 
    // std::string filepath = "test_01.txt"; 
    InputValues input_values = read_input_file(filepath);
    

    /* do the polynomial regression y = aMx^M ... + a1x1 + a0x0 solving for optimal aM params
       assuming the most linear combination of the features is multiplied 

       symmetric Hankel matrix O(n^2) from a = A^-1 * y

        ⎡ a0 ⎤   =   ⎡     N         Σ(x_k)      ⎤^(-1)   ⎡     Σ(y_k)   ⎤
        ⎣ a1 ⎦       ⎣   Σ(x_k)      Σ(x_k^2)    ⎦        ⎣ Σ(y_k * x_k) ⎦
        
        or

        ⎡ a0 ⎤   =             1            ⎡  Σ(x_k^2)   -Σ(x_k)   ⎤   ⎡     Σ(y_k)   ⎤
                   ----------------------                         
        ⎣ a1 ⎦       N*Σ(x_k^2) - Σ(x_k)^2  ⎣   -Σ(x_k)       N     ⎦   ⎣ Σ(y_k * x_k) ⎦

        Gaussian elimination
        Hankel fast algorithms: https://doi.org/10.1016/j.laa.2010.12.001
    */

    int order = 4; // degree of the polynomial
    std::vector<std::vector<double>> A(order+1, std::vector<double> (order+1));
    std::vector<double> a(order+1, 0.0);
    std::vector<double> y(order+1, 0.0);

    
    // compute the relevant sums Σ(x_k)^2
    std::vector<double> A_sums(2 * order + 1, 0.0);
    for (int i = 0; i < input_values.N; i++ ) {
        double add_x = 1;
        for (int j = 0; j < input_values.F; j++) {
            add_x *= input_values.features[i][j];
        }
        // kinda smart ngl
        double x_power = 1.0;
        for (int k = 0; k <= 2 * order; k++) {
            if (k <= order) {
                y[k] += x_power * input_values.prices[i];  // Σ(x^k * y)
            }
            A_sums[k] += x_power;
            x_power *= add_x;
        }
    }

    // populate the A matrix from the sums
    for (int i = 0; i < A.size(); i++) {
        for (int j = i; j < A.size(); j++) {
            A[i][j] = A[j][i] = A_sums[i + j];
        }
    }


    // invert the A matrix
    std::vector<std::vector<double>> inverted = invert_matrix(A);

    // compute the coefficients
    for (int i = 0; i < inverted.size(); i++) {
        for (int j = 0; j < inverted.size(); j++) {
            a[i] += inverted[i][j] * y[j];
        }
    }
    
    std::vector<double> answers = {180.38, 1312.07, 440.13, 343.72};

    // print the tests predictions
    for (int i = 0; i < input_values.T; i++){
        double add_x = 1;
        for (int j = 0; j < input_values.F; j++) {
            // std::cout << input_values.tests[i][j] << " " << mult_x << "\n"; 
            add_x *= input_values.tests[i][j];
        }
        // Compute the answer
        double prediction = 0;
        for (int k = 0; k < order; k++){
            prediction += a[k] * std::pow(add_x, k);
        } 
        std::cout << prediction << " | " << answers[i] << " | " << abs(answers[i] - prediction)/answers[i] << "\n";
    }



    return 0;
}