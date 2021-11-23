#include "mitra/Client.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
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

void mitraTestCode()
{
	bool usehdd = false, cleaningMode = false;

    Server server(usehdd, cleaningMode);
    Client client(&server, cleaningMode, 100);

    client.update(OP::INS, "test", 5, false);
    client.update(OP::INS, "test", 6, false);
    client.update(OP::INS, "test", 7, false);
    client.update(OP::DEL, "test", 6, false);
    vector<int> res = client.search("test");
    
    for (auto item : res) {
        cout << item << endl;
    }

}

int main () {
    
	vector <KVpair*> data;
	
	
	readFile(data);
	mergeSort(data, 0 , data.size() - 1);
	printKVpairVector(data);
	
	cout << endl << endl;
	
	mitraTestCode();
	
	cleanReadData(data);
	
	return 0;
}