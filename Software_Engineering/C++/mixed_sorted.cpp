#include <algorithm>
#include <iostream>
#include <string.h>

using namespace std;

enum Direction {UP, DOWN};

class MixedArray{
public:
	MixedArray(int length, int mod){
		len = length;
		array1 = new int [len];
		array2 = new int [len];
		for(int i = 0; i < len; i++){
			array1[i] = rand() % mod;
			array2[i] = rand() % mod;
		}
	}
	void print(int arrayNum);
	void sort(int arrayNum, Direction direction);
	int* concatenate();
private:
	int len;
	int* array1;
	int* array2;
};

int find(int target, int* array, int start, int end, int* count){
	++(*count);
	if(end < start)
		return -1;
	if(start == end)
		if(target == array[start])
			return start;
		else
			return -1;
	int mid = (start + end) / 2;
	if(target == array[mid])
		return mid;
	if((array[mid] > array[mid + 1] && array[mid] > array[mid - 1]) || (array[mid] == array[mid + 1] && array[mid - 1])){
		int left = find(target, array, start, mid -1, count);
		if(left != -1)
			return left;
		else
			return find(target, array, mid + 1, end, count);
	}
	else if(array[mid] > array[mid + 1] || array[mid] < array[mid - 1]){
		if(target > array[mid])
			return find(target, array, start, mid - 1, count);
		else
			return find(target, array, mid + 1, end, count);
	}
	if(target > array[mid])
		return find(target, array, mid + 1, end, count);
	else
		return find(target, array, start, mid - 1, count);
}

void MixedArray::print(int arrayNum){
	int* ptr;
	cout << "array" << arrayNum << " = ";
	if(arrayNum == 1)
		ptr = array1;
	else
		ptr = array2;
	for(int i = 0; i < len; i++){
		cout << ptr[i] << ",";
	}
	cout << endl;
}

void MixedArray::sort(int arrayNum, Direction direction){
	int* ptr;
	if(arrayNum == 1)
		ptr = array1;
	else
		ptr = array2;
	std::sort(ptr, ptr + len);
	if(direction == DOWN)
		reverse(ptr, ptr + len);
}

int* MixedArray::concatenate(){
	int* new_array = new int [len * 2];
	memcpy(new_array, array1, len * sizeof(int));
	memcpy(new_array + len, array2, len * sizeof(int));
	return new_array;
}

//argument 1 is arrazy size, argument 2 is range of values in array, argument 3 is target
int main(int argc, char* argv[]){
	//create target array
	if(argc >= 4){
		int length = atoi(argv[1]);
		int mod = atoi(argv[2]);
		MixedArray mixedArray(length, mod);
		//mixedArray.print(1);
		mixedArray.sort(1, UP);
		//mixedArray.print(1);
		//mixedArray.print(2);
		mixedArray.sort(2, DOWN);
		//mixedArray.print(2);
		int* ptr = mixedArray.concatenate();
		/*cout << "Total array = ";
		for(int i = 0; i < length * 2; i++){
			cout << ptr[i] << ",";
		}
		cout << endl;*/
		int target = atoi(argv[3]);
		int count = 0;
		int index = find(target, ptr, 0, length * 2, &count);
		cout << endl << index << "," << ptr[index] << ", count = " << count << endl;
	}
}