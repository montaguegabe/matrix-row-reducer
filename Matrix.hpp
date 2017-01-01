//
//  Matrix.hpp
//  Matrix Row Reducer
//
//  Created by Gabe Montague on 12/31/16.
//  Copyright © 2016 Gabe Montague. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include <vector>
#include <string>

using std::vector;
using std::size_t;
using std::string;

// Implemented using a simple two dimensional vector of doubles
class Matrix {
public:
    Matrix() {
        
    }
    Matrix(unsigned rows, unsigned columns) { setSize(rows, columns); }
    
    // Container methods
    void setSize(unsigned rows, unsigned columns) {
         const auto emptyRow = vector<double>(columns, 0.0);
        _vectors = vector<vector<double> >(rows, vector<double>(emptyRow));
    }
    
    size_t getRows() {
        return _vectors.size();
    }
    
    size_t getColumns() {
        return getRows() ? _vectors[0].size() : 0;
    }
    
    
    bool loadFromFile(string filename); // Returns success
    void print() const;
    
    vector<double> & operator[] (const size_t index) {
        return _vectors[index];
    }
    
    // Arithmetic methods
    void swapRows(const size_t row1, const size_t row2);
    void addRows(const size_t targetRow, const size_t sourceRow, const double factor);
    void scaleRow(const size_t row, const double factor);
    
private:
    vector<vector<double> > _vectors;
    size_t _augBarLocation = -1;
    
    // Private methods
    
};

#endif /* Matrix_hpp */
