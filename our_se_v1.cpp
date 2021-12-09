#include "static/Amortized.h"
#include "build/static/OMAP.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include <math.h>
using namespace std;

struct KVpair 
{
	string name;
	double salary;
};

// alter mSize to size of Database
const int mSize = 10;

//************************************************************
//Adapted mergesort from this linke: https://www.programiz.com/dsa/merge-sort

void merge(vector<KVpair*> &data, int p, int q, int r) {

	// Create L ← A[p..q] and M ← A[q+1..r]
	int n1 = q - p + 1;
	int n2 = r - q;

	vector <KVpair*> L, M;

	for (int i = 0; i < n1; i++)
		L.push_back(data.at(p + i));
	for (int j = 0; j < n2; j++)
		M.push_back(data.at(q + 1 + j));

	// Maintain current index of sub-arrays and main array
	int i, j, k;
	i = 0;
	j = 0;
	k = p;

	// Until we reach either end of either L or M, pick larger among
	// elements L and M and place them in the correct position at A[p..r]
	while (i < n1 && j < n2) {
		if ((L.at(i))->salary < (M.at(j))->salary) {
			data.at(k) = L.at(i);
			i++;
		} else if((L.at(i))->salary == (M.at(j))->salary) {
			if((L.at(i)->name).compare(M.at(j)->name))
			{
				data.at(k) = L.at(i);
				++i;
			}
			else
			{
				data.at(k) = M.at(j);
				++j;
			}
		} else {
			data.at(k) = M.at(j);
			j++;
		}
		k++;
	}

	// When we run out of elements in either L or M,
	// pick up the remaining elements and put in A[p..r]
	while (i < n1) {
		data[k] = L[i];
		i++;
		k++;
	}

	while (j < n2) {
		data[k] = M[j];
		j++;
		k++;
	}
}

// Divide the array into two subarrays, sort them and merge them
void mergeSort(vector<KVpair*> &inputData, int l, int r) {
  if (l < r) {
    // m is the point where the array is divided into two subarrays
    int m = l + (r - l) / 2;

    mergeSort(inputData, l, m);
    mergeSort(inputData, m + 1, r);

    // Merge the sorted subarrays
    merge(inputData, l, m, r);
  }
}


//************************************************************



//reads in data from the USPS_dataset
//extracts the name and the salary of each line
//and saves it in the KVpair struct
void readFile(vector<KVpair*> &inputData)
{
	ifstream infile( "/Development/USPS_dataset.txt" );
	int i = 0;
	while (infile && i < 10)
	{
		++i;
		string s;
		if (!getline( infile, s )) break;

		istringstream ss( s );
		KVpair *record = new KVpair;

		//take only the name and salary from the database: i = 0 and i = 3
		int i = 0;
		while (ss)
		{
			string s;
			if (!getline( ss, s, ',' )) break;
			
			if (i == 0)
			{
				record->name = s;
			}
			else if (i == 3)
			{
				record->salary = stod(s);
				break;
			}
			++i;
		}

		inputData.push_back( record );
	}
}

//deletes the dynamically allocated KVpairs
void cleanReadData(vector<KVpair*> &data)
{
	for(auto & elem : data)
	{
		delete elem;
	}
}

//Prints the the name and salary of each KVpair in vector data
void printKVpairVector(vector<KVpair*> &data)
{
	for(int k = 0; (unsigned) k < data.size(); ++k)
	{
		cout << "Here is the name: " << (data.at(k))->name << " and the salary: " << (data.at(k))->salary << endl;
	}
}

//Fill OMAP with n elements after sorting them by their search attribute
void fillOMAP(OMAP &testmap, vector<KVpair*> &data)
{
	int current;
    string passin = ""; 
    int control = (data.at(0))->salary;
    int counter = 0;

    for (int i = 0; i < (mSize); i++){
        current = (data.at(i))->salary;
        if(current != control){
			passin = to_string(counter+1) + " " + to_string(i);
			testmap.insert((data.at(i-1))->salary, passin);
			control = current; 
			counter = i;
        } 
	if(i == mSize-1){
		passin = to_string(counter+1) + " " + to_string(i+1);
		testmap.insert((data.at(i))->salary, passin);	
	}
    }
}


string getSRCnode(string start, string end)
{
	int newStart = stoi(start);
	int newEnd = stoi(start);
	int i = 0;
	if(stoi(start) == stoi(end))
		return (start + "-" + end);
	while(newStart > 0 && newEnd < 10)
	{
		
		//position on the level from left to right of the node we are looking at
		int pos = newEnd / pow(2, i);
		if(newEnd + pow(2, i) >= stoi(end))
		{
			newEnd = newEnd + pow(2, i);
			break;
		}
		if(pos % 2 == 0)
		{
			newStart = newStart - pow(2, i);
		}
		else
		{
			newEnd = newEnd + pow(2, i);
		}
		++i;
	}
	
	return (to_string(newStart) + "-" + to_string(newEnd));
}


//https://www.codegrepper.com/code-examples/cpp/split+string+by+whitespace+c%2B%2B
vector<string> string_split(const string& str) {
	vector<string> result;
	istringstream iss(str);
	for (string s; iss >> s; )
		result.push_back(s);
	return result;
}

string getRangeQuery(double a, double b, OMAP &testmap)
{
	string first = testmap.find(a);
	string second = testmap.find(b);
	cout << "here" << endl;
	cout << first << endl;
	cout << second << endl;
	
	
	//vector<string> firstPair = string_split(first);
	//vector<string> secondPair = string_split(second);
	
	
	return getSRCnode(first, second);
}

//Fills Pibas with the data indexed using the SRC tree
/*void srcQueryConstruction(vector<KVpair*> &data, Amortized *client)
{

	for(int level = 0; level <= ceil(log2(data.size())); ++level)
	{
		for(int i = 1; (i + pow(2, level) - 1) <= pow(2, ceil(log2(data.size()))); i = i + ceil(pow(2,level-1)))
		{
			int end = (i + pow(2, level) - 1);
			//cout << to_string(i) + "-" + to_string(end) << endl;
			client->update(OP::INS, to_string(i) + "-" + to_string(end), pow(2, level), false);
		}
	}
	 
}*/

string makeQuery(OMAP &testmap)
{
	//read input from stdin
	string inputQuery;
	cout << "Enter your query:" << endl;
	getline(cin, inputQuery);
	//cout << inputQuery << endl;
	vector<string> parsedQuery = string_split(inputQuery);
	
	cout << "You queried: " << inputQuery << " and here are your results: " << endl;
	string res = getRangeQuery(stoi(parsedQuery.at(0)), stoi(parsedQuery.at(1)), testmap);
	return res;
}

int main () {
	
	//Initializes OMAP based on mSize
	bytes<Key> key{0};
	OMAP testmap(mSize, key);
	
	//read in then sort database
	vector <KVpair*> data;
	readFile(data);
	mergeSort(data, 0 , data.size() - 1);
	
	//fill OMAP with sorted data using salary as the key and the range of records
	//containing that salary as the value
	fillOMAP(testmap, data);
	
	
	//Amortized client(false, 50, 100);
	//initialize PiBas with SRC indexes
	//srcQueryConstruction(data, &client);
	printKVpairVector(data);
	
	testmap.insert(54363, "1-1");
	
	string inputQuery;
	cout << "Enter your query:" << endl;
	getline(cin, inputQuery);
	//cout << inputQuery << endl;
	vector<string> parsedQuery = string_split(inputQuery);
	string first = testmap.find(54363);
	string second = testmap.find(stoi(parsedQuery.at(1)));
	cout << "first: " << first << endl;
	cout << second << endl;
	
	cout << "You queried: " << inputQuery << " and here are your results: " << endl;
	
	
	
	
	
	
	
	
	//string output = makeQuery(testmap);
	//cout << output << endl;
	
	
	//test query of client
    /*vector<int> res = client.search("1-2");
	
	cout << "searching for (1,2): " << res.size() << " ||" << endl; 
    for (auto item : res) {
        cout << item << endl;
    }*/
	
	
	
	return 0;
}