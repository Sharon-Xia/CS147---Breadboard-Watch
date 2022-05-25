"""
run this before executing:
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
	print(request.args.get("steps"))
	return "We received value: "+str(request.args.get("steps"))


# load past user data into server from files

# after receiving data, update in files 