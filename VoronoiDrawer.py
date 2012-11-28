# -*- encoding: utf-8 -*- 
from pylab import scatter, plot, show
import numpy

def draw_edges():
	count = int(raw_input())
	while count > 0:
		x1, y1, x2, y2 = map(raw_input().split())
			
		x = numpy.linspace(lowx, highx, 100) 
		y = g(x) * alpha 
		plot(x, y, linewidth=1.0)
		
		count -= 1
	
	try:
		x1 = .3 # X a ser testado com a g(x) * α
		g = lambda x: pow(x, 2)
		lowx, highx, accx, accfx = x1, x1, 0.0, 0.0
		while True:
			x, fx = map(float, raw_input().split())
			scatter(x, fx, marker='o', c='r')
			lowx, highx = min(lowx, x), max(highx, x)
			accx += pow(g(x), 2)
			accfx += fx * g(x)
	except EOFError:
		pass
	alpha = accfx / accx
	print 'α =', alpha
	x = numpy.linspace(lowx, highx, 100) 
	y = g(x) * alpha 
	plot(x, y, linewidth=1.0)
	scatter(x1, g(x1) * alpha, marker='o', c='r')
	show()

if __name__ == '__main__':
	quadrados_minimos()
 
