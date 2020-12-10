#include <iostream>   
#include <string>


using namespace std;

enum Code
{
    STOP, BUILD, ORIGIN, CANONICAL, PROCESS
};

struct SimplexMethod {
    double **matrix;
    bool *signMatrix;
    double *mainFoo;
    double *mainFooCopy;
    double *resultMatrix;
    int rowAmount; // conditions
    int colAmount; // coefficients
    string originTable = "";
    string canonicalTable = "";
    string resultFoo = "";
    bool processSuccess;

    //Solution steps
    void printSimplex() {
        for (int i = 0; i < rowAmount; i++)
        {
            for (int j = 0; j < colAmount * 2 + 2; j++)
            {
                cout.width(3);
                cout << " | " << matrix[i][j];
            }
            cout << endl;
        }
        for (int i = 0; i < colAmount * 2 + 2; i++)
        {
            cout << " | " << mainFoo[i];
        }
        cout << endl << endl;
    }

    void buildOriginTable() {
        originTable += " F = ";
        //add mainFoo string
        for (int i = 0; i < rowAmount; i++)
        {
            if (mainFoo[i] > 0) { originTable += '+'; };
            originTable += to_string((int)mainFoo[i]) + 'x' + to_string(i + 1) + ' ';
        }
        originTable += "--> max\n";
        //add inequalities strings
        for (int i = 0; i < rowAmount; i++)
        {
            originTable += '|';
            for (int j = 0; j < colAmount; j++)
            {
                originTable += ' ';
                if (matrix[i][j] > 0) { originTable += '+'; };
                originTable += to_string((int)matrix[i][j]) + 'x' + to_string(j + 1);
            }

            originTable += ' ';
            if (signMatrix[i] == 0) { originTable += "<= "; }
            else { originTable += ">="; };

            originTable += to_string((int)matrix[i][colAmount * 2]);
            originTable += '\n';
        }
        //add ending
        originTable += "|";
        for (int i = 0; i < rowAmount; i++)
        {
            originTable += " x" + to_string(i + 1);
        }
        originTable += ">= 0\n";
    }

    void toCanonical() {
        for (int i = 0; i < rowAmount; i++)
        {
            if (signMatrix[i])
            {
                signMatrix[i] = 0;
                // if greater & equal, then invert sign and variables
                for (int j = 0; j < colAmount; j++)
                {
                    //fix problem with -0 (thx ieee754)
                    if (matrix[i][j] == 0) { continue; };
                    matrix[i][j] *= -1;
                }
                if (matrix[i][colAmount*2] != 0) { matrix[i][colAmount * 2] *= -1; };           
            }
        }
    }

    void buildCanonicalTable() {
        canonicalTable += " F = ";
        //add mainFoo string
        for (int i = 0; i < rowAmount; i++)
        {
            if (mainFoo[i] > 0) { canonicalTable += '+'; };
            canonicalTable += to_string((int)mainFoo[i]) + 'x' + to_string(i + 1) + ' ';
        }
        canonicalTable += "--> max\n";
        //add inequalities strings
        for (int i = 0; i < rowAmount; i++)
        {
            canonicalTable += '|';
            for (int j = 0; j < colAmount; j++)
            {
                canonicalTable += ' ';
                if (matrix[i][j] > 0) { canonicalTable += '+'; };
                canonicalTable += to_string((int)matrix[i][j]) + 'x' + to_string(j + 1);
            }

            canonicalTable += " +y" + to_string(i + 1) + " = " + to_string((int)matrix[i][colAmount * 2]) + '\n';
        }
        //add ending
        canonicalTable += "|";
        for (int i = 0; i < rowAmount; i++)
        {
            canonicalTable +=  " x" + to_string(i + 1) + ',';
        }
        for (int i = 0; i < rowAmount; i++)
        {
            if (i < rowAmount - 1) { canonicalTable += ','; };
            canonicalTable += " y" + to_string(i + 1);
        }
        canonicalTable += ">= 0\n";

        //coefficients of mainFoo to left side of inequality
        for (int i = 0; i < colAmount; i++)
            mainFoo[i] *= -1;
    }

    void input() {
        processSuccess = true;

        cout << "Enter amount of coefficients" << endl;
        cin >> colAmount;
        cout << "Enter amount of inequalities" << endl;
        cin >> rowAmount;

        //simplex table, initialize with 0
        matrix = new double* [rowAmount];
        for (int i = 0; i < rowAmount; i++)
        {
            matrix[i] = new double[colAmount * 2 + 2];
            for (int j = 0; j < colAmount*2+2; j++)
            {
                matrix[i][j] = 0;
            }
        }
        //signs for simplex table
        signMatrix = new bool[rowAmount];
        resultMatrix = new double[colAmount];
        for (int i = 0; i < colAmount; i++)
        {
            resultMatrix[i] = 0;
        }
        mainFoo = new double[colAmount * 2 + 2];
        mainFooCopy = new double[colAmount * 2 + 2];
        for (int i = 0; i < colAmount * 2 + 2; i++)
        {
            mainFoo[i] = 0;
            mainFooCopy[i] = 0;
        }

        //fill simplex table with inequalities
        cout << "---------------------------------------------" << endl;
        for (int i = 0; i < rowAmount; i++)
        {
            for (int j = 0; j < colAmount; j++)
            {

                cout << "Enter coef for " << j + 1 << " variable in " << i + 1 << " inequality(row)\n";
                cin >> matrix[i][j];
            }
            cout << "Choose sign for " << i + 1 << " inequality(row)\n";
            cout << "0) - less & equal\n";
            cout << "1) - greater & equal\n";
            cin >> signMatrix[i];
            cout << "Enter right part of inequality\n";
            cin >> matrix[i][colAmount * 2];
        }

        //add main func
        cout << "---------------------------------------------" << endl;
        for (int i = 0; i < colAmount; i++)
        {
            cout << "Enter for " << i + 1 << " variable in main function\n";
            cin >> mainFoo[i];
            mainFooCopy[i] = mainFoo[i];
        }

        //add new vectors y1, y2, .. yn
        cout << "---------------------------------------------" << endl;
        for (int i = colAmount; i < colAmount*2; i++)
        {
            for (int j = 0; j < rowAmount; j++)
            {
                if ((i - colAmount) == j) { matrix[j][i] = 1; };
            }
        }
        
    }

    //simplex method
    void process() {
        cout << "---------------------------------------------\n";
        cout << "NEW ITERATION" << endl;
        int x = 0;
        int y = 0;
        double min = 0;
        //Optimality test
        for (int i = 0; i < colAmount * 2; i++)
        {
            //find minimum negative number in mainFoo (choose permissive col)
            if (mainFoo[i] < 0 && mainFoo[i] <= min) {
                min = mainFoo[i];
                x = i;
            }
        }
        //if all positive --> its optimal
        //base case of recursion (exit)
        if (min == 0) {
            cout << "Matrix is optimal!\n";
            return;
        };

        //find minimum positive number (choose permissive row)
        min = DBL_MAX;
        for (int i = 0; i < rowAmount; i++)
        {
            matrix[i][colAmount * 2 + 1] = matrix[i][colAmount * 2] / matrix[i][x];
            if (matrix[i][colAmount * 2 + 1] > 0 && matrix[i][colAmount * 2 + 1] < min) {
                min = matrix[i][colAmount * 2 + 1];
                y = i;
            }
        }

        // if we can't get min --> Solution doesn't exists
        if (min == DBL_MAX) { 
            processSuccess = false;
            return;
        }

        cout << "calculate permissive column (Rightmost column)\n";
        printSimplex();
        double permissiveElement = matrix[y][x];
        //create new permissive row
        for (int i = 0; i < colAmount * 2 + 1; i++) {
            matrix[y][i] /= permissiveElement;
        }
        cout << "New permissive row\n";
        printSimplex();

        double coef;
        //create new rows exclude permissive row
        for (int i = 0; i < rowAmount; i++)
        {
            if (i == y) { continue; }
            coef = matrix[i][x];
            for (int j = 0; j < colAmount * 2 + 1; j++)
            {
                matrix[i][j] -= coef * matrix[y][j];
            }
        }
        //don't forget about mainFoo matrix, its also part of simplex table
        coef = mainFoo[x];
        for (int i = 0; i < colAmount * 2 + 1; i++)
        {
            mainFoo[i] -= coef * matrix[y][i];
        }
        cout << "New rows\n";
        printSimplex();
        
        process();
    }

    void buildResult() {
        cout << "Final Matrix: \n";
        printSimplex();
        if (processSuccess == false) {
            resultFoo = "Solution doesn't exists\n";
            return;
        };
        int counter;
        // find bases
        for (int i = 0; i < colAmount; i++)
        {
            int tmp;
            counter = 0;
            int colCoordinate;
            for (int j = 0; j < rowAmount+1; j++)
            {
                // thats better to have matrix and mainFoo in single array, my bad
                // so thats how i go throw them
                if (j == rowAmount) {
                    if (mainFoo[i] != 0) {
                        counter++;
                        tmp = mainFoo[i];
                        colCoordinate = j;
                    }
                }
                else if (matrix[j][i] != 0) {
                    counter++;
                    tmp = matrix[j][i];
                    colCoordinate = j;
                }
            }

            //if col has only one value --> its basis
            if (counter == 1) {
                //lets calculate final coefs right there
                resultMatrix[i] = matrix[colCoordinate][colAmount * 2] / tmp;
            }
        }

        //build resultTable
        resultFoo = "Fmax(";
        for (int i = 0; i < colAmount; i++)
        {
            resultFoo += to_string(mainFooCopy[i]) + " * ";
            resultFoo += to_string(resultMatrix[i]);           
            i < colAmount - 1 ? resultFoo += " ; " : resultFoo += ") = ";
        }
        resultFoo += to_string(mainFoo[colAmount * 2]) + '\n';
    }

    void menu() {
        cout << "---------------------------------------------\n";
        cout << "Choose command:\n";
        cout << "1) - build table\n";
        cout << "2) - print origin table\n";
        cout << "3) - print canonical table\n";
        cout << "4) - process task\n";
        cout << "0) - stop programm\n";
        cout << "command: \n";
        int code;
        cin >> code;
        cout << endl;
        switch (code) {
        case BUILD:
            input();
            buildOriginTable();
            toCanonical();
            buildCanonicalTable();
            menu();
            break;
        case ORIGIN:
            cout << originTable;
            menu();
            break;
        case CANONICAL:
            cout << canonicalTable;
            menu();
            break;
        case PROCESS:
            process();
            buildResult();     
            cout << "Result of calculation:\n";
            cout << resultFoo;
            menu();
            break;
        case STOP:
            cout << "Stop program...\n";
            return;
        default:
            cout << "Wrong command!\n";
            menu();
            break;
        }
    }
};

int main()
{
    SimplexMethod* simplex = new SimplexMethod;
    simplex->menu();
}
