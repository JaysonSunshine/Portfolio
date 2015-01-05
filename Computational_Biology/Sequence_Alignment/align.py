#!/usr/bin/python3.2

from math import *
import sys, re

GAP='-'

# Assumes only two fasta entries
def read_fasta(fasta):
    a=''
    a_name=''
    b=''
    b_name=''
    state=0
    for line in open(fasta, 'r'):
        line = line.rstrip('\n')
        line = line.rstrip('\r')
        if 0 == state:
            if None == re.search('^>', line):
                print('error: not valid fasta format')
            a_name = line[1:]
            state = state + 1
        elif 1 == state:
            if None == re.search('^>', line):
                a = a + line
            else:
                state = state + 1
                b_name = line[1:]
        else:
            b = b + line
    return (a,a_name,b,b_name)

def read_scoring(scoring):
    state = 0
    header = []
    sm = {}
    for line in open(scoring, 'r'):
        line = line.rstrip('\n')
        line = line.rstrip('\r')
        if None == re.search('^#', line):
            a = line.split(' ')
            if 0 == state:
                header = a
                state = state + 1
            else:
                if len(a) != 1 + len(header):
                    print('error: the number of entries did not match the header')
                    sys.exit(2)
                for i in range(len(header)):
                    sm[a[0] + header[i]] = int(a[i+1])
   
    return sm

def align(a,b,sm,scale):
    n=len(a) # rows
    m=len(b) # cols

    viterbi = [[0 for x in range(m + 1)] for x in range(n + 1)]
    for i in range(n + 1):
        for j in range(m + 1):
            viterbi[i][j] = -9999999999999999 #each cell is initalized to a sufficiently negative number so we can keep track as we update through the Viterbi algorithm (for the ret_matrix)
    ret_matrix = [[0 for x in range(m + 1)] for x in range(n + 1)]
   
    viterbi[n][0] = 0
    #global score
    if scale == 'global':
        for i in reversed(range(n + 1)):
            for j in range(m + 1):
                #up one cell
                if i - 1 > -1 and viterbi[i - 1][j] < viterbi[i][j] - 5:
                     viterbi[i - 1][j] = viterbi[i][j] - 5
                     ret_matrix[i - 1][j] = 2
                #right one cell
                if j < m and viterbi[i][j + 1] < viterbi[i][j] - 5:
                     viterbi[i][j + 1] = viterbi[i][j] - 5
                     ret_matrix[i][j + 1] = 0
                if i > 0 and j < m:
                    string = a[n - i] + b[j]
                #diagonal up one cell
                if i - 1 > -1 and j < m and viterbi[i - 1][j + 1] < viterbi[i][j] + sm[string]:
                    viterbi[i - 1][j + 1] = viterbi[i][j] + sm[string]
                    ret_matrix[i - 1][j + 1] = 1
    #local score: starts at (n,0) and calculates viterbi score for each cell under local alignment conditions, Pevzner 6.8
    if scale == 'local':
        ret_matrix[n][0] = 3 #3 represents the end of the sequence
        for i in reversed(range(n + 1)):
            for j in range(m + 1):
                #up one cell
                if i - 1 > -1 and viterbi[i - 1][j] < viterbi[i][j] - 5 and viterbi[i][j] - 5 > -1:
                     viterbi[i - 1][j] = viterbi[i][j] - 5
                     ret_matrix[i - 1][j] = 2 #2 represents a vertical move
                if i - 1 > -1 and viterbi[i - 1][j] < viterbi[i][j] - 5 and viterbi[i][j] - 5 < 0:
                     viterbi[i - 1][j] = 0
                     ret_matrix[i - 1][j] = 3 #3 for jumping from origin
                #right one cell
                if j < m and viterbi[i][j + 1] < viterbi[i][j] - 5 and viterbi[i][j] - 5 > -1:
                     viterbi[i][j + 1] = viterbi[i][j] - 5
                     ret_matrix[i][j + 1] = 0 #0 represents a horizontal move
                if j < m and viterbi[i][j + 1] < viterbi[i][j] - 5 and viterbi[i][j] - 5 < 0:
                     viterbi[i][j + 1] = 0
                     ret_matrix[i][j + 1] = 3
                if i > 0 and j < m:
                    string = a[n - i] + b[j]
                #diagonal up one cell
                if i - 1 > -1 and j < m and viterbi[i - 1][j + 1] < viterbi[i][j] + sm[string] and viterbi[i][j] + sm[string] > -1:
                    viterbi[i - 1][j + 1] = viterbi[i][j] + sm[string]
                    ret_matrix[i - 1][j + 1] = 1 #1 represents a diagonal move
                if i - 1 > -1 and j < m and viterbi[i - 1][j + 1] < viterbi[i][j] + sm[string] and viterbi[i][j] + sm[string] < 0:
                    viterbi[i - 1][j + 1] = 0
                    ret_matrix[i - 1][j + 1] = 3

    #finds highest scoring cell in viterbi
    local_score = 0
    loc_x = 0
    loc_y = 0
    for i in range(n + 1):
            for j in range(m + 1):
                if viterbi[i][j] > local_score:
                    local_score = viterbi[i][j]
                    loc_x = i
                    loc_y = j
    x = ''
    y = ''
    i = 0
    j = m

    #global: starts at cell (0,m) in ret_matrix and backtraces to origin
    if scale == 'global':
        while i != n or j != 0:
            if ret_matrix[i][j] == 0:
                y = b[j - 1] + y
                j = j - 1
                x = GAP + x
            elif ret_matrix[i][j] == 1:
                y = b[j - 1] + y
                j = j - 1
                x = a[n - 1 - i] + x
                i = i + 1
            elif ret_matrix[i][j] == 2:
                y = GAP + y
                x = a[n - 1 - i] + x
                i = i + 1

    #local: starts at the cell in ret_matrix that corresponds to the 	 highest scoring cell in viterbi as calculated under local alignment conditions; continues until it reaches the origin (local alignment = global alignment) or until we jumped to the end cell from the origin according to Jones and Pevzner 6.8
    i = loc_x
    j = loc_y
    if scale == 'local':
        while ret_matrix[i][j] != 3:
            if ret_matrix[i][j] == 0:
                y = b[j - 1] + y
                j = j - 1
                x = GAP + x
            elif ret_matrix[i][j] == 1:
                y = b[j - 1] + y
                j = j - 1
                x = a[n - 1 - i] + x
                i = i + 1
            elif ret_matrix[i][j] == 2:
                y = GAP + y
                x = a[n - 1 - i] + x
                i = i + 1 

    if scale == 'global':
        return (x,y,viterbi[0][m])
    return(x,y,local_score)

def main():
    # get arguments
    if 3 != len(sys.argv):
        print('error: two input files required')
        sys.exit(2)
    fasta = sys.argv[1]
    scoring = sys.argv[2]

    # read in input files
    (a,a_name,b,b_name)=read_fasta(fasta)
    (sm)=read_scoring(scoring)
    scale = 'global'
    (ag,bg,sg)=align(a,b,sm, scale)
    scale = 'local'
    (al,bl,sl)=align(a,b,sm, scale)

    # print output
    print("#GLOBAL SCORE=", sg, sep='')
    print(a_name,ag)
    print(b_name,bg)
    print("#LOCAL SCORE=", sl, sep='')
    print(a_name,al)
    print(b_name,bl)

if __name__ == '__main__':
    main()
