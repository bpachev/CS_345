l = [1, 5, 41, 205, 25343, 126715, 237631, 1039063, 1188155, 5195315, 9742871, 48714355, 6022282433, 30111412165, 246913579753, 1234567898765]
n = 1234567898765
k = 4321
# k = 29 * 149
m_l = [j % k for j in l]
m_l1 = [j % 29 for j in l]
print m_l1

def partitionfunc(n,k,l=1):
    '''n is the integer to partition, k is the length of partitions, l is the min partition element size'''
    if k < 1:
        raise StopIteration
    if k == 1:
        if n >= l:
            yield (n,)
        raise StopIteration
    for i in range(l,n+1):
        for result in partitionfunc(n-i,k-1,i):
            yield (i,)+result
            
for p in partitionfunc(28,16):
  print p
  

