#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

void parentheses(int left_remain, int right_remain, int left_have, int right_have, string word){
	if(left_remain == 0 && right_remain == 0){
		if(left_have == right_have)
			cout << word << endl;
	}
	else if(left_remain == 0){
		if(left_have > right_have){
			parentheses(left_remain, right_remain - 1, left_have, right_have + 1, word + ")");
		}
		else if(left_have == right_have)
			cout << word << endl;
	}
	else if(right_remain == 0){
		if(left_have == right_have)
			cout << word << endl;
	}
	else{
		if(right_remain > left_have - right_have){
			parentheses(left_remain - 1, right_remain, left_have + 1, right_have, word + "(");
		}
		if(left_have > right_have){
			parentheses(left_remain, right_remain - 1, left_have, right_have + 1, word + ")");
			if(left_have == right_have)
				cout << word << endl;
		}
	}
}

int main(int argc, char* argv[]){
	if(argc > 2){
		int left = atoi(argv[1]);
		int right = atoi(argv[2]);
		string word = "";
		parentheses(left, right, 0, 0, word);
	}
	return 0;
}