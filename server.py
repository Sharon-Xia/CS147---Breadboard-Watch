"""
#run this before executing:
#ssh  -X -i "C:/Users/longl/OneDrive/lab3_cs147.pem" ubuntu@ec2-3-219-247-146.compute-1.amazonaws.com


open XLaunch
start server from putty

cd cs147_project
export MPLBACKEND="agg"
export FLASK_APP=server.py
chmod +x data/
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
#matplotlib.use('TkAgg')

import io
import matplotlib.pyplot as plt,mpld3
import matplotlib.animation as animation
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from matplotlib.figure import Figure
from flask import Response

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




# *** ANIMATED GRAPH ***

fig, axs = plt.subplots(2, 1)
fig.suptitle("Step Counts and Heartrate Graphs for User")
ax1 = axs[0]
ax2 = axs[1]

#Step Counts Graph
# ax1 = plt.subplot(2,2,1)
plt.gcf().autofmt_xdate()
ax1.tick_params(rotation=90)

#Heart Rate Graph
# ax2 = plt.subplot(2,2,2)
plt.gcf().autofmt_xdate()
ax2.tick_params(rotation=90)

plt.subplots_adjust(hspace=1.4)


def animate(i, user='default'):

	days, steps = getDataFromFile(user, 'steps')
	currentDay = watch.getStepCounter().getCurrentDay()
	days.append(currentDay)
	steps.append(watch.getStepCounter().getSteps(currentDay))

	ax1.clear()
	ax1.plot(days, steps)
	ax1.set_title("Step Counts")
	ax1.set_xlabel("time")
	ax1.set_ylabel("steps")

	time, bpm = getDataFromFile(user, 'bpm')
	t, b = watch.getBpmTracker().getCurrentBpm()
	if t != None:
		time.append(t)
		bpm.append(b)

	ax2.clear()
	ax2.plot(time, bpm)
	ax2.set_title("Heart Rate Graph Test")
	ax2.set_xlabel("time")
	ax2.set_ylabel("bpm")  
	
	return 0

ani = animation.FuncAnimation(fig, animate, interval=1000)


# FLASK 
data = 'foo'
app = Flask(__name__)

@app.route("/")
def hello():
	steps = (int)(request.args.get("steps"))

	bpm = (int)(request.args.get("bpm"))

	watch.updateData(steps, bpm)

	return "We received value: steps="+str(request.args.get("steps")) \
		+ "bpm=" + str(bpm)


@app.route('/plot.png')
def chartTest():
	animate(1000)
	output = io.BytesIO()
	FigureCanvas(fig).print_png(output)
	return Response(output.getvalue(), mimetype='dummy_name.png')

	#return render_template('untitled1.html', name = 'new_plot', url ='dummy_name.png')

if __name__ == "__main__":
	thread.start_new_thread(lambda: app.run())

# y = threading.Thread(target=animate, args=(1,))
# y.start()

#This starts a local server that contains the graphs
plt.savefig("dummy_name.png")

# http://127.0.0.1:5000/plot.png