import csv
import matplotlib.pyplot as plt
import numpy as np
import numpy.numarray as na
import pylab

def barGraph(old, new, old_std, new_std):
	labels = ["TradeIns", "Purchased"]
	data = [old, new]
	error = [old_std, new_std]

	xlocations = na.array(range(len(data)))+0.5
	width = 0.5
	pylab.bar(xlocations, data, yerr=error, width=width) #change color of error bars
	pylab.yticks(range(0, int(1.3 * max(old + old_std, new + new_std))))
	pylab.xticks(xlocations+ width/2, labels)
	pylab.xlim(0, xlocations[-1]+width*2)
	pylab.title("Nationwide average MPG")
	pylab.gca().get_xaxis().tick_bottom()
	pylab.gca().get_yaxis().tick_left()

	pylab.show()

def globalsData():
	with open('Final_Paid_TDB.txt') as f:
		f.readline()
		reader = csv.reader(f)
		global_count = 0
		old_mpg_sum = 0
		new_mpg_sum = 0
		old_mpg = []
		new_mpg = []
		states = {}
		for row in reader:
			global_count += 1
			if row[7] in states:
				states[row[7]] = (states[row[7]][0] + float(row[26]), states[row[7]][1] + float(row[40]),states[row[7]][2] + 1)
			else:
				states[row[7]] = (float(row[26]), float(row[40]), 1)
			old_mpg.append(float(row[26]))
			new_mpg.append(float(row[40]))
			old_mpg_sum += float(row[26])
			new_mpg_sum += float(row[40])
			#print row[7], "old mpg =", row[26], "new mpg =", row[40]
		#b = float()
		#b = old_mpg // global_count
		old_std = np.std(old_mpg)
		new_std = np.std(new_mpg)
		old_mpg_ave = (float(old_mpg_sum) / float(global_count))
		new_mpg_ave = (float(new_mpg_sum) / float(global_count))
		#print "average old mpg = ", "%.2f" % old_mpg_ave, "old std =", old_std, "average new mpg =", "%.2f" % new_mpg_ave, "new std =", new_std
		#print global_count
		#barGraph(old_mpg_ave, new_mpg_ave, old_std, new_std)
		return states

def regions(west, midwest, south, northeast):
	west.add('WA')
	west.add('OR')
	west.add('CA')
	west.add('NV')
	west.add('ID')
	west.add('MT')
	west.add('WY')
	west.add('CO')
	west.add('NM')
	west.add('AZ')
	west.add('HI')
	west.add('AK')
	west.add('UT')
	midwest.add('ND')
	midwest.add('SD')
	midwest.add('NE')
	midwest.add('KS')
	midwest.add('MN')
	midwest.add('IA')
	midwest.add('MO')
	midwest.add('WI')
	midwest.add('IL')
	midwest.add('IN')
	midwest.add('MI')
	midwest.add('OH')
	south.add('TX')
	south.add('OK')
	south.add('AR')
	south.add('LA')
	south.add('MS')
	south.add('AL')
	south.add('TN')
	south.add('KY')
	south.add('WV')
	south.add('MD')
	south.add('DE')
	south.add('VA')
	south.add('NC')
	south.add('SC')
	south.add('GA')
	south.add('FL')
	northeast.add('PA')
	northeast.add('NY')
	northeast.add('NJ')
	northeast.add('CT')
	northeast.add('RI')
	northeast.add('MA')
	northeast.add('VT')
	northeast.add('NH')
	northeast.add('ME')

def statesData(states):
	states_averages = {}
	#count = 0
	for elem in states:
		#print count, elem, float(states[elem][0]) / float(states[elem][2]), float(states[elem][1]) / float(states[elem][2])
		a =  (float(states[elem][0]) / float(states[elem][2]))
		#count += 1
		#print "TYPE", type(a)
		b = (float(states[elem][1]) / float(states[elem][2]))
		states_averages[elem] = (a,b, b / a, states[elem][2])
	#for elem in states_averages:
		#print elem, states_averages[elem][0], states_averages[elem][1], states_averages[elem][2]
	sorted_states = sorted(states_averages.items(), key=lambda x: -x[1][2])
	west = set()
	midwest = set()
	south = set()
	northeast = set()
	regions(west, midwest, south, northeast)
	count = 0
	region = ''
	for elem in sorted_states:
		if elem[0] in west:
			region = 'west'
		elif elem[0] in midwest:
			region = 'midwest'
		elif elem[0] in south:
			region = 'south'
		elif elem[0] in northeast:
			region = 'northeast'
		else:
			region = 'other'
		print count, elem[0], region, elem[1][2], elem[1][3]
		count += 1
	#print "Best states"
	#for i in range(0,10):#sorted_states:
		#print i + 1, " = ", sorted_states[i] #, states_averages[elem][0], states_averages[elem][1]#, states_averages[elem][2]
	#print "Worst states"
	#for i in range(len(sorted_states) - 12, len(sorted_states) + 1):
		#print i, " = ", sorted_states[i - 1]
	#barGraph()

def main():
	states = globalsData()
	statesData(states)

if __name__ == "__main__":
	main()



