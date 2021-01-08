from flask import Flask, redirect, url_for, render_template, request
import parameterpasser

app = Flask(__name__)

@app.route("/home")
@app.route("/")
def home():
	return render_template("index.html")


@app.route("/mymug", methods=["POST", "GET"])

def login():
	if request.method == "POST":
		name = request.form["username"]
		spending = request.form["maxSpendingLimit"]
		timeframe = request.form["timeframe"]

		parameterpasser.main(name, spending, timeframe)

		return render_template("mug.html")
	else:
		return render_template("mug.html")




if __name__ == "__main__":
	app.run(debug=True)
