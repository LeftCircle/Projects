import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True

formA = []

@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():

    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    nMoS = request.form.get("nMoS")
    nchar = request.form.get("nchar")
    broken = 0
    errorMessage = ""
    if not nMoS:
        errorMessage += " --input the name of a movie or a show-- \n"
        broken = 1
    if not nchar:
        errorMessage += " --input the name of a character-- \n"
        broken = 1
    if broken == 1:
        return render_template("error.html", message = errorMessage )

    if broken == 0:
        # Determine if movie or show is checked
        file = open("survey.csv", "a")
        writer = csv.writer(file)
        writer.writerow((request.form['eType'],
            request.form.get("nMoS"), request.form.get("nchar"),
            request.form.get("coolness")))
        file.close()
    return render_template("form.html")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    file = open("survey.csv", "r")
    reader = csv.reader(file)
    data = list(reader)
    for row in reader:
        formA.append(row)

    return render_template("sheet.html", formA=formA)
