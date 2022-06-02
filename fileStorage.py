import os
from datetime import datetime

# Folder Path
path = "data/"
  
# Change the directory
os.chdir(path)

def writeDataToFile(user, dataType, x, y):
	file1 = open(user + '_' + dataType + '.txt', "a")
	file1.write(str(x) + "," + str(y) + "\n")
	file1.close()


dateFormat = {"steps": "%Y-%m-%d %H:%M:%S", "bpm": "%Y-%m-%d %H:%M:%S"}

def getDataFromFile(user, dataType):

	graph_data = open(user + '_' + dataType + '.txt', 'r').read()
	lines = graph_data.split('\n')
	xs = []
	ys = []

	for line in lines:
		if len(line) > 1:
			x, y = line.split(',')
			xs.append(datetime.strptime(x, dateFormat[dataType]))
			ys.append(int(y))

	return xs, ys