one = 10**1000
two = one * 2
four = two * 2

val = 0
for i in range(0, 1000):
    k = i
    tmp = (four // (8*k+1) - two // (8*k+4) - one // (8*k+5) - one // (8*k+6)) // 16**k
    
    if tmp > 0:
        print(f'{"+" if i > 0 else ""}(4 / (8*{k}+1) - 2 / (8*{k}+4) - 1 / (8*{k}+5) - 1 / (8*{k}+6)) / 16^{k} ', end='')
    else: break