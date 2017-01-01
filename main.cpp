//
//  Matrix.cpp
//  Matrix Row Reducer
//
//  Created by Gabe Montague on 12/31/16.
//  Copyright © 2016 Gabe Montague. All rights reserved.
//

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "Matrix.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::fabs;

static const double zeroThresh = 0.0000001;

// Helpers
static void solveMatrix(Matrix & matrix);

int main(int argc, char const *argv[]) {
    
    // Create matrix and undo matrix
    Matrix matrix;
    Matrix backupMatrix;
    
    bool success = false;
    
    while (!success) {
    
        cout << "Enter matrix filename, or type 'enter' to enter." << endl;
        string promptString;
        cin >> promptString;
        
        if (promptString == "enter" || promptString == "'enter'") {
            
            unsigned rows, columns;
            
            cout << "Enter the number of rows: ";
            cin >> rows;
            
            cout << "Enter the number of columns: ";
            cin >> columns;
            
            // Set the matrix size
            matrix.setSize(rows, columns);
            backupMatrix.setSize(rows, columns);
            
            cout << endl << "Note: you can enter '|' to place the augmentation bar." << endl;
            
            // Have the user input the matrix
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    
                    double input;
                    cout << "Row " << (i + 1) << " Column " << (j + 1) << ": ";
                    cin >> input;
                    
                    matrix[i][j] = input;
                }
            }
            success = true;

        // Input is a matrix file
        } else {
            success = matrix.loadFromFile(promptString);
            if (!success) {
                cout << "Error: File does not exist or is of invalid format." << endl << endl;
            }
        }
    }
    
    // Get matrix dimensions
    size_t rows, columns;
    rows = matrix.getRows();
    columns = matrix.getColumns();
    
    // Backup
    backupMatrix = matrix;
    cout << endl;
    
    // Main loop
    cout << "Note: All rows and columns should be specified with a 1-based number." << endl;
    while (true) {
        
        // Print matrix
        matrix.print();
        
        // Get command
        cout << endl << "Command: ('a'dd, 'm'ul, 's'wap, 'u'ndo, 'r'un solver, 'q'uit): ";
        string commandStr;
        cin >> commandStr;
        char command = commandStr[0];
        
        // Backup before forward commands
        if (command == 'a' || command == 'm' || command == 's' || command == 'r') {
            backupMatrix = matrix;
        }
        
        size_t targetRow, sourceRow;
        double factor;
        
        switch (command) {
            
            // Add
            case 'a':
                
                printf(" Source row: ");
                cin >> sourceRow;
                
                printf(" Factor: ");
                cin >> factor;
                
                cout << " Target row: ";
                cin >> targetRow;
                
                matrix.addRows(targetRow - 1, sourceRow - 1, factor);
                break;
            
            // Multiply
            case 'm':
                cout << " Factor: ";
                cin >> factor;
                
                cout << " Row: ";
                cin >> targetRow;
                
                matrix.scaleRow(targetRow - 1, factor);
                break;
            
            // Swap
            case 's':
                printf(" Source row: ");
                cin >> sourceRow;
                
                cout << " Target row: ";
                cin >> targetRow;
                
                matrix.swapRows(targetRow - 1, sourceRow - 1);
                break;
                
            // Undo
            case 'u':
                cout << endl << endl << "Matrix reverted. You may only undo once at a time." << endl;
                matrix = backupMatrix;
                break;
                
            case 'r':
                
                solveMatrix(matrix);
                
                cout << endl << "Solved matrix." << endl;
                break;
                
            case 'q':
                cout << endl;
                return 0;
                
            // Unrecognized
            default:
                cout << endl << endl << "Invalid command. Use only one letter." << endl;
                break;
        }
        
        cout << endl;
    }
    
    return 0;
}

void solveMatrix(Matrix & matrix) {
    
    size_t rows = matrix.getRows();
    size_t columns = matrix.getColumns();
    
    // Our pivot entry
    size_t pivotRow = 0;
    size_t pivotColumn = 0;
    
    while (true) {
        
        // Part 1: Clean the row to ones and zeros
        
        // Scale to 1
        if (matrix[pivotRow][pivotColumn] != 1.0) {
            double factor = 1.0 / matrix[pivotRow][pivotColumn];
            matrix.scaleRow(pivotRow, factor);
        }
        
        // Clear the column
        for (int i = 0; i < rows; i++) {
            
            if (i == pivotRow) continue;
            
            double value = matrix[i][pivotColumn];
            
            // Subtract multiples
            if (fabs(value) >= zeroThresh) {
                matrix.addRows(i, pivotRow, -value);
            }
        }
        
        matrix.print();
        
        
        // Part 2: Select a new pivot, or break if we are unable. TODO: Select pivot to minimize rounding error.
        
        size_t potentialPivotRow = pivotRow + 1;
        size_t potentialPivotColumn = pivotColumn + 1;
        
        // Check that we have room to search for potential pivots
        if (potentialPivotRow == rows || potentialPivotColumn == columns) {
            return;
        }
        
        // Search for another non-zero pivot to use
        while (true) {
            
            // Check (absolute) value of potential pivot
            double potentialPivotValue = fabs(matrix[potentialPivotRow][potentialPivotColumn]);
            if (potentialPivotValue >= zeroThresh) break;
            
            // Search down the column first
            potentialPivotRow++;
            
            // Check if we've exhausted the column
            if (potentialPivotRow == rows) {
                
                // Move on to the next column
                potentialPivotRow = pivotRow + 1;
                potentialPivotColumn++;
                
                // If we are out of columns, we are out of potential pivots
                if (potentialPivotColumn == columns) {
                    return;
                }
            }
        }
        
        // We like a nice diagonal alignment for pivots, so swap the rows to achieve this
        if (potentialPivotRow != pivotRow + 1) matrix.swapRows(potentialPivotRow, pivotRow + 1);
        pivotRow = pivotRow + 1;
        pivotColumn = potentialPivotColumn;
    }
}
