import sys

def wordsplit(text):
	f = open(text, 'r')
	for line in f:
		a = line.split()
		for word in a:
			print word

def main(argv):
	wordsplit(sys.argv[1])

if __name__ == '__main__':
	main(sys.argv)