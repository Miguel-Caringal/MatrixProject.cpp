// Miguel Caringal
// Final Project
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Creating a struct called Matrix
struct Matrix
{
    // Declaring the length of the matrix
    int N;
    // Declaring both 2d arrays
    vector<vector<double> > data;
    vector<vector<double> > identity;

    // Creating a function to create the identity matrix
    Matrix(int dimensions, vector<vector<double> > temp)
    {
        // switching the passed value of dimensions from the user to N, so it can be used throughout the struct
        N = dimensions;
        data = temp;
        // creating identity matrix
        identity = vector<vector<double> >();
        // Creating identity matrix itself, looping as long as it's less than dimensions
        for (int i = 0; i < N; i++)
        {
            // creating temp vector
            vector<double> vec = vector<double>();
            for (int j = 0; j < N; j++)
            {
                // if row number == column number it must be one
                if (i == j)
                {
                    vec.push_back(1);
                }
                // else its just 0
                else
                {
                    vec.push_back(0);
                }
            }
            // pushing the temp vec of the line into the 2d vec of identity
            identity.push_back(vec);
        }
    }

    // Function to determine whether to pivot or not
    bool pivot(int colNum)
    {
        // Looping down each column, and checking whether there is a descrepancy of 100000 or greater
        for(int i = 0; i < N - 1; i++)
        {
            for(int j = i + 1; j < N; j++)
            {
                // if there is a need to swap then return true
                if(abs(data[i][colNum] - data[j][colNum]) >= 100000)
                {
                    return true;
                }
            }
        }

        // else return false
        return false;
    }

    // Function that will normalize and pivot each column on the bottom left hand side
    void normalize(int colNum)
    {
        // If the need to pivot is there
        if(pivot(colNum))
        {
            // Declaring Variables
            double maxAbsVal = 0;
            double maxValRowIndex = 0;

            // Getting the highest value, but absolute as we care about the number of digits, not just how big the number is
            for(int i = 0; i < N; i++)
            {
                // checking whether the next value on the column is bigger based on abs digits, and getting the largest value
                if(abs(data[i][colNum]) > abs(maxAbsVal))
                {
                    maxAbsVal = data[i][colNum];
                    maxValRowIndex = i;
                }
            }
            // Swapping the Values for the original matrix
            vector<double> data_temp = data[colNum];
            data[colNum] = data[maxValRowIndex];
            data[maxValRowIndex] = data_temp;

            // Swapping the values for the identity matrix
            vector<double> identity_temp = identity[colNum];
            identity[colNum] = identity[maxValRowIndex];
            identity[maxValRowIndex] = identity_temp;
        }

        // Normalizing each column
        for(int i = colNum; i < N; i++)
        {
            // getting the divisor, what the whole row will be divided by
            double divisor = data[i][colNum];
            // this will skip the number if it's 0, as if it's 0 it does not need to be touched
            if(divisor == 0)
            {
                continue;
            }
            // dividing the entire row by the divisor
            for(int j = 0; j < N; j++)
            {
                // doing this for the original matrix
                data[i][j] /= divisor;
                // doing this for the identity
                identity[i][j] /= divisor;
            }
            cout << endl;
        }
    }

    // Function that will subtract the rows in the bottom left hand corner
    void lowerLeftRD()
    {
        // looping as long as it's less than number of rows
        for(int rowBound = 0; rowBound < N; rowBound++)
        {
            // call the normalizing function
            normalize(rowBound);
            // looping as long as it's greater than rowbound, so that it dosen't go over
            for(int i = N - 1; i > rowBound; i--)
            {
                // if it's already 0, no need to subtract at all
                if (data[i][rowBound] == 0)
                {
                    continue;
                }
                // the actual subtracting
                for(int j = 0; j < N; j++)
                {
                    data[i][j] -= data[rowBound][j];
                    identity[i][j] -= identity[rowBound][j];
                    // if the number happens to be epsilon, which i have defined for this project as 0.00000000001
                    if (data [i][j] <= 0.00000000001 && data [i][j] >= -0.00000000001)
                    {
                        // making it 0 instead
                        data [i][j] = 0;
                    }
                }
            }
        }
    }

    // Function that will calculate the row differences in the top right
    void upperRightRD()
    {
        // The process is slightly different, as normalization does not occur anymore, starting at rowbound going down to make sure it dosen't cross into the other part of the matrix
        for(int rowBound = N - 1; rowBound >= 0; rowBound--)
        {
            // looping till rowbound so it dosen't go over into the other part of the matrix
            for(int i = 0; i < rowBound; i++)
            {
                // determine what to multiply the numbers by
                double k = data[i][rowBound];
                for(int j = 0; j < N; j++)
                {
                    // multiply and then subtract the numbers
                    data[i][j] -= k * data[rowBound][j];
                    // doing the same for identity
                    identity[i][j] -= k * identity[rowBound][j];
                }
            }
        }
    }
    // Determining whether it's inversible or not, as if the program can't go any further it's non-inversible
    bool printInverse()
    {
        // calling the subtraction functions
        lowerLeftRD();
        upperRightRD();
        // checking if the original matrix ended up as the identity
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                // if the diagonal is not 1, output error message and return false
                if(i == j)
                {
                    if(data[i][j] != 1)
                    {
                        cout << "The matrix is not invertible!" << endl;
                        return false;
                    }
                }
                // if any other number isn't 0, output error message and return false
                else
                {
                    if(data[i][j] != 0)
                    {
                        cout << "The matrix is not invertible" << endl;
                        return false;
                    }
                }
            }
        }

        // Outputting the Matrix if it has an inverse
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                cout << setw(15) << identity[i][j];
            }
            cout << endl;
        }
        return true;
    }
};

int main()
{
    // Declaring Variables
    int dimensions;
    // Getting the user input, telling the user what the program does
    cout << "This program will calculate the inverse of an n by n matrix" << endl;
    cout << "Please enter the number of rows/columns for the matrix - INTEGER" << endl;
    cin >> dimensions;
    // While the dimensions are less than 2, invalid
    while(dimensions < 2)
    {
        cout << "Please enter value greater or equal to 2, and it must be an integer" << endl;
        cin >> dimensions;
    }
    // Creating 2d vector
    vector<vector<double> > matrixData = vector<vector<double> >();

    // Getting the user input for the matrix
    for (int i = 0; i < dimensions; i++)
    {
        // pushing back into the 2d vector
        matrixData.push_back(vector<double>());
        // Looping for each coordinate
        for (int j = 0; j < dimensions; j++)
        {
            // Getting the user input
            cout << "Please enter the value for the matrix at (" << i << "," << j << ")" << endl;
            double newVal = 0;
            cin >> newVal;
            // pushing back into the vector
            matrixData[i].push_back(newVal);
        }
    }
    // Passing values to the function
    Matrix m = Matrix(dimensions, matrixData);
    // calling the print inverse function, to print output
    m.printInverse();
    return 0;
}


