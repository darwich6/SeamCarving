#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <cmath>

// function that removes seams
void removeSeam(std::vector<std::vector<int> >& matrix, int& rows, int& columns, int maxGreyScale){

    // now we need to calculate the energy matrix
    std::vector<std::vector<int> > energyMatrix;
    for(int i = 0; i < rows; ++i){
        std::vector<int> energyColumn;
        for(int j = 0; j < columns; ++j){
            // For the boundary cases, the difference = 0 if one of the pixel is outside of the given image.
            int currentPixel = matrix[i][j];
            int leftPixel, rightPixel, abovePixel, belowPixel, energyPixel;
            // we are the left edge, then the left becomes the current to get a difference of 0
            if(j - 1 < 0){
                leftPixel = currentPixel;
            } else {
                leftPixel = matrix[i][j-1];
            }
            if(j + 1 == columns){
                rightPixel = currentPixel;
            } else {
                rightPixel = matrix[i][j+1];
            }
            if(i - 1 < 0){
                abovePixel = currentPixel;
            } else { 
                abovePixel = matrix[i-1][j];
            }
            if(i + 1 == rows){
                belowPixel = currentPixel;
            } else {
                belowPixel = matrix[i+1][j];
            }
            // ok so here we need to calculate the energy of each pixel
            // e(i,j) = |v(i,j)-v(i-1,j)|+ |v(i,j)-v(i+1,j)|+ |v(i,j)-v(i,j-1)|+ |v(i,j)-v(i,j+1)|,
            energyPixel = std::abs(currentPixel-leftPixel) + std::abs(currentPixel-rightPixel) + std::abs(currentPixel-abovePixel) + std::abs(currentPixel-belowPixel);
            
            // test to see if they look right
            /*std::cout << "Left Pixel: " << leftPixel << "\n";
            std::cout << "Current Pixel: " << currentPixel << "\n";
            std::cout << "Right Pixel: " << rightPixel << "\n";
            std::cout << "Above Pixel: " << abovePixel << "\n";
            std::cout << "Below Pixel: " << belowPixel << "\n";
            std::cout << "Energy Pixel: " << energyPixel << "\n"; */
            energyColumn.push_back(energyPixel);
        }

        energyMatrix.push_back(energyColumn);
    }
    
    // Test to see if energy Matrix is populated correctly
    /*std::cout << "Energy Matrix: " << "\n";
    for(int i = 0; i < rows; ++i) {
        for (int j= 0; j < columns; ++j){
            std::cout << energyMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    } */

    // now we need to create the cumulative energy matrix
    std::vector<std::vector<int> > cumulativeEnergyMatrix;
    for(int i = 0; i < rows; ++i){
        std::vector<int> cumulativeEnergyColumn;
        for(int j = 0; j < columns; ++j){
            int leftAboveEnergy, aboveEnergy, rightAboveEnergy;
            // if we are in the first row, everything above should be zero
            if(i == 0){
                //so we are in the top row, then the currentEnergy will be the currentCumaltiveEnergy
                leftAboveEnergy = 0;
                rightAboveEnergy = 0;
                aboveEnergy = 0;
            } else {
                // ok so if we aren't at the top row, we need to determine the min of the three above it
                if(j - 1 < 0){
                    // this means that there isn't a leftAbove so we need to make sure it doesn't select it
                    leftAboveEnergy = INT_MAX;
                } else {
                    leftAboveEnergy = cumulativeEnergyMatrix[i-1][j-1];
                }
                // since we already account for the first row, we don't have to account for there not being an aboveEnergy
                aboveEnergy = cumulativeEnergyMatrix[i-1][j];
                // now we will look at rightAbove
                if(j + 1 == columns){
                    //this represents that their isnt anything to the right
                    rightAboveEnergy = INT_MAX;
                } else {
                    rightAboveEnergy = cumulativeEnergyMatrix[i-1][j+1];
                }
            }

            // now find the minimum and add that to the index of the energy matrix
            int minValue = leftAboveEnergy;
            if(aboveEnergy < minValue){
                minValue = aboveEnergy;
            }
            if(rightAboveEnergy < minValue){
                minValue = rightAboveEnergy;
            }

            // add it to the cumulative Min Column
            cumulativeEnergyColumn.push_back(minValue + energyMatrix[i][j]);
        }

        cumulativeEnergyMatrix.push_back(cumulativeEnergyColumn);
    }

    // Test to see if Cumulative Energy Matrix is populated correctly
    /*std::cout << "Cumulative Energy Matrix: " << "\n";
    for(int i = 0; i < rows; ++i) {
        for (int j= 0; j < columns; ++j){
            std::cout << cumulativeEnergyMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    } */

    // Now we need to remove the vertical seams
    /*The minimum seam is then calculated by backtracing from the bottom to the top edge. 
    First, the minimum value pixel in the bottom row of the accumulated cost matrix is located. 
    This is the bottom pixel of the minimum seam */
    
    int rowIndexDelete = 0;
    for(int i = 0; i < columns - 1; ++i){
        if(cumulativeEnergyMatrix[rows-1][rowIndexDelete] > cumulativeEnergyMatrix[rows - 1][i + 1]){
            //std::cout << "Made it here \n";
            rowIndexDelete = i + 1;
        }
    }

    //std::cout << "Did it we make it here \n";
    // delete the corresponding index of the bottom row
    matrix[rows - 1].erase(matrix[rows - 1].begin() + rowIndexDelete);

    //std::cout << "How bout here \n";

    // now we need to delete the smallest of three above the cumulativeEnergy Index
    for(int i = rows - 2; i >= 0; --i){
        int centerCumalative = rowIndexDelete;
        int leftAboveCumulative = 0;
        int rightAboveCumulative = 0;
        if(rowIndexDelete - 1 < 0){
            leftAboveCumulative = rowIndexDelete;
        } else {
            leftAboveCumulative = rowIndexDelete - 1;
        }
        if(rowIndexDelete + 1 == columns){
            rightAboveCumulative = rowIndexDelete;
        } else {
            rightAboveCumulative = rowIndexDelete + 1;
        }

        /*std::cout << "center: " << matrix[i][centerCumalative] << "\n";
        std::cout << "leftAbove: " << matrix[i][leftAboveCumulative] << "\n";
        std::cout << "rightAbove: " << matrix[i][rightAboveCumulative] << "\n"; */

        // default the remove to the left
        rowIndexDelete = leftAboveCumulative;
        //find the min with the center and the right
        if(cumulativeEnergyMatrix[i][centerCumalative] < cumulativeEnergyMatrix[i][rowIndexDelete]){
            rowIndexDelete = centerCumalative;
        }
        if(cumulativeEnergyMatrix[i][rightAboveCumulative] < cumulativeEnergyMatrix[i][rowIndexDelete]){
            rowIndexDelete = rightAboveCumulative;
        }

        /*std::cout << "Chosen delete: " << matrix[i][rowIndexDelete] << "\n"; */

        //delete the corresponding index
        matrix[i].erase(matrix[i].begin() + rowIndexDelete);
    } 

    // Test to see if we deleted vertical seams correctly
    /*std::cout << " Seams Removed Matrix: " << "\n";
    for(int i = 0; i < rows; ++i) {
        for (int j= 0; j < columns; ++j){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    } */

    --columns;

}

// transpose a vector of vectors of ints.
std::vector<std::vector<int> > transpose(std::vector<std::vector<int> > matrix, int rows, int columns) {
    
    // initialize a vector of vectors of the same size
    std::vector<std::vector<int> > transposedMatrix;

    //iterate through the given vector of vectors and transpose it. I becomes J and J becomes I
    for(int i = 0; i < columns; ++i){
        std::vector<int> transposeColumn;
        for(int j = 0; j < rows; ++j){
            transposeColumn.push_back(matrix[j][i]);
        }
        transposedMatrix.push_back(transposeColumn);
    }

    return transposedMatrix;
}


// Seam Carving Main Function
int main(int argc, char *argv[]) {
    
    //Check to see that the number of arguments is correct and the integer inputs are greater than 0
    if(argc != 4 || atoi(argv[2]) < 0 || atoi(argv[3]) < 0){

        std::cerr << "Number of command line arguments is incorrect \nPlease input ./seamCarving535 Filename NumOfVerticals NumOfHorizontals \n";

    } else {

        // Check to see if the file given is the correct extension
        std::string fileName = argv[1];
        if(fileName.substr(fileName.length() - 3, fileName.length()) != "pgm"){
            std::cerr << "Wrong File Input \nPlease input a file with extension \".pgm\" \n";
        } else{
            std::cout << "Correct input." << "\n";
            std::cout << "Please wait while seams are carved...\n";

            // Time to read in the file!
            std::fstream fileInput;
            fileInput.open(fileName);
            std::string currentLine;
            std::string comment;
            int columns;
            int rows;
            int maxGreyScale;

            if(fileInput.is_open()){
                // This should represent grabbing the "P2";
                getline(fileInput, currentLine);
                if(currentLine == "P2"){
                    // This should represent grabbing the comment if it exists
                    getline(fileInput, currentLine);
                    if(currentLine.substr(0, 1) == "#"){
                        comment = currentLine;
                    } else{
                        comment = "# No comment in original file";
                    }

                    // This should represent getting the height and width of the array
                    getline(fileInput, currentLine);
                    columns = stoi(currentLine.substr(0, currentLine.find(' ')));
                    rows = stoi(currentLine.substr(currentLine.find(' '), currentLine.length()));
                    /*std::cout << "Columns: " << columns << "\n";
                    std::cout << "Rows: " << rows << "\n"; */

                    //now grab the max value of the greyscale
                    getline(fileInput, currentLine);
                    maxGreyScale = stoi(currentLine);
                    /*std::cout << "Max GreyScale: " << maxGreyScale << "\n"; */

                    // make the array the size of the given input for rows and columns
                    std::vector<std::vector<int> > matrix;

                    // iterate through the rows and columns and write the file input into the array
                    for(int i = 0; i < rows; ++i){
                        // store the columns in a vector
                        std::vector<int> columnsVec;

                        for(int j = 0; j < columns; ++j){
                            //grab the current int and put it into the vector
                            int currentInt;
                            fileInput >> currentInt;
                            columnsVec.push_back(currentInt);
                        }
                        
                        // push back the column vector into the 2D vector
                        matrix.push_back(columnsVec);

                    }

                    // Test to display the matrix
                    /*std::cout << "Inputted Matrix: " << "\n";
                    for(int i = 0; i < rows; ++i) {
                        for (int j= 0; j < columns; ++j){
                            std::cout << matrix[i][j] << " ";
                        }
                        std::cout << std::endl;
                    } */
                    
                    // check to see if we grab the correct number of seams to remove vertically 
                    int verticalRemoves = atoi(argv[2]);
                    // check to see if we can actually remove that number of seams
                    if(verticalRemoves > rows){
                        std::cerr << "Error: Not enough rows from input to remove. \n";
                        std::cerr << "Command Line Vertical Removes: " << verticalRemoves << "\n";
                        std::cerr << "Number of Rows: " << rows << "\n";
                        return 1;
                    }
                    
                    for(int i = 0; i < verticalRemoves; ++i){
                        removeSeam(matrix, rows, columns, maxGreyScale);
                    }

                    // now we need to transpose the matrix
                    std::vector<std::vector<int> > transposedMatrix = transpose(matrix, rows, columns);

                    // Test to display the transposed Matrix
                    /*std::cout << "Transposed Matrix: " << "\n";
                    for(int i = 0; i < transposedMatrix.size(); ++i) {
                        for (int j = 0; j < transposedMatrix[0].size(); ++j){
                            std::cout << transposedMatrix[i][j] << " ";
                        }
                        std::cout << std::endl;
                    } */

                    // so now the original rows will the columns and the columns will be the rows
                    /*std::cout << "Transposed Rows: " << columns << "\n";
                    std::cout << "Transposed Columns: " << rows << "\n"; */

                    // check to see if we grab the correct number of seams to remove horizontally
                    int horizontalRemoves = atoi(argv[3]);
                    // check to see if we can actually remove that number of seams
                    if(horizontalRemoves > rows){
                        std::cerr << "Error: Not enough columns from input to remove. \n";
                        std::cerr << "Command Line Horizontal Removes: " << horizontalRemoves << "\n";
                        std::cerr << "Number of Columns: " << columns << "\n";
                        return 1;
                    } 

                    // now we can remove the horizontal seams which we will call the same function just with the transposed matrix
                    for(int i = 0; i < horizontalRemoves; ++i){
                        removeSeam(transposedMatrix, columns, rows, maxGreyScale);
                    }

                    // once we have removed the horizontal seams we will transpose the result to get it back in original form
                    std::vector<std::vector<int> > resultMatrix = transpose(transposedMatrix, columns, rows);

                    // Test to display the result Matrix
                    /*std::cout << "Result Matrix: " << "\n";
                    for(int i = 0; i < resultMatrix.size(); ++i) {
                        for (int j = 0; j < resultMatrix[0].size(); ++j){
                            std::cout << resultMatrix[i][j] << " ";
                        }
                        std::cout << std::endl;
                    } */

                    // now we will print out the output
                    std::ofstream outputFile(fileName.substr(0, fileName.length() - 4) + "_processed_" + std::to_string(verticalRemoves) + "_" + std::to_string(horizontalRemoves) + ".pgm");
                    std::string outputFileString = (fileName.substr(0, fileName.length() - 4) + "_processed_" + std::to_string(verticalRemoves) + "_" + std::to_string(horizontalRemoves) + ".pgm");
                    if(outputFile.is_open()){
                        outputFile << "P2\n";
                        outputFile << comment << "\n";
                        outputFile << std::to_string(columns) + " " + std::to_string(rows) << "\n";
                        outputFile << maxGreyScale << "\n";
                        for(int i = 0; i < rows; ++i){
                            for(int j = 0; j < columns; ++j){
                                outputFile << resultMatrix[i][j] << " ";
                            }
                            outputFile << "\n";
                        }
                        outputFile.close();
                        std::cout << "Output file is written to: " << outputFileString << "\n";
                    }else{
                        std::cerr << "Could not open output file: " << outputFileString << "\n";
                        return 1;
                    }


                } else {
                    std::cerr << "Incorrect form of PMG File. \n";
                }
            } else {
                std::cerr << "Could not open input file: " << fileName << "\n";
            }
        }
    }
}
