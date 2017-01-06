//
//  main.cpp
//  Random Matrix
//
//  Created by Gabe Montague on 12/31/16.
//  Copyright © 2016 Gabe Montague. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using std::ofstream;
using std::cout;
using std::endl;
using std::string;

int main (int argc, char const *argv[]) {
    
    // Open write file based on path argument
    if (argc <= 1) {
        cout << "Please specify an output path." << endl;
        return 0;
    }
    
    string outputPath = argv[1];
    cout << "Writing to " << outputPath << endl;
    ofstream outputFile;
    outputFile.open(outputPath);
    
    // Output a random matrix
    const int rows = 4;
    const int columns = 8;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            
            if (j == 4) outputFile << "| ";
            
            int value = rand() % 6 - 3;
            outputFile << value << " ";
        }
        
        outputFile << "\n";
    }
    
    outputFile.close();
    
    cout << "Done" << endl;
    
    return 0;
}
