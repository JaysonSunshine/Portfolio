#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

//this uses linear space, but it can be altered to use constant space
void fibhelp(int n, int end, vector<long long>* memo){
	if(n > end)
		return;
	if(n == 1 || n == 2){
		(*memo).push_back(1);
		fibhelp(n + 1, end, memo);
	}
	else{
		(*memo).push_back((*memo)[n - 3] + (*memo)[n - 2]);
		fibhelp(n + 1, end, memo);
	}
}

long long fib(int n){
	if(n < 1)
		return 0;
	if(n <= 2)
		return 1;
	vector<long long> memo;
	fibhelp(1, n, &memo);
	return memo.back();
}

int main(int argc, char* argv[]){
	cout << fib(atoi(argv[1])) << "\n";
}