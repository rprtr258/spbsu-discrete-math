s2 = str(input())
s = [(s2[i], i + 1) for i in range(len(s2))]
print("".join(map(lambda x: str(x[1] % 10), s)), "".join(map(lambda x: x[0], s)), sep='\n')
s.sort()
print("".join(map(lambda x: str(x[1] % 10), s)), "".join(map(lambda x: x[0], s)), sep='\n')
i = s[0][1]
while i != 1:
 print(s[i - 1][0], end = "")
 i = s[i - 1][1]
print(s[i - 1][0], end = "")
