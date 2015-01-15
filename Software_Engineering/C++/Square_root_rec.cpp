#include <iostream>
#include <algorithm>

using namespace std;

double squareRootHelper(int num, double start, double end){
	double precision = 0.00001;
	double array[100];
	for(int i = 0; i < 100; i++){
		array[i] = start + i * ((end - start) / 99);
	}
	double test = num - (array[50] * array[50]);
	if(test < 0)
		test *= -1;
	if(test < precision)
		return array[50];
	if(array[50] * array[50] > num)
		end = array[49];
	else
		start = array[51];
	squareRootHelper(num, start, end);
}

double squareRoot(int num){
	if(num == 0 || num == 1)
		return num;
	double start, end;
	if(num > 1){
		start = 1, end = num;
	}
	else
		start = 0, end = 1;
	return squareRootHelper(num, start, end);
}

int main(int argc, char* argv[]){
	if(argc != 1){
		int num = atoi(argv[1]);
		string word = "";
		if(num < 0){
			word = "i";
			num *= -1;
		}
		cout << squareRoot(num) << word << endl;
	}
	return 0;
}