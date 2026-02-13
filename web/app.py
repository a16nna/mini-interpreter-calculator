web/app.py
from flask import Flask, request, jsonify, render_template
import subprocess
import os

app = Flask(__name__)

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
CALC_PATH = os.path.join(BASE_DIR, "calc.exe")

@app.route("/")
def home():
    return render_template("index.html")

@app.route("/evaluate", methods=["POST"])
def evaluate():
    expression = request.json["expression"]

    try:
        result = subprocess.check_output(
            [CALC_PATH, expression],
            text=True
        ).strip()

        return jsonify({"result": result})

    except:
        return jsonify({"result": "Error"})
