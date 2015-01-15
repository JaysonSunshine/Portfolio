#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

void parentheses(int left_remain, int right_remain, int left_have, int right_have, string word){
	if(left_remain == 0 && right_remain == 0){
		return;
	}
	else if(left_remain == 0){
		if(left_have > right_have){
			if(left_have == right_have + 1)
				cout << word + ")" << endl;
			else
				parentheses(left_remain, right_remain - 1, left_have, right_have + 1, word + ")");
		}
		
	}
	else if(right_remain == 0){
		return;
	}
	else{
		if(right_remain > left_have - right_have){
			parentheses(left_remain - 1, right_remain, left_have + 1, right_have, word + "(");
		}
		if(left_have > right_have){
			if(left_have == right_have + 1)
				cout << word + ")" << endl;
			parentheses(left_remain, right_remain - 1, left_have, right_have + 1, word + ")");
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