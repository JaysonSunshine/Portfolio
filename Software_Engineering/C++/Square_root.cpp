#include <iostream>
#include <algorithm>

using namespace std;

double squareRoot(int num){
	if(num == 0 || num == 1)
		return num;
	double start, end;
	double precision = 0.00001;
	double array[100];
	double test = 1;
	if(num > 1){
		start = 1, end = num;
	}
	else
		start = 0, end = 1;
	while(test > precision){
		for(int i = 0; i < 100; i++){
			array[i] = start + i * ((end - start) / 99);
		}
		if(array[50] * array[50] > num)
			end = array[50];
		else
			start = array[50];
		test = num - (array[50] * array[50]);
		if(test < 0)
			test *= -1;
	}
	return array[50];
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