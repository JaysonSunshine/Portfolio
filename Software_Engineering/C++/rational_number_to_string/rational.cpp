#include <iostream>
#include <string>
#include <stdlib.h>
#include <unordered_map>

using namespace std;

string rationalHelp(int num, int den, string* result, unordered_map<int, int>* table, int pos){
	int value = num / den;
	int rem = num - value * den;
	if(num % den == 0)
		return *result + to_string(value);
	if(table->find(num) == table->end()){
		(*table)[num] = pos;
		rem *= 10;
		while(rem < den){
			rem *= 10;
			*result += to_string(0);
			pos++;
		}
		*result += to_string(value);
		return rationalHelp(rem, den, result, table, pos + 1);
	}
	else{
		return (*result).substr(0, (*table)[num]) + "(" + (*result).substr((*table)[num], (*result).length()) + ")";
	}
	return "";
}

string rational(int num, int den){
	unordered_map<int, int> table;
	int value = num / den;
	int pos = 0;
	string result = "\"" + to_string(value);
	string result_help = "";
	if(num % den == 0)
		return result + "\"";
	else{
		int rem = num - value * den;
		rem *= 10;
		result += ".";
		while(rem < den){
			rem *= 10;
			result_help += to_string(0);
			pos++;
		}
		return result + rationalHelp(rem,den,&result_help, &table, pos) + "\"";
	}
}

int main(int argc, char* argv[]){
	if(argc > 2){
		int num = atoi(argv[1]);
		int den = atoi(argv[2]);	
		cout << rational(num, den) << endl;
	}
	else
		cout << "Requires two input parameters" << endl;
}
