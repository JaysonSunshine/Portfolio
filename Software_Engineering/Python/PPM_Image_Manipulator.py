# Name: Jay Cloyd
#Sources I used: Google, StackOverflow for some function usage for Python, Python manual

import math
from struct import *

# parse the file named fname into a dictionary of the form 
# {'width': int, 'height' : int, 'max' : int, 'pixels' : (int * int * int) list}
def parsePPM(fname):
    d = {}
    l = []
    i = 1
    f = open(fname, 'rb')
    for line in f:
        if i == 2:
            d['width'] = int(line.split()[0])
            d['height'] = int(line.split()[1])
        if i == 3:
            d['max'] = int(line.split()[0])
        if i > 3:
            l.append(unpack('%dB' %len(line), line))
        i += 1
    pixels = map(list, l)
    flat_pixels = [y for x in pixels for y in x]
    it = iter(flat_pixels)
    d['pixels'] = zip(it, it, it)
    return d

# a test to make sure you have the right format for your dictionaries
def testParsePPM():
    return parsePPM("example.ppm") == {'width': 2, 'max': 255, 'pixels': [(10, 23, 52), (82, 3, 215), (30, 181, 101), (33, 45, 205), (40, 68, 92), (111, 76, 1)], 'height': 3}

# write the given ppm dictionary as a PPM image file named fname
# the function should not return anything
def unparsePPM(ppm, fname):
    f = open(fname, 'w')
    f.write('P6\n' + str(ppm['width']) + ' ' + str(ppm['height']) + '\n' + str(ppm['max']) + '\n')
    #pixels = [pack('BBB', dList[0], dList[1], dList[2]) for dList in sum(map(list, ppm['pixels']), [])]
    dlist = map(list, ppm['pixels'])
    flat_dlist = [y for x in dlist for y in x]
    for i in range(0, len(flat_dlist)):
        f.write(pack('B', flat_dlist[i]))

def negate(ppm):
    l = []
    for i in ppm['pixels']:
        i = tuple(ppm['max'] - x for x in i)
        l.append(i)
    ppm['pixels'] = l
    return ppm

def mirrorImage(ppm):
    d = ppm
    width = d['width']
    length = len(d['pixels'])
    for i in range(0,width/2):
        swap = d['pixels'][i:length:width]
        #print range(i,length,width)
        #print range((width - i - 1), length, width)
        #print swap
        d['pixels'][i:length:width] = d['pixels'][(width - i - 1):length:width]
        d['pixels'][(width - i - 1):length:width] = swap
    return d

# produce a greyscale version of the given ppm dictionary.
# the resulting dictionary should have the same format, 
# except it will only have a single value for each pixel, 
# rather than an RGB triple.
def greyscale(ppm):
    l = []
    d = {}
    for i in ppm['pixels']:
        l.append(int(round(.299 * i[0] + .587 * i[1] + .114 * i[2])))
    d['height'] = ppm['height']
    d['width'] = ppm['width']
    d['max'] = ppm['max']
    d['pixels'] = l
    return d

# take a dictionary produced by the greyscale function and write it as a PGM image file named fname
# the function should not return anything
def unparsePGM(pgm, fname):
    f = open(fname, 'w')
    f.write('P5\n' + str(pgm['width']) + ' ' + str(pgm['height']) + '\n' + str(pgm['max']) + '\n')
    #dlist = map(list, pgm['pixels'])
    #print dlist
    #flat_dlist = [y for x in dlist for y in x]
    for i in range(0, len(pgm['pixels'])):
        f.write(pack('B', pgm['pixels'][i]))

# gaussian blur code adapted from:
# http://stackoverflow.com/questions/8204645/implementing-gaussian-blur-how-to-calculate-convolution-matrix-kernel
def gaussian(x, mu, sigma):
  return math.exp( -(((x-mu)/(sigma))**2)/2.0 )

def gaussianFilter(radius, sigma):
    # compute the actual kernel elements
    hkernel = [gaussian(x, radius, sigma) for x in range(2*radius+1)]
    vkernel = [x for x in hkernel]
    kernel2d = [[xh*xv for xh in hkernel] for xv in vkernel]

    # normalize the kernel elements
    kernelsum = sum([sum(row) for row in kernel2d])
    kernel2d = [[x/kernelsum for x in row] for row in kernel2d]
    return kernel2d

# blur a given ppm dictionary, returning a new dictionary  
# the blurring uses a gaussian filter produced by the above function
def gaussianBlur(ppm, radius, sigma):
    # obtain the filter
    d = {}
    l = []
    lower = 0
    upperheight = ppm['height'] - 1
    upperwidth = ppm['width'] - 1
    gfilter = gaussianFilter(radius, sigma)
    for i in range(0,ppm['height']):
        for j in range(0, ppm['width']):
            r = 0
            g = 0
            b = 0
            for x in range(0, 2 * radius + 1):
                for y in range(0, 2 * radius + 1):
                    if (i - (radius - x)) < 0:
                        height = 0
                    elif (i - (radius - x)) > (ppm['height'] - 1):
                        height = (ppm['height'] - 1)
                    else:
                        height = i - (radius - x)
                    if (j - (radius - y)) < 0:
                        width = 0
                    elif (j - (radius - y)) > (ppm['width'] - 1):
                        width = ppm['width'] - 1
                    else:
                        width = j - (radius - y)
                    r += gfilter[x][y] * ppm['pixels'][height * ppm['width'] + width][0]
                    g += gfilter[x][y] * ppm['pixels'][height * ppm['width'] + width][1]
                    b += gfilter[x][y] * ppm['pixels'][height * ppm['width'] + width][2]
            l.append((int(round(r)),int(round(g)),int(round(b))))
    d['height'] = ppm['height']
    d['width'] = ppm['width']
    d['max'] = ppm['max']
    d['pixels'] = l
    return d