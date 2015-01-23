#include <iostream>
#include <stdlib.h>

using namespace std;

int fib(int n){
	if(n < 1)
		return 0;
	if(n <= 1)
		return 1;
	return fib(n - 1) + fib(n - 2);
}

int main(int argc, char* argv[]){
	int result;
	result = fib(atoi(argv[1]));
	if(result)
		cout << result << endl;
	else
		cout << "Fibonacci undefined for n < 1\n";
}