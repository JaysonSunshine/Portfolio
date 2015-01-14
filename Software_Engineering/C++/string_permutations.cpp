#include <string>
#include <iostream>

using namespace std;

void swap(string* word, string::iterator it, int i){
	char swap;
	string::iterator it_new = it + i;
	swap = *it;
	*it = *it_new;
	*it_new = swap;
}

void permutations(string* word, string new_word, string::iterator it, int n, int length){
	if(n == 1){
		cout << new_word + *it << endl;
	}
	else{
		for(int i = 0; i < n; i++){
			swap(word, it, i);	
			permutations(word, new_word + *it, it + 1, n - 1, length);
			swap(word, it, i);
		}
	}
}

int main(int argc, char* argv[]){
	if(argc != 1){
		string word = argv[1];
		int n = word.length();
		string new_word = "";
		string::iterator it = word.begin();
		permutations(&word, new_word, it, n, n);
	}
	return 0;
}