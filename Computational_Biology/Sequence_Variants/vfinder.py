import math
import sys

#This program was found via a Google search
def choose(n, k):
    """
    A fast way to calculate binomial coefficients by Andrew Dalke (contrib).
    """
    if 0 <= k <= n:
        ntok = 1
        ktok = 1
        for t in range(1, min(k, n - k) + 1):
            ntok *= n
            ktok *= t
            n -= 1
        return ntok // ktok
    else:
        return 0
    
#This function calculates the log of a sum
def LogSumP_Basic(x, y):
    if x >= y:
        z = y - x
        total = x + math.log10(1 + math.pow(10, z))
    else:
        z = x - y
        total = y + math.log10(1 + math.pow(10, z))
    return total

def LogSumP_Adv(x, y):
    if x >= y:
        z = y - x
        total = x + math.log10(1 + math.pow(10, z))
    else:
        z = x - y
        total = y + math.log10(1 + math.pow(10, z))
    return total

#This function calculates the score from the basic variant scoring model
def LogPTotal_Basic(t, theta):
    sumtotal = 0              #this will be our total
    for i in range(0, len(t)): #loop through all the lines
        u = t[i]
        v = u.split(    )
        a = float(v[2])
        if theta != 0:
            b = math.log10(theta)
        else:
            b = -sys.float_info.max #smallest possible number that can be represented
        c = float(v[1])
        d = math.log10(1 - theta)
        sumtotal += LogSumP_Basic(a + b, c + d)
    return sumtotal

#This function calculaes the score from the advanced variant scoring model
def LogPTotal_Adv(t, theta):
    person_total = 0 #
    total = 1 #
    for j in range(0,20): #there were 20 unique individuals in the 10 files; this could be generalized
                          #for other data files
        term_1 = 1 #we use the following three terms to multiple the three terms (k = 0, 1, 2) found in
                   #each observation for a given person.  I used this approach to ensure order of operations
                   #was correct
        term_2 = 1
        term_3 = 1
        for i in range(0,len(t)): #we look through all the lines for every person; alternatively we could
                                  #sort the file before this step
            u = t[i]
            v = u.split(    ) #we split on tab to access the three relevant fields
            if int(v[0]) == j: #if this line belongs to the person we're looking at we'll use it
                term_1 *= float(v[1])
                term_2 *= (((float(v[1]) / 2) + (float(v[2]) / 2)))
                term_3 *= float(v[2])
        sub_total = term_1 + term_2 + term_3
        temp = 0
        for k in range(0,3): #this caculate the joint probability of all observations for a single person
            temp += math.pow(theta, k) * math.pow(1 - theta, k) * choose(2, k) * sub_total
        person_total += temp
        total *= person_total #the set of observations for each person is independent from all other people
    return(total)

#This function is recursive with nested calls to LogPTotal_Adv
def Q_Funtion(m, k):
    if k == 0:
        return -sys.float_info.max
    return LogSumP_Basic(Q_Funtion(m, k - 1), LogSumP_Basic(math.log10(LogPTotal_Adv(t, (k - 1) / m)), \
                                                         math.log10(LogPTotal_Adv(t, k / m)) + \
                                                         math.log10(1 / (2 * m))))


s = "" #s will be the string that contains all the information from all ten files
for i in range(1,11): #this loop reads in the ten text files and creates a string
                      #to work with
    f = open('seq.{0}.txt'.format(i), 'r')
    s += f.read()
    f.close()

t = s.splitlines() #now we can work with individual lines

basic_score = LogPTotal_Basic(t, 0.2) - LogPTotal_Basic(t, 0)


q_m = Q_Funtion(100, 100)

l_zero = math.log10(LogPTotal_Adv(t, 0))

adv_score = q_m - l_zero

f = open('output.txt', 'w') #we'll write our output to this file

f.write("BASIC_SCORE = " + str(basic_score) + '\n')  #output basic score
f.write("ADV_SCORE = " + str(adv_score) + '\n')    #output advanced score

temp_1 = 0
for i in range(0, 101): #this loop outputs the log posterior probability density table
    l_i = math.log10(LogPTotal_Adv(t, i/100))
    f.write("LOG_P_PHI " + str(i) + " " + str(l_i - q_m) + '\n')
    temp_1 += math.pow(10, l_i - q_m)
    
f.close()
