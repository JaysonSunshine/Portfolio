#include <algorithm>
#include <iostream>

using namespace std;

int subsequent_sum(int* nums, int n){
	int max = 0, temp = 0, start = 0, end = -1;
	for(int i = 0; i < n; i++){
		temp += nums[i];
		if(temp < 0){
			start = i + 1;
			temp = 0;
		}
		if(temp > max){
			max = temp;
			end = i;
		}
	}
	if(end == -1)
		cout << "No positive subsequent_sum!\n";
	else{
		cout << endl << "Start == " << start << endl << "End = " << end << endl;
	}
	return max;
}

int main(int argc, char* argv[]){
	if(argc != 1){
		int n = atoi(argv[1]);
		int* nums = new int [n];
		for(int i = 0; i < n; i++){
			nums[i] = (rand() % 20) - 10;
			cout << nums[i] << ",";
		}
		cout << subsequent_sum(nums, n) << endl;
	}
	return 0;
}