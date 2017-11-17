import random
fil = open('input.txt', 'w')
fer = random.randint(2, 5* 100000)
print(random.randint(10, 10**fer), file = fil)
fil.close()