import sys

def isPrime(N):
	for i in range(2, N / 2 + 1):
		if N % i == 0:
			return False
	return True

def primes(N):
	if N == 1:
		return 0
	if N == 2:
		return 1
	count = 1
	for i in range(3, N + 1):
		if isPrime(i):
			count += 1
	return count

def main(args):
	print primes(int(sys.argv[1]))

if __name__ == "__main__":
	main(sys.argv)
