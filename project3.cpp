// Project 3 
// CS DSA 5005 Computing Structures
// Fall 2023

#include <iostream>
#include <vector> // for array of transactions
#include <list> // for blockchain

#define MINING_DIFFICULTY 1024
#define LARGE_PRIME 1000003

using namespace std;

class transaction
{
	public:
	int tID; // transaction id
	int fromID; // source ID
	int toID; // target ID
	int tAmount; // how much is being transfered
	string timeStamp; // time of the transaction read from the input file
 
	transaction(){
		tID = 100;
    	fromID = 100;
    	toID = 100;
    	tAmount = 100;
    	timeStamp = "DefaultTimeStamp";
	} // TODO: default constructor
	transaction(int temptID, int tempfromID, int temptoID, int temptAmount, string temptimeStamp){
		tID = temptID;          // Initialize transaction ID
    	fromID = tempfromID;    // Initialize source ID
    	toID = temptoID;        // Initialize target ID
    	tAmount = temptAmount;  // Initialize transfer amount
    	timeStamp = temptimeStamp;
	} // non default constructor - default 100 for values
	void displayTransaction(){}
};

//TODO: Define the constructors and displayTransaction function.

class block
{
	public:
    int nonce;
	int hashValue = 0; //Default value of hash for a block. It will be changed once the block is mined.
	int blockNumber; // block number of the current block
	int currentNumTransactions; // how many transactions are currently in the block
	int maxNumTransactions; // how many maximum transactions can be in the block
	vector<transaction> bTransactions; // vector of transaction objects
	
	block(){
	blockNumber = 1; // Initialize block number to 1
    currentNumTransactions = 0;
    maxNumTransactions = 0;
	} // default constructor
	block(int bNumber, int maxTransactions); // non default constructor
	// insert method to insert a new transaction
	void insertTransaction(transaction newT);
	void computeHash(); //function for determining the hash of the block. Please refer to project description.
	void mineBlock();
	void displayBlock();
};

block::block(int bNumber, int maxTransactions)
{
	blockNumber = bNumber;
	currentNumTransactions = 0; // initialize
	this -> maxNumTransactions = maxTransactions;
}

// to insert transaction into the bTransactions vector in this block
void block::insertTransaction(transaction newT) 
{
	cout << "Inserting transaction to block #" << blockNumber << endl;
	bTransactions.push_back(newT);
	currentNumTransactions++;
	if(currentNumTransactions== maxNumTransactions){
		mineBlock();//checking if the block is full, if yes then calling mineblock
	}
	// displayBlock();
}
//Ascii value sum of all characters in a block, modulo a large prime number.
//This should give a unique integer for each unique block content.
void block::computeHash() {
//TODO: determine the value of hash for the block and store it in hashValue.
int asciiSum = 0;
for(const transaction& t : bTransactions) {
    	string tIDString = to_string(t.tID);//converting integer to string
		 //cout << "tID: " << t.tID << " fromID: " << t.fromID << " toID: " << t.toID << " tAmount: " << t.tAmount << " timeStamp: " << t.timeStamp << endl;
    
		for(char c: tIDString){
			asciiSum+=static_cast<int>(c);//summing ascill value of each character of the string
		}
		string fromIDString=to_string(t.fromID);
		for(char c: fromIDString){
			asciiSum+=static_cast<int>(c);
		}
		string toIDString=to_string(t.toID);
		for(char c: toIDString){
			asciiSum+=static_cast<int>(c);
		}
		string tAmountString=to_string(t.tAmount);
		for(char c: tAmountString){
			asciiSum+=static_cast<int>(c);
		}
        for(char c : t.timeStamp) {
           asciiSum+=static_cast<int>(c);
}
}
hashValue = asciiSum % LARGE_PRIME;//finding the hashvalue

}

void block::mineBlock() {
    nonce = 1; // Nonce initialized to 1.
    //TODO: determine the nonce satisfying the condition: hashValue times nonce divided by MINING_DIFFICULTY yeilds zero remainder.
    //Print the value of nonce (as shown in output file).
	computeHash();//calling the compute hash() function
	while(true){
		computeHash();
		if((hashValue* nonce) % MINING_DIFFICULTY == 0){
			break;
		}
		else{
			nonce++;
		}
		}
		cout<< "Block mined with nonce:"<< nonce<< endl;

}
// display the bTransactions vector that is a part of this block
void block::displayBlock()
{
	//TODO: Display the contents of the block, as shown in the output file.
	//cout << "Adding block #" << blockNumber << endl;
    //cout << "Inserting transaction to block #" << blockNumber << endl;
	
    if (currentNumTransactions > 0) {
        cout << "Block Number: " << blockNumber << " -- Number of Transaction: " << currentNumTransactions << endl;
        for (const transaction& t : bTransactions) {
            cout << t.tID << " " << t.fromID << " " << t.toID << " " << t.tAmount << " " << t.timeStamp << endl;
        }
    }
    else {
        cout << "Block Number: " << blockNumber << " -- Number of Transaction: 0" << endl;
    }

    cout << "Hash value: " << hashValue << endl;
}


class blockChain
{
    public:
	int currentNumBlocks; // maintain the size of the list/block chain list
	list<block> bChain;// blockchain as a linked list 
	int numTransactionsPerBlock;

    blockChain(){
	currentNumBlocks = 0;
    numTransactionsPerBlock = 0;
	}// default constructor
    blockChain(int tPerB){
		currentNumBlocks = 0;
		this->numTransactionsPerBlock = tPerB;
	} // non default constructor takes as argument the number of transactions allowed per block.
    // insert method to insert new transaction into the block chain - add new block if existing block is full
    // while inserting new block into list, insert front 
    void insertTransaction(transaction newT);
    void displayBlockChain();
};

//TODO: default and non-default constructors.

void blockChain::insertTransaction(transaction newT)
{
	
	//TODO: transaction newT to be inserted in the latest block in the linked list bChain.
    //Make sure number of transactions in a block do not exceed threshold. If equal to threshold,
    //Create new block and push it to the bChain list. When a block becomes full, mine it.
	if (bChain.empty() ) {//checking if block is empty
        block newBlock(currentNumBlocks + 1, numTransactionsPerBlock);
		cout << "Adding block #" << currentNumBlocks+1 << endl;
        newBlock.insertTransaction(newT);//inserting into new block
        bChain.push_back(newBlock); 
        currentNumBlocks++;
    }
	else {
	if(bChain.back().currentNumTransactions ==numTransactionsPerBlock){//if block is full
		block newBlock(currentNumBlocks + 1, numTransactionsPerBlock);
		cout << "Adding block #" << currentNumBlocks+1 << endl;
        newBlock.insertTransaction(newT);
        bChain.push_back(newBlock); 
        currentNumBlocks++;
	}
	else{
		bChain.back().insertTransaction(newT);
	}
    }
}

// display to loop through the block chain list
void blockChain::displayBlockChain()
{
	//TODO: display the block chain. Should call the display method of the individual blocks.
	cout << "Current number of blocks: " << currentNumBlocks << endl;
	 for (block& b : bChain) {//calling displayblock
        b.displayBlock(); 
    }
}


int main()
{
	int numTransactionsPerBlock; // max transactions per block

    //Redirected input:

	cin >> numTransactionsPerBlock;
	cout << "Number of transactions per block: " << numTransactionsPerBlock << endl;

	int totalNumTransactions; // total transactions to be read from the input file

	cin >> totalNumTransactions;
	cout << "Total number of transactions: " << totalNumTransactions << endl;

	// object of block chain
	blockChain* b1 = new blockChain(numTransactionsPerBlock);

	int temptID;
	int tempfromID;
	int temptoID;
	int temptAmount;
	string temptimeStamp; 

	//TODO: Read Transactions from the file and store in the block chain.

	// display the entire block chain
	for (int i = 0; i < totalNumTransactions; i++) {
			cin >> temptID >> tempfromID >> temptoID>> temptAmount>> temptimeStamp;//reading the transactions from the file
            transaction t(temptID, tempfromID, temptoID, temptAmount, temptimeStamp);//storing the value in the object
            b1->insertTransaction(t);//linked list the insert transaction
        }

	b1->displayBlockChain();

	return 0;
}

