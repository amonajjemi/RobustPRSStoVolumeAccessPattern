#include "mitra/Client.h"
#include "build/mitra/OMAP.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
using namespace std;

struct KVpair 
{
	string name;
	double salary;
};



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

// alter mSize to size of Database
const int mSize = 10;

//Initializes OMAP based on mSize
bytes<Key> key{0};
OMAP testmap(mSize, key);
//Bid testbid = new Bid(string value)

int main () {
	vector <KVpair*> data;
	readFile(data);
	mergeSort(data, 0 , data.size() - 1);
	
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
cout << testmap.find((data.at(0))->salary) << endl;
cout << testmap.find((data.at(1))->salary) << endl;
cout << testmap.find((data.at(2))->salary) << endl;
cout << testmap.find((data.at(3))->salary) << endl;
cout << testmap.find((data.at(4))->salary) << endl;
cout << testmap.find((data.at(5))->salary) << endl;
cout << testmap.find((data.at(6))->salary) << endl;
cout << testmap.find((data.at(7))->salary) << endl;
cout << testmap.find((data.at(8))->salary) << endl;
cout << testmap.find((data.at(9))->salary) << endl;
return 0;
}
