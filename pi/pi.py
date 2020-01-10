from random import random
from math import sqrt
import time

if __name__ == '__main__':
    DARTS = 1200
    hits = 0
    start = time.time()
    for i in range(1, DARTS):
        x, y = random(), random()
        dist = sqrt(x**2 + y**2)
        if dist <= 1.0:
            hits = hits + 1
    pi = 4 * (hits/DARTS)
    time.sleep(2)
    print('Pi的值是 %s' % pi)
    print('程序的运行时间是 %-5.5ss' % str(time.time()-start))

