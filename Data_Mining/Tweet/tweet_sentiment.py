import sys
import json

def lines(fp):
    filelength = len(fp.readlines())
    print str(filelength)
    return filelength

def sentiment():
	afinnfile = open("AFINN-111.txt")
	scores = {} # initialize an empty dictionary
	for line in afinnfile:
	  term, score  = line.split("\t")  # The file is tab-delimited. "\t" means "tab character"
	  scores[term] = int(score)  # Convert the score to an integer.
	return scores
	#print scores.items() # Print every (term, score) pair in the dictionary

def convert(fp, scores):
	#print fp.readlines()
	for line in fp:
		sentiment = 0
		d = json.loads(line)
		text = ''
		if "text" in d:
			text = d["text"].encode('utf-8')
		for word in text.split():
			if word in scores:
				sentiment += scores[word]
		if sentiment != 0:
			print str(sentiment) + ' ' + text 
		#print text

def main():
    sent_file = open(sys.argv[1])
    tweet_file = open(sys.argv[2])
    scores = sentiment()
    #lines(tweet_file)
    convert(tweet_file, scores)
    #lines(sent_file)
    


if __name__ == '__main__':
    main()
