import copy
from enum import Enum

class Dir(Enum):
	left = 0
	right = 1

def leftRotate(ar, k, Direction):
	new_ar = copy.copy(ar)
	new_ar.reverse()
	n = len(ar)
	if Direction == Dir.right:
		left = new_ar[0:k]
		right = new_ar[k:]
	else:
		left = new_ar[0:n - k]
		right = new_ar[n - k:]
	left.reverse()
	right.reverse()
	return left + right

def main():
	ar = [1,2,3,4,5,6,7,8,9]
	print ar
	print "left rotate by 3", leftRotate(ar, 3,Dir.left)
	print "right rotate by 3", leftRotate(ar, 3,Dir.right)

if __name__ == "__main__":
	main()