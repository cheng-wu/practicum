datafile = open('file.txt', 'r')

slowdown = []
for line in datafile:
	words = line.split()
	slowdown.append(words[5])

slowdown.sort()

file1 = open("cdf1.txt", "w")

for i in  range(0,len(slowdown)):
	cdf = float((i+1))/(len(slowdown)+1)
	file1.write(slowdown[i] + "\t" +str(cdf) + "\n")
file1.close()

file2 = open("cdf2.txt", "w")

for i in  range(0,len(slowdown)):
	cdf = float((i+1))/(len(slowdown)+1)
	if(cdf >= 0.95):
		file2.write(slowdown[i] + "\t" +str(cdf) + "\n")
file2.close()