#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cfloat>
#include <cmath>
#include <algorithm>
using namespace std;

// Define the aRow class
class aRow{
public:
	vector<string> myRow; //for storing the row data

	//methods
	aRow(){}// default constructor
	void display (){
	}; //display method
	string getColValue (int col){
        if (col < 0 || col >= myRow.size()) {
        cout << "Error, Please enter correct col number " << col << endl;
        return "Invalid";
    }
    //get myRow[col]
    return myRow[col];// this method is 
    } 
	~aRow(){}//destructor
	//other methods if you need them
};
// Define all the aRow methods 

class Table {
public:
	vector<string> colTypes; //storing column data types.
	vector<string> colNames; //storing column headers, if applicable.
	vector<aRow> myTable; // storing the rows with vector of user defined class.

	//methods
	Table (){}//constructor
	~Table(){}// destructor

	Table* selectColumns (int* colNums, int numElements){//returns a pointer to a Table object which has data 
										// for the array of column numbers in the argument.
    Table* newTable = new Table(); //creating a object newtable to store the selected columns
    newTable->colTypes.resize(numElements);//resizing the colTypes size as numElements
    for (int i = 0; i < numElements; i++) {
        if (colNums[i] >= 0 && colNums[i] < colTypes.size()) {//checking whether col index is valid or not
            newTable->colTypes[i] = colTypes[colNums[i]];//assigning the colNums to a col accordingly 
        } else {
            cout << "Error: It is a invalid column number " << colNums[i] << endl;
            newTable->colTypes[i] = "Invalid";//provides invalid output if col index is invalid
        }
    }
    newTable->colNames.resize(numElements);//resizing colNames size equal to numElements
    for (int i = 0; i < numElements; i++) {
        if (!colNames.empty() && colNums[i] >= 0 && colNums[i] < colNames.size()) {
            newTable->colNames[i] = colNames[colNums[i]];//checking for three conditions not empty case, >=0 and till colNames size
        } else {
            //If we donot have any column names then need to generate default headers
            newTable->colNames[i] = "Column " + to_string(i + 1); // names goes like Column1 Column2 like that..
        }
    }
    //  We are iterating through each row in the original table to get table accoriding to our requirement
    for (const aRow & originalRow : this->myTable) {
        aRow newRow;
        for (int i = 0; i < numElements; i++) {
            //Checking whether colName is within the given range or not
             if (colNums[i] >= 0 && colNums[i] <originalRow.myRow.size()) {
                newRow.myRow.push_back(originalRow.myRow[colNums[i]]);
            } else if (colNums[i] == originalRow.myRow.size()) { // Check if it's the last column
                newRow.myRow.push_back(originalRow.myRow.back());
            } else {
                cout << "Error: It is an invalid column number: " << colNums[i] << endl;
                newRow.myRow.push_back("Invalid");
            }
        }
        //Adding new row to the new table
        newTable->myTable.push_back(newRow);
    }
    return newTable;
}

	Table* subsetTable (int bRow, int eRow, int bCol, int eCol) {// returns a pointer to a Table object with 
																// beginning and end row and column numbers.
	Table* newTable1 = new Table();
    newTable1->colTypes.resize(eCol - bCol + 1);//to get range from start to end including inclusive range
    for (int i = bCol; i <= eCol; i++) {
        if (i >= 0 && i < colTypes.size()) {
            newTable1->colTypes[i - bCol] = colTypes[i];//assigning the col value
        } else {
            cout << "It is a invalid column number: " << i << endl;
            newTable1->colTypes[i - bCol] = "Invalid";//if the col number is invalid then throws an error
        }
    }
    // Checking if there are any col Names otherwise assign a header to each column
    if (!colNames.empty()) {
        newTable1->colNames.resize(eCol - bCol + 1);//to get inclusive range
        for (int i = bCol; i <= eCol; i++) {
            if (!colNames.empty() && i >= 0 && i < colNames.size()) {
                newTable1->colNames[i - bCol] = colNames[i];//assigning col Names if any
            } else {
                newTable1->colNames[i - bCol] = "Column " + to_string(i);//if no col Name found assgning a default farmat like Column i, where i will be 1,2...
            }
        }
    }
    // Validating if row and column indicies are within the range if out throws an error
    if (bRow < 0 || bRow >= this->myTable.size() || eRow < 0 || eRow >= this->myTable.size() ||
        bCol < 0 || bCol >= this->colTypes.size() || eCol < 0 || eCol >= this->colTypes.size()){//checking with table size and cols 
        cout << "Invalid row or column indices." << endl;
        return nullptr;//returing a null ptr
    }
    // Need to assign values to a new row to new table
    for (int i = bRow; i <= eRow; i++) {//iterating row and col to store in new table
        aRow originalRow1 = this->myTable[i];
        aRow newRow1;
        for (int j = bCol; j <= eCol; j++) {
            if (j < originalRow1.myRow.size()) {
                newRow1.myRow.push_back(originalRow1.myRow[j]);//assigning row values 
            } else {
                cout << "Invalid column index: " << j << " in row " << i << endl;//i represents row number and j represents col number
                
            }
        }        
        newTable1->myTable.push_back(newRow1);//pushing row values into new table
    }
    return newTable1;
}

	string columnAverage (int colNum){// returns the average of values in the column colNum.

	if (colNum < 0 || colNum >= colTypes.size()) {//checking the colnum is within the index or not
        cout << "It is a Invalid column number: " << colNum << endl;
        return "Invalid";
    }
    // Need to check whether coltype is int or float, otherwise we cannot perform average operation
    if (colTypes[colNum] != "int" && colTypes[colNum] != "float") {
        cout << "Column " << colNum << " is not a valid format." << endl;
        return "The column doesnot have numeric values";
    }
    double sum = 0.0;//to find sum of the col
    int count = 0;//to find number of valid numeric values

    for (const aRow& row : myTable) {
        if (colNum < row.myRow.size() && !row.myRow[colNum].empty() && row.myRow[colNum] != " " && !std::all_of(row.myRow[colNum].begin(), row.myRow[colNum].end(), ::isspace)) {// only if colNum is not empty or less than row size
            try {
                if (colTypes[colNum] == "int") {
                    try {//try and catch expectionals handling
                        sum += stoi(row.myRow[colNum]);
                        count++;// to find valid numeric counts
                    } catch (const std::invalid_argument& e) {
                        cout << "Invalid value in column " << colNum << ": " << e.what() << endl;
                    }
                } else if (colTypes[colNum] == "float") {
                    sum += stod(row.myRow[colNum]);
                    count++;
                }
                else if(std::all_of(row.myRow[colNum].begin(), row.myRow[colNum].end(), ::isspace)){
                    sum +=0;
                }
            } catch (const std::out_of_range& e) {
                cout << "Value out of range in column " << colNum << ": " << e.what() << endl;
            }
        }
    }

    if (count == 0) {
        cout << "No valid numeric values in column " << colNum << endl;
        return "No valid numeric values";
    }

    double average = sum / count;

    return to_string(average);
    }


	string columnMax (int colNum){// returns the max value of the column colNum.
	if (colNum < 0 || colNum >= colTypes.size()) {//valid col number or not
        cout << "Invalid column number: " << colNum << endl;
        return "Invalid";
    }
    // Checking if the column type is numeric (int or float)
    if (colTypes[colNum] != "int" && colTypes[colNum] != "float") {
        cout << "Column " << colNum << " is not numeric." << endl;
        return "Not Numeric";
    }
    // Initialize max value
    double maxVal = -DBL_MAX;

    // finding the maximum value and storing it in a val
    for (const aRow& row : myTable) {
        if (colNum < row.myRow.size() && !row.myRow[colNum].empty() && row.myRow[colNum] != " " && !std::all_of(row.myRow[colNum].begin(), row.myRow[colNum].end(), ::isspace)) {
            if (colTypes[colNum] == "int") {//for int type
                try {
                    int maxval = stoi(row.myRow[colNum]);//converts to integer
                    if (maxval > maxVal) {//iterating to check which number is mximum, if any number is found greater than update maxVal
                        maxVal = maxval;
                    }
                } catch (const std::invalid_argument& e) {
                    cout << "Invalid value in column " << colNum << ": " << e.what() << endl;
                }
            } else if (colTypes[colNum] == "float") {//for float type
                try {
                    double maxval = stod(row.myRow[colNum]);//converts to float
                    if (maxval > maxVal) {
                        maxVal = maxval;
                    }
                } catch (const std::invalid_argument& e) {
                    cout << "Invalid value in column " << colNum << ": " << e.what() << endl;
                }
            }
        }
    }
    // Convert the maximum value to a string and return it
    return to_string(int(maxVal));
}

	string columnMin (int colNum){ // returns the min value of the column colNum.
    if (colNum < 0 || colNum >= colTypes.size()) {//valid col number or not
        cout << "Invalid column number: " << colNum << endl;
        return "Invalid";
    }

   // Checking if the column type is numeric (int or float)
    if (colTypes[colNum] != "int" && colTypes[colNum] != "float") {
        cout << "Column " << colNum << " is not numeric." << endl;
        return "Not Numeric";
    }

    // Initialize min value
    double minVal = DBL_MAX;//DBL_MAX assigns min value, we can use -DBL_MIN as well

    // Iterate through the rows and find the minimum value
    for (const aRow& row : myTable) {
        if (colNum < row.myRow.size() && !row.myRow[colNum].empty() && row.myRow[colNum] != " " && !std::all_of(row.myRow[colNum].begin(), row.myRow[colNum].end(), ::isspace)) {
            if (colTypes[colNum] == "int") {
                try {//try and catch error handling expection(one way to handle expectional errors)
                    int minval = stoi(row.myRow[colNum]);//converts to integer
                    if (minval < minVal) {//checking for minval and updating "minVal" accordingly
                        minVal = minval;
                    }
                } catch (const std::invalid_argument& e) {
                    cout << "Invalid value in column " << colNum << ": " << e.what() << endl;
                }
            } else if (colTypes[colNum] == "float") {
                try {
                    double minval = stod(row.myRow[colNum]);//coverts to float
                    if (minval < minVal) {
                        minVal = minval;
                    }
                } catch (const std::invalid_argument& e) {
                    cout << "Invalid value in column " << colNum << ": " << e.what() << endl;
                }
            }
        }
    }
    return to_string(int(minVal));//return minVal
}

	int missingValues (int colNum) {// returns the number of missing values in the column colNum.
	int missingCount = 0;//to find the count of missingvalue

    // Check if colNum is valid
    if (colNum < 0 || colNum >= colTypes.size()) {
        cout << "Invalid column number: " << colNum << endl;
        return -1; //returning a value to indicate an error(return type should be an integer)
    }

    for (const aRow& row : myTable) {
        if (colNum < row.myRow.size()) {
            //checking if they are any empty colNum or whitespace
            if(row.myRow[colNum].empty() || row.myRow[colNum] == " " || std::all_of(row.myRow[colNum].begin(), row.myRow[colNum].end(), ::isspace)){
                missingCount= missingCount + 1;
            }  
        } 
        else if (colNum == row.myRow.size()) { // Check if it's the last column
            if (colNum >= row.myRow.size() || row.myRow.back().empty() || row.myRow.back() == " " || std::all_of(row.myRow[colNum].begin(), row.myRow[colNum].end(), ::isspace)) {//checking for empty, missing values and if colNum is greater than row size it means it is the last column 
                missingCount= missingCount + 1;
            }
        }
        
    }
    
    return missingCount;//return count of missing values
}

	void displayTableSummary(){ //Displays number of rows, number of columns, total number of missing values,
							//table Headers (1, 2, 3, ... in case there are no headers), followed by the tabular data.
    
    cout << "Displaying Table Summary:" << endl;
    cout << "Rows: " << myTable.size() << endl;
    cout << "Columns: " << colTypes.size() << endl;

    // Display total number of missing values
    int totalMissing = 0;
    for (int col = 0; col < colTypes.size(); col++) {
        totalMissing += missingValues(col);
    }

    cout << "Total Missing Values: " << totalMissing << endl;
    cout << "\nDisplaying Table: " << endl;

    // Checking if there are any colNames to print or otherwise assigning a default colNames
    if (!colNames.empty()) {//if not empty print colName which is found
        for (int i = 0; i < colTypes.size(); i++) {
            cout << colNames[i] << " ";
        }
        cout << endl;
    } else {
        for (int i = 0; i < colTypes.size(); i++) {
            cout << i+1<< " ";
        }
        cout << endl;
    }

    // Printing the table
    for (const aRow& row : myTable) {
        for (const string& cell : row.myRow) {
            cout << cell << " ";
        }
        cout << endl;
    }
	}
	void display (){//display the table with column names
	if (!colNames.empty()) {//if not empty print colName which is found
        for(const string & colName:colNames){
            cout << colName << " " ;
        }
        cout << endl;

    }
}
};

// Define all the Table methods & other utility functions you may need.

int main () {

	Table* aTable;

	int headerOrNot;
	string csvFileName;
   
	ifstream csvFile (csvFileName);
	int numColumns;
	int numRows;
   
	cin >> headerOrNot >> csvFileName;
	cin >> numColumns;

	//Create a Table instance
	aTable = new Table ();

	// next read the data types and store this *aTable object - Write code
	//read the col data types

    for (int i = 0; i < numColumns; i++) {
        string colType;
        cin >> colType;
        aTable->colTypes.push_back(colType);//reading col data types and storing them in a table(aTable)
    }
	cin >> numRows;
    
	// open and read the csv files that has numRows and numCols and store this in aTable - Write code
    csvFileName = "tests/" + csvFileName;
	std::ifstream csvfile(csvFileName);
    std::string line;
    bool isFirstRow = true;  // Flag to check if it's the first row
    while (getline(csvfile, line)) {
    if (line.empty()) {
        continue; // if they are any empty rows skip them
    }
    stringstream ss(line);
    aRow row;
    string cell;
    for (int i = 0; i < numColumns; i++) {
        if (getline(ss, cell, ',')) {
            row.myRow.push_back(cell);//we are checking each row, pushing them when we see ","
        }
    }
    if (isFirstRow) {
        aTable->colNames = row.myRow;  // Set column names from the first row
        isFirstRow = false;
    } else {
        aTable->myTable.push_back(row);
    }
    }

    // Close the file
    csvfile.close();

	(*aTable).displayTableSummary();

	int cols[] = {0,1,4,2};
	Table* bTable = (*aTable).selectColumns (cols, 4);
	(*bTable).displayTableSummary();

	Table* cTable = (*bTable).subsetTable (0, 6, 2, 3);
	(*cTable).displayTableSummary();

	cout << (*aTable).columnAverage (3) << endl;
	cout << (*bTable).columnMax (2) << endl;
	cout << (*cTable).columnMin (1) << endl;
	cout  << (*aTable).missingValues (4) << endl;
}