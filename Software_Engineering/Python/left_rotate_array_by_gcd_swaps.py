import copy
from fractions import gcd

def leftRotate(ar, k):
	if k == 0:
		return ar
	new_ar = copy.copy(ar)
	size = len(new_ar)
	if gcd(size,k) > 1:
		for t in range(0,gcd(size,k)):
			temp = new_ar[t]
			i = k + t
			for _ in range(0,gcd(size,k)):
				new_ar[(i - k) % size] = new_ar[i % size]
				i += k
				i % size
			new_ar[(-k + t) % size] = temp
	else:
		temp = new_ar[0]
		i = k
		while (i % size) != 0:
			new_ar[(i - k) % size] = new_ar[i % size]
			i += k
			i % size
		new_ar[-k % size] = temp
	return new_ar

def main():
	ar = [1,2,3,4,5,6,7,8,9]
	print ar
	for i in range(0,len(ar)):
		rotated = leftRotate(ar,i)
		print "rotate left ", i, "times", rotated

if __name__ == "__main__":
	main()