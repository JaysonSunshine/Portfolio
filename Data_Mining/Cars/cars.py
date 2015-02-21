import csv
import matplotlib.pyplot as plt
import numpy as np
import numpy.numarray as na
import pylab

def barGraph(data, error, labels, title):
	xlocations = na.array(range(len(data)))+0.5
	width = 0.5
	pylab.bar(xlocations, data, yerr=error, width=width, ecolor='r') #change color of error bars
	pylab.yticks(range(0, int(1.3 * max([x + y for x, y in zip(data, error)]))))
	pylab.xticks(xlocations+ width/2, labels)
	pylab.xlim(0, xlocations[-1]+width*2)
	pylab.title(title)
	pylab.gca().get_xaxis().tick_bottom()
	pylab.gca().get_yaxis().tick_left()

	pylab.show()

def globalsData(west, midwest, south, northeast):
	with open('Final_Paid_TDB.txt') as f:
		f.readline()
		reader = csv.reader(f)
		global_count = 0
		old_mpg_sum = 0
		new_mpg_sum = 0
		old_mpg = []
		new_mpg = []
		west_arr = []
		west_ratio_arr = []
		midwest_arr = []
		midwest_ratio_arr = []
		south_arr = []
		south_ratio_arr = []
		northeast_arr = []
		northeast_ratio_arr = []
		west_sum = 0; west_total = 0; west_old_sum = 0
		south_sum = 0; south_total = 0; south_old_sum = 0
		northeast_sum = 0; northeast_total = 0; northeast_old_sum = 0
		midwest_sum = 0; midwest_total = 0; midwest_old_sum = 0
		states = {}
		
		for row in reader:
			global_count += 1
			if row[7] in states:
				states[row[7]] = (states[row[7]][0] + float(row[26]), states[row[7]][1] + float(row[40]),states[row[7]][2] + 1)
			else:
				states[row[7]] = (float(row[26]), float(row[40]), 1)
			if row[7] in west:
				west_arr.append(float(row[40]))
				west_sum += float(row[40])
				west_old_sum += float(row[26])
				if float(row[40]) != 0 and float(row[26]) != 0:
					west_ratio_arr.append(float(row[40]) / float(row[26]))
				west_total += 1
			elif row[7] in midwest:
				midwest_arr.append(float(row[40]))
				midwest_sum += float(row[40])
				midwest_old_sum += float(row[26])
				if float(row[40]) != 0 and float(row[26]) != 0:
					midwest_ratio_arr.append(float(row[40]) / float(row[26]))
				midwest_total += 1
			elif row[7] in south:
				south_arr.append(float(row[40]))
				south_sum += float(row[40])
				south_old_sum += float(row[26])
				if float(row[40]) != 0 and float(row[26]) != 0:
					south_ratio_arr.append(float(row[40]) / float(row[26]))
				south_total += 1
			elif row[7] in northeast:
				northeast_arr.append(float(row[40]))
				northeast_sum += float(row[40])
				northeast_old_sum += float(row[26])
				if float(row[40]) != 0 and float(row[26]) != 0:
					northeast_ratio_arr.append(float(row[40]) / float(row[26]))
				northeast_total += 1
			old_mpg.append(float(row[26]))
			new_mpg.append(float(row[40]))
			old_mpg_sum += float(row[26])
			new_mpg_sum += float(row[40])
		old_std = np.std(old_mpg)
		new_std = np.std(new_mpg)
		old_mpg_ave = (float(old_mpg_sum) / float(global_count))
		new_mpg_ave = (float(new_mpg_sum) / float(global_count))

		data = [old_mpg_ave, new_mpg_ave]
		error = [old_std, new_std]
		labels = ["TradeIns", "Purchased"]
		title = "Nationwide average MPG"
		print "Data for Figure 1"
		print ["%.2f" % i for i in data], ["%.2f" % i for i in error], "\n"
		barGraph(data, error, labels, title)

		west_std = np.std(west_arr)
		west_ratio_std = np.std(west_ratio_arr)
		midwest_std = np.std(midwest_arr)
		midwest_ratio_std = np.std(midwest_ratio_arr)
		south_std = np.std(south_arr)
		south_ratio_std = np.std(south_ratio_arr)
		northeast_std = np.std(northeast_arr)
		northeast_ratio_std = np.std(northeast_ratio_arr)

		west_ratio = float(west_sum) / float(west_old_sum)
		west_ave = float(west_sum) / float(west_total)
		midwest_ave = float(midwest_sum) / float(midwest_total)
		midwest_ratio = float(midwest_sum) / float(midwest_old_sum)
		south_ave = float(south_sum) / float(south_total)
		south_ratio = float(south_sum) / float(south_old_sum)
		northeast_ave = float(northeast_sum) / float(northeast_total)
		northeast_ratio = float(northeast_sum) / float(northeast_old_sum)

		region = [(west_ave, 'west'), (midwest_ave, 'midwest'), (south_ave, 'south'), (northeast_ave, 'northeast')]

		data = [west_ave, midwest_ave, south_ave, northeast_ave]
		error = [west_std, midwest_std, south_std, northeast_std]
		labels = ['West', 'Midwest', 'South', 'Northeast']
		title = "Regional new mpg ave"
		print "Data for figure 4"
		print ["%.2f" % i for i in data], ["%.2f" % i for i in error]
		barGraph(data, error, labels, title)
		data = [west_ratio, midwest_ratio, south_ratio, northeast_ratio]
		error = [west_ratio_std, midwest_ratio_std, south_ratio_std, northeast_ratio_std]
		labels = ['West', 'Midwest', 'South', 'Northeast']
		title = "Regional by ratio"
		print "\nData for figure 5"
		print ["%.2f" % i for i in data], ["%.2f" % i for i in error]
		barGraph(data, error, labels, title)
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

def bestWorstStates(sorted_states, nation, choice):
	title = ''
	if choice == 2:
		print "Best states by ratio"
		title = "Best and worst states by ratio"
	else:
		print "Best states by new mpg"
		title = "Best and worst states by new mpg"
	i = 0
	count = 0
	bestStates = []
	worstStates = []
	labels = []
	while count < 10:#sorted_states: could change to exclude regions marked as belonging to other
		if sorted_states[i][0] in nation:
			print count + 1, " = ", sorted_states[i][0], "%.2f" % sorted_states[i][1][choice], sorted_states[i][1][4] #, states_averages[elem][0], states_averages[elem][1]#, states_averages[elem][2]
			bestStates.append(sorted_states[i][1][choice]) #, states_averages[elem][0], states_averages[elem][1]#, states_averages[elem][2]
			labels.append(sorted_states[i][0])
			count += 1
		i += 1
	if choice == 2:
		print "Worst states by ratio"
	else:
		print "Worst states by new mpg"
	i = len(sorted_states) - 1
	count = 0
	while count < 10: #range(len(sorted_states) - 12, len(sorted_states) + 1):
		if sorted_states[i][0] in nation:
			print 50 - count, " = ", sorted_states[i][0], "%.2f" % sorted_states[i][1][choice], sorted_states[i][1][4]
			worstStates.append(sorted_states[i][1][choice])
			labels.append(sorted_states[i][0])
			count += 1
		i -= 1
	data = bestStates + worstStates
	error = [0]*len(data)
	barGraph(data,error, labels, title)

#west, midwest, south, northeast
def regionData(states_averages, regionArray):
	for elem in states_averages:
		if elem in regionArray[0]:
			print elem, states_averages[elem][4]

def statesData(states, west, midwest, south, northeast):
	states_averages = {}
	region = ''
	nation = south.union(northeast.union(west.union(midwest)))
	for elem in states:
		a =  (float(states[elem][0]) / float(states[elem][2]))
		b = (float(states[elem][1]) / float(states[elem][2]))
		if elem in west:
			region = 'west'
		elif elem in midwest:
			region = 'midwest'
		elif elem in south:
			region = 'south'
		elif elem in northeast:
			region = 'northeast'
		else:
			region = 'other'
		states_averages[elem] = (a,b, b / a, states[elem][2], region)
	sorted_states = sorted(states_averages.items(), key=lambda x: -x[1][2])
	new_mpg_sorted_states = sorted(states_averages.items(), key=lambda x: -x[1][1])
	print "\nData for figures 2 and 3"
	bestWorstStates(sorted_states, nation, 2)
	bestWorstStates(new_mpg_sorted_states, nation, 1)

def main():
	west = set()
	midwest = set()
	south = set()
	northeast = set()
	regions(west, midwest, south, northeast)
	states = globalsData(west, midwest, south, northeast)
	statesData(states,west, midwest, south, northeast)

if __name__ == "__main__":
	main()



