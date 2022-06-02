"""
run this before executing:
ssh  -X -i "C:/Users/longl/OneDrive/lab3_cs147.pem" ubuntu@ec2-3-219-247-146.compute-1.amazonaws.com

cd cs147_project
export MPLBACKEND="agg"
export FLASK_APP=server.py
python3 -m flask run --host=0.0.0.0


"""

""" pip files
pycairo
matplotlib
mpld3
"""

from flask import Flask
from flask import request
from watchData import watchData

import matplotlib
matplotlib.use('Agg')

import matplotlib.pyplot as plt,mpld3
import matplotlib.animation as animation
from stepCounter import stepCounter
from matplotlib import style
from fileStorage import writeDataToFile, getDataFromFile


import logging
import threading
import time


# plt.switch_backend('agg')


# store data into csvs
# https://stackabuse.com/saving-text-json-and-csv-to-a-file-in-python/ 


watch = watchData()


app = Flask(__name__)



@app.route("/")
def hello():
	steps = (int)(request.args.get("steps"))

	bpm = (int)(request.args.get("bpm"))

	watch.updateData(steps, bpm)

	return "We received value: steps="+str(request.args.get("steps")) \
		+ "bpm=" + str(bpm)



# *** ANIMATED GRAPH ***

fig, axs = plt.subplots(2, 1)
ax1 = axs[0]
ax2 = axs[1]

#Step Counts Graph
# ax1 = plt.subplot(2,2,1)
ax1.set_title("Step Counts")
ax1.set_xlabel("day")
ax1.set_ylabel("steps")
plt.gcf().autofmt_xdate()
ax1.tick_params(rotation=90)

#Heart Rate Graph
# ax2 = plt.subplot(2,2,2)
ax2.set_title("Heart Rate Graph Test")
ax2.set_xlabel("time")
ax2.set_ylabel("bpm")  
plt.gcf().autofmt_xdate()
ax2.tick_params(rotation=90)

plt.subplots_adjust(hspace=0.75)


def animate(i, user='default'):

	days, steps = getDataFromFile(user, 'steps')
	currentDay = watch.getStepCounter().getCurrentDay()
	days.append(currentDay)
	steps.append(watch.getStepCounter().getSteps(currentDay))

	ax1.clear()
	ax1.plot(days, steps)

	time, bpm = getDataFromFile(user, 'bpm')
	ax2.clear()
	ax2.plot(time, bpm)
	t, b = watch.getBpmTracker().getCurrentBpm()
	if t != None:
		time.append(t)
		bpm.append(b)
	
	return 0

ani = animation.FuncAnimation(fig, animate, interval=1000)

# y = threading.Thread(target=animate, args=(1,))
# y.start()

#This starts a local server that contains the graphs
plt.show()

# def updateGraph():
# 	while(1):
# 		plt.savefig("dummy_name.png")
# 		time.sleep(1)

# x = threading.Thread(target=updateGraph)
# x.start()


# while(1):
# 	plt.savefig("dummy_name.png")
