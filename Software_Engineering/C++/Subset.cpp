#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

vector<vector<int> > subset(int array[], int n, int max){
	vector<int> temp;
	vector<int> temp2;
	vector< vector<int> > collection;
	if(n == 1){	
		temp.push_back(max);
		temp2.push_back(array[0]);
		collection.push_back(temp);
		collection.push_back(temp2);
		return collection;
	}
	collection = (subset(array + 1, n - 1, max));
	vector<vector <int> > collection2(collection);
	for(unsigned int i = 0; i < collection.size(); i++){
			collection[i].push_back(array[0]);
	}
	collection.insert(collection.end(), collection2.begin(), collection2.end());
	return collection;
}

int main(int argc, char* argv[]){
	int n = atoi(argv[1]);
	int* array = new int[n];
	for(int i = 0; i < n; i++){
		array[i] = i;
	}
	vector<vector<int> > results;
	int max = 1 + *max_element(array, array + n * sizeof(int));
	results = subset(array, n, max);
	for(unsigned int i = 0; i < results.size(); i++){
		for(unsigned int j = 0; j< results[i].size(); j++){
			if(results[i][j] != max)
				cout << "[" << results[i][j] << "]";
			if(results[i][j] == max && results[i].size() == 1)
				cout << "[]";
		}
		cout << endl;
	}
}