import sys
import re

def wordsplit(text):
	f = open(text, 'r')
	for line in f:
		a = re.split('\s|\[|\"|\.|\,|\:|\(|\)|\!|--|\?|\]|;|_|\/|#|&', line)
		for word in a:
			if word != '':
				print word

def main(argv):
	wordsplit(sys.argv[1])

if __name__ == '__main__':
	main(sys.argv)