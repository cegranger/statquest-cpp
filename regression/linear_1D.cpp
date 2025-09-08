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

int main(){
   

    // get the input.txt file
    std::string filepath = "input.txt"; 
    // std::string filepath = "test_01.txt"; 
    InputValues input_values = read_input_file(filepath);
    

    
    /* do the linear regression y = mx + b solving for optimal m and b params
       assuming the most linear combination of the features is multiplied

        ⎡ b ⎤   =   ⎡     N         Σ(x_k)      ⎤^(-1)   ⎡     Σ(y_k)   ⎤
        ⎣ m ⎦       ⎣   Σ(x_k)      Σ(x_k^2)    ⎦        ⎣ Σ(y_k * x_k) ⎦
        
        or

        ⎡ b ⎤   =             1            ⎡  Σ(x_k^2)   -Σ(x_k)   ⎤   ⎡     Σ(y_k)   ⎤
                   ----------------------                         
        ⎣ m ⎦       N*Σ(x_k^2) - Σ(x_k)^2  ⎣   -Σ(x_k)       N     ⎦   ⎣ Σ(y_k * x_k) ⎦

    */

    double sum_x = 0;
    double sum_x2 = 0;
    double sum_y = 0;
    double sum_xy = 0;
    
    // Compute the relevant sums
    for (int i = 0; i < input_values.N; i++) {
        double mult_x = 1;
        for (int j = 0; j < input_values.F; j++) {
            mult_x = mult_x * input_values.features[i][j];
        }
        sum_x += mult_x;
        sum_x2 += mult_x*mult_x;
        sum_y += input_values.prices[i];
        sum_xy += mult_x * input_values.prices[i];
    }

    // Compute the determinant of inversed matrix
    double det = input_values.N * sum_x2 - sum_x * sum_x;
    double inv_det = 1/det;

    // Compute the b and m values according the the minimizer
    double b = (inv_det * sum_x2) * sum_y - (inv_det * sum_x) * sum_xy;
    double m = (inv_det * -sum_x) * sum_y + (inv_det * input_values.N) * sum_xy;

    // std::cout << sum_x <<  "\n"; 
    // std::cout << sum_x2 <<  "\n";
    // std::cout << sum_y <<  "\n";  
    // std::cout << sum_xy <<  "\n"; 
    std::cout << m << " " << b <<  "\n";

    // print the tests
    for (int i = 0; i < input_values.T; i++){
        double mult_x = 1;
        for (int j = 0; j < input_values.F; j++) {
            // std::cout << input_values.tests[i][j] << " " << mult_x << "\n"; 
            mult_x = mult_x * input_values.tests[i][j];
        }
        std::cout << m * mult_x + b << "\n"; 
    }


    return 0;
}