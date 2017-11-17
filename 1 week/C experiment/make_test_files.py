from random import randint
for i in [4,5,6,7]:
    s = str(i)
    n = 10**i
    fil = open("test_"+s+'.txt', 'w')
    for i in range(n):
        print(randint(1, 10000), '.', randint(1, 10000), sep = '', end = ' ', file = fil)
    fil.close()