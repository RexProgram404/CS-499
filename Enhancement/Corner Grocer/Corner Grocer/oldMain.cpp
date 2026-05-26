//Brian Har
//Program should check frequency of items from a list
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

class Counter {
public:
	Counter(string file);
	void pause(int duration);
	int countItem(string item);
	void getAllItemCount();
	void closeFile();
	void printItemFreq();
	void printItemFreqHist();
	void saveData();
	void countSpecificItem();

private:
	ifstream inFS;
	ofstream outFS;
	string item;
	string currWord;
	int itemFreq = 0;
	map<string, int> itemsCount;
	vector<string> key;
	char quit = 'y';

};

 Counter::Counter(string file){//parameter allows for use of other input files
	inFS.open(file);//opens file
		if (!inFS.is_open()) {//checks if file was open
			cout << "Can't open file" << endl;
		}
}
int Counter::countItem(string item) {
	while (!inFS.eof()) {
			inFS >> currWord;
			if (!inFS.fail()) {
				if (currWord == item) {
					++itemFreq;// counts instance of word
				}
			}
		}
	return itemFreq;
 }
void Counter::pause(int duration) {//duration to adust pause input duration
	cout << "Press enter to continue..." << endl; //pause till user input
	cin.clear();
	cin.ignore(duration);
}
void Counter::getAllItemCount() { //Counts all item frequencies and stores them to itemsCount
	inFS.clear();
	inFS.seekg(0, inFS.beg);
	while (!inFS.eof()) {
		inFS >> item;	
		if (itemsCount.count(item) != 1) { //Checks if word has already been counted for
			key.push_back(item);
			itemFreq = 0;		
			while (!inFS.eof()) {
				inFS >> currWord;
				if (!inFS.fail()) {
					if (currWord == item) {
						++itemFreq;
					}
				}
			}
			itemsCount.emplace(item, itemFreq+1); //+1 due to unknow error that causes it to not read the first instance of the item
			inFS.clear(); //clears and resets inFS stream to begining of file
			inFS.seekg(0, inFS.beg);
		}
		
	}
}
void Counter::closeFile() {
	inFS.close();//closes file
}
void Counter::printItemFreq() {
	for (string keys : key) {//goes through each key for itemsCount map
		cout << keys << " " << itemsCount.at(keys) << endl;
	}
	cout << endl;
}
void Counter::printItemFreqHist() {
	for (string keys : key) {
		cout << keys << " ";
		for (int i = 0; i < itemsCount.at(keys); i++) {//prints * equal to word frequency
			cout << "*";
		}
		cout << endl;
	}
}
void Counter::saveData() {
	outFS.open("frequency.dat");//creates file if it does not exist
	for (string keys : key) {//goes through each key for itemsCount map
		outFS << keys << " " << itemsCount.at(keys) << endl;
	}
	outFS << endl;
	cout << "\nData saved" << endl;
	outFS.close();
}
void Counter::countSpecificItem() {
	while (tolower(quit) != 'n') {
		cout << "Please input an item" << endl;
		cin >> item;


		cout << item << " appears " << this->countItem(item) << " times." << endl;

		cout << "Do you wish to try another item?(y) Or to continue to the other data? (n). \Press q to quit the program." << endl;
		cin >> quit;
		if (tolower(quit) == 'q') {
			exit(0);
		}

		cout << endl;
	}
}


int main(){
	string item;

	Counter count = Counter("CS210 project 3 input file.txt");

	count.countSpecificItem();

	count.pause(2);

	count.getAllItemCount();
	
	count.closeFile();

	count.printItemFreq();

	count.pause(1);

	count.printItemFreqHist();

	count.saveData();

	return 0;
}