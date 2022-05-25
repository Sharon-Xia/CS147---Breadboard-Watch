"""
export FLASK_APP=server.py
python3 -m flask run --host=0.0.0.0
"""

from flask import Flask
from flask import request
from stepCounter import stepCounter

# store data into csvs
# https://stackabuse.com/saving-text-json-and-csv-to-a-file-in-python/ 

app = Flask(__name__)

@app.route("/")
def hello():
	print(request.args.get("var"))
	return "We received value: "+str(request.args.get("var"))