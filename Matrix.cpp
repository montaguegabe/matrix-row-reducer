//
//  Matrix.cpp
//  Matrix Row Reducer
//
//  Created by Gabe Montague on 12/31/16.
//  Copyright © 2016 Gabe Montague. All rights reserved.
//

#include <fstream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ios>

#include "StringSplit.hpp"
#include "Matrix.hpp"

using std::strtok;
using std::getline;
using std::ifstream;
using std::stod;
using std::invalid_argument;
using std::vector;
using std::cout;
using std::endl;
using std::setprecision;
using std::fixed;
using std::setw;

#pragma mark - Container methods

bool Matrix::loadFromFile(string filename) {
    
    int columns = -1;
    int columnsPrev = -1;
    
    string line;

    ifstream infile(filename);
    if (!infile) {
        return false;
    }
    
    auto constructed = vector<vector<double> >();
    
    while (getline(infile, line)) {
        
        // Create a row in the matrix
        auto row = vector<double>();
        columnsPrev = columns;
        columns = 1;
        
        // Tokenize the input line
        vector<string> tokens = split(line, ' ');
        for (string const token : tokens) {
                
            if (token[0] == '|') {
                _augBarLocation = columns;
                
            } else {
                double value;
                try {
                    value = stod(token);
                } catch (const std::invalid_argument&) {
                    return false;
                }
                
                row.push_back(value);
                columns++;
            }
        }
        
        // Check row length and append
        if (columns != columnsPrev && columnsPrev != -1) {
            return false;
        }
        constructed.push_back(row);
    }
    
    _vectors = constructed;
    return true;
}

void Matrix::print() const {
    
    cout << endl << "Matrix:" << endl;
    size_t column;
    
    for (auto const & row: _vectors) {
        column = 0;
        for (double const & value: row) {
            if (column == _augBarLocation - 1) {
                cout << " | ";
            }
            
            cout << setw(6) << fixed << setprecision(3) << value << " ";
            column++;
        }
        cout << endl;
    }
    cout << endl;
}


# pragma mark - Arithmetic methods

// Note: all row/column numbers are 0-based

void Matrix::swapRows(const size_t row1, const size_t row2) {
    
    _vectors[row1].swap(_vectors[row2]);
}

void Matrix::addRows(const size_t targetRow, const size_t sourceRow, const double factor) {
    
    const size_t columns = getColumns();
    auto & targetVector = _vectors[targetRow];
    auto const & sourceVector = _vectors[sourceRow];
    
    for (int i = 0; i < columns; i++) {
        targetVector[i] += sourceVector[i] * factor;
    }
}

void Matrix::scaleRow(const size_t row, const double factor) {

    const size_t columns = getColumns();
    auto & rowVector = _vectors[row];
    
    for (int i = 0; i < columns; i++) {
        rowVector[i] *= factor;
    }
}
