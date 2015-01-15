#include <algorithm>
#include <iostream>

using namespace std;

void sum_pairs(int nums[], int n, int target){
	int low = 0, high = n - 1;
	while(low < high){
		int sum = nums[low] + nums[high];
		if(sum == target){
			cout << "(" << nums[low] << "," << nums[high] << 
				")" << endl;
			low++;
			high--;
		}
		else if(sum > target){
			high--;
		}
		else{
			low++;
		}
	}
}

int main(int argc, char* argv[]){
	int n = atoi(argv[1]);
	int target = atoi(argv[2]);
	int* nums = new int [n];
	for(int i = 0; i < n; i++){
		nums[i] = (rand() % 20) - 10;
	}
	sort(nums, nums + n);
	for(int i = 0; i < n; i++){
		cout << nums[i] << ",";
	}
	cout << endl;
	sum_pairs(nums, n, target);
	return 0;
}