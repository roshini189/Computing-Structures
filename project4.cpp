#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <map>
#include <limits>
// Define the tree node structure
class myTree {
    public:
    std::vector<myTree*> children;
    std::set<int> values;
    // Constructor
    myTree(std::set<int> val) : values(val){}
    
};

// Function declarations
myTree* constructTree(const std::set<int>& nodeVals, const std::vector<std::vector<int>>& data, int dim, int maxDim);
void processPointQuery(myTree* root, const std::vector<int>& query, int dim, int maxDim,std::vector<int> querypath,std::vector<std::vector<int>>& QueryStack);
void processRangeQuery(myTree* root, const std::vector<std::pair<int, int>>& query,int dim, int maxDim,std::vector<int> querypath);
void printData(const std::vector<std::vector<int>>& data);
//void printTree(myTree* root, int level);
void printPointQuery(const std::vector<int>& query) ;
void printPointRangeQuery(const std::vector<std::pair<int, int>>& query) ;
//void printDataPoints(const std::vector<int>& query);
int main() {
    int numRows, numDimensions; 
    std::cin >> numDimensions; // Read the number of dimensions from the standard input
    std::cin >> numRows; // Read the number of rows from the standard input

    std::vector<int> row; 
    std::vector<std::vector<int>> data;

    // Reading file or input data
    for (int i = 0; i < numRows; i++) { // Looping through each row
        for (int j = 0; j < numDimensions; j++) { // Looping through each dimension of the row
            int temp;
            std::cin >> temp; // Read each element of the row from standard input
            row.push_back(temp); // Store the element in the row vector
        }
        data.push_back(row); // Add the row to the data set
        row.clear(); // Clear the row vector for the next iteration
    }

    std::set<int> firstDimVals; // Creating a set to store unique values of the first dimension

    // Extracting unique values of the first dimension from the data set
    for (const auto& row : data) {
        firstDimVals.insert(row[0]); // Inserting the first dimension value into the set
    }

    // Creating the multidimensional attribute tree with the constructed data
    myTree* root = constructTree(firstDimVals, data, 1, numDimensions);

    //TODO: Read data from the file and create the multidimensional attribute tree.
    //myTree* root = constructTree({}, data, 1, numDimensions);
    //TODO: Read and Process queries.
    // After constructing the tree
    //printTree(root, 0);

    int numQueries = 0;
    std::cin >> numQueries; // Reading the number of queries from standard input

    // Loop to process each query
    for (int i = 0; i < numQueries; i++) {
        char QueryType;
        std::cin >> QueryType; // Reading the type of query (Point or Range) from input

        if (QueryType == 'P') { // If it's a Point query
            std::vector<int> query(numDimensions);
            
            // Reading each dimension value of the point query
            for (int j = 0; j < numDimensions; j++) {
                std::cin >> query[j]; // Storing the dimension values in the query vector
            }
            
            // Process point query
            printPointQuery(query); // Printing the point query values
            std::vector<int> querypath; // Initializing vector to store query path
            std::vector<std::vector<int>> QueryStack; // Initializing stack to store query information
            processPointQuery(root, query, 0, numDimensions, querypath, QueryStack); // Processing the point query
        } else { // If it's a Range query
            std::vector<std::pair<int, int>> query(numDimensions);
            // Reading start and end values for each dimension of the range query
            for (int j = 0; j < numDimensions; j++) {
                int start, end;
                std::cin >> start >> end;
                query[j] = std::make_pair(start, end); // Storing start and end values in a pair
            }
            
            std::vector<int> querypath; // Initializing vector to store query path
            printPointRangeQuery(query); // Printing the range query values
            processRangeQuery(root, query, 0, numDimensions, querypath); // Processing the range query
        }
    }

    //TODO: Print input file data.
    printData(data); // Printing the input file data

    // Optional: Clean up tree memory
    // ...

    return 0; 
}

myTree* constructTree(const std::set<int>& nodeVals, const std::vector<std::vector<int>>& data, int dim, int maxDim) {
    if (dim > maxDim) {
        return nullptr; //if the dimension exceeds maxDim or no data
    }
    myTree* root = new myTree(nodeVals);//root node creation
    if (dim == maxDim) {
        return root;//if it comes to the last dimension no need to create child notes can return the root
    }
    // creating a map function to hold the the unique value in the current dimension
    std::map<int, std::vector<std::vector<int>>> childrenData;
    // Organize data based on the current dimension's value
    for (const auto& row : data) {
        if (row.size() > dim - 1) { // Checking if the dimension index exists in the row
            childrenData[row[dim - 1]].push_back(row);
        }
    }
    // Iterating through each unique value in this dimension
    for (const auto& pair : childrenData) {
        std::set<int> nextNodeVals; // Preparing the set for the next dimension
        std::vector<std::vector<int>> nextData; // creating a nextdata vector to store the next dimension
        // Filtering data for the next dimension and storing unique values
        for (const auto& row : pair.second) {
            if (row.size() > dim) { // Ensuring there is a next dimension
                nextNodeVals.insert(row[dim]);
                nextData.push_back(row);
            }
        }
        // Recursive call to create child node for the next dimension
        myTree* child = constructTree(nextNodeVals, nextData, dim + 1, maxDim);
        if (child != nullptr) {
            root->children.push_back(child);
        }
    }
    return root;
}

// void printTree(myTree* root, int level = 0  ) {
//     if (root) {
//         std::cout << std::string(level * 2, ' ') << "Node: ";
//         for (int val : root->values) {
//             std::cout << " " << val;
//         }
//         std::cout << std::endl;
//         for (myTree* child : root->children) {
//             printTree(child, level + 1);
//         }
//     }
// }

void printPointQuery(const std::vector<int>& query) { //Point Query print function
 std::cout << "Outputting Point Query results:" << std::endl;
    std::cout << "Point Query:";
    for (size_t i = 0; i < query.size(); ++i) {//iterating through the query
        std::cout << " " << query[i];
    }
    std::cout << std::endl;
}

void processPointQuery(myTree* root, const std::vector<int>& query, int dim, int maxDims, std::vector<int> querypath, std::vector<std::vector<int>>& QueryStack) {
    // Checking if the current dimension equals the maximum dimensions or if the root node is null
    if (dim == maxDims || !root) {
        // If the current dimension equals the maximum dimensions
        if (dim == maxDims) {
            // Printing the query path values if they match the maximum dimensions
            for (int val : querypath) {
                std::cout << val << " ";
            }
            std::cout << std::endl; 
        }
        return; 
    }

    int count = 0;
    auto it = root->values.begin();
    while (it != root->values.end()) {
    int val = *it;
    // Checking if the current value matches the query or if the query allows any value (-1)
    if (query[dim] == -1 || query[dim] == val) {
        // Adding the current value to the querypath
        querypath.push_back(val);
        // Recursively call the function on the child node (if it exists)
        myTree* childNode = nullptr;
        if (!root->children.empty()) {
            if (count < root->children.size()) {
                childNode = root->children[count];
            }
        }
        processPointQuery(childNode, query, dim + 1, maxDims, querypath, QueryStack);
        // Removing the last value from the querypath after the recursive call
        querypath.pop_back();
    }
    ++count; // Increment the counter for indexing child nodes
    ++it;    // Move iterator to the next value
}
}


void processRangeQuery(myTree* root, const std::vector<std::pair<int, int>>& query, int dim, int maxDims, std::vector<int> querypath) {
    // Checking if the current dimension equals the maximum dimensions or if the root node is null
    if (dim == maxDims || !root) {
        // If the current dimension equals the maximum dimensions
        if (dim == maxDims) {
            // Print the query path values if they match the maximum dimensions
            for (int val : querypath) {
                std::cout << val << " ";
            }
            std::cout << std::endl; 
        }
        return; 
    }

    int count = 0; // Initializing a counter for indexing child nodes

    // Iterating over the values in the current node
    for (int val : root->values) {
        // Fetching the low and high bounds for the current dimension from the query
        int low = query[dim].first;
        int high = query[dim].second;

        // Adjusting the bounds if they are -1 
        if (low == -1) {
            low = std::numeric_limits<int>::min();
        }
        if (high == -1) {
            high = std::numeric_limits<int>::max();
        }
        // Checking if the current value falls within the range 
        if (val >= low && val <= high) {
        // pushing values back to query
            querypath.push_back(val);
        // Recursively call the function on the child node 
            myTree* childNode = nullptr;
            if (!root->children.empty()) {
                childNode = root->children[count];
            }
            processRangeQuery(childNode, query, dim + 1, maxDims, querypath);
        // Removing the last value from the querypath after the recursive call
            querypath.pop_back();
        }
        count++; // Increment the counter for indexing child nodes
    }
}


void printPointRangeQuery(const std::vector<std::pair<int, int>>& query) {//Range query print function
    std::cout << "Outputting Range Query results:" << std::endl;
    std::cout << "Ranged Query:";
    for (const auto& pair : query) {
        std::cout << " " << pair.first << ":" << pair.second;
    }
    std::cout << std::endl;
}

void printData(const std::vector<std::vector<int>>& data) {//Print data function
    std::cout<< "Printing file data:"<<std::endl;
    for (const auto& row : data) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}
