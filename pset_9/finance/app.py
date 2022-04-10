import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get details of user's stocks
    user_id = session["user_id"]
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    stocks = db.execute("SELECT symbol, SUM(shares) as shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", user_id)

    # Iterate through the user's stocks, also keeping track of how much the user has in total of stocks
    total_cash_stocks = 0
    for stock in stocks:
        result = lookup(stock["symbol"])
        stock["name"] = result["name"]
        stock["price"] = float(usd(result["price"]).lstrip("$"))
        stock["total"] = stock["price"] * stock["shares"]
        total_cash_stocks = total_cash_stocks + stock["total"]

    # Return portfolio
    total_cash = total_cash_stocks + cash
    return render_template("index.html", stocks = stocks, cash = cash, total_cash = total_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    user_id = session["user_id"]

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get symbol, price, cash, and user_id
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        result = lookup(symbol)
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Check to see if symbol was typed in
        if symbol == None:

            return apology("please provide a symbol", 400)

        # Check to see if symbol exists
        if result == None:

            return apology("symbol does not exist", 400)

        # Check to see if user submitted non-fraction
        try:
            int(shares)

        except:
            return apology("cannot submit a fraction number of shares", 400)

        # Check to see if user submitted zero or negative number of shares
        if int(shares) <= 0:

            return apology("cannot submit zero or negative number of shares", 400)

        # If no errors, calculate, update user cash and insert the transaction
        price = float(usd(result["price"]).lstrip("$"))
        remainder = cash - price * int(shares)

        # Check to see if user can't afford shares bought
        if remainder < 0:

            return apology("insufficient cash", 400)

        db.execute("UPDATE users SET cash = ? where id = ? ", remainder, user_id)
        db.execute("INSERT INTO transactions (user_id, symbol, type, price, shares, timestamp) VALUES (?, ?, ?, ?, ?, ?)", user_id, symbol, "buy", price, shares, datetime.datetime.now())

        return redirect("/")

    # User reached route via GET (as by clicking a link or redirect)
    return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get details of user's stock history
    user_id = session["user_id"]
    stocks = db.execute("SELECT symbol, type, price, shares, timestamp FROM transactions WHERE user_id = ? GROUP BY timestamp", user_id)

    # Return portfolio
    return render_template("history.html", stocks = stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get symbol submitted and name, symbol, and price
        symbol = request.form.get("symbol")
        result = lookup(symbol)

        # Check to see if symbol was typed in
        if symbol == "":

            return apology("please provide a symbol", 400)

        # Check to see if symbol exists
        if result == None:

            return apology("symbol does not exist", 400)

        symbol = result["symbol"]
        price = float(usd(result["price"]).lstrip("$"))
        name = result["name"]

        # If symbol exists and passes error handling
        return render_template("quoted.html", name = name, symbol = symbol, price = price)

    # User reached route via GET (as by clicking a link or redirect)
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get username, password, and confirmation
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check to see if username or password is blank
        if username == "" or password == "" or confirmation == "":

            return apology("submit a username and a password, then confirm your password", 400)

        # Check to see if password and confirmation don't match
        if password != confirmation:

            return apology("passwords are not the same", 400)

        # Check to see if username is taken
        if len(db.execute('SELECT username FROM users WHERE username = ?', username)) > 0:

            return apology("username is taken", 400)

        # Check to see if password has less than 8 characters
        if len(password) < 8:

            return apology("password must at least be 8 characters", 400)

        # Check to see if password has no numbers by creating a function checkPassword
        def checkPassword(str):

            flag = False

            for i in str:

                if i.isdigit():

                    flag = True

            return flag

        # Checking password

        if checkPassword(password) == False:

            return apology("password must have at least 1 number", 403)


        # As long as error was not caught above, register user. first need to generate a hash of the password
        hashed_password = generate_password_hash(password)
        db.execute("INSERT into users (username, hash) VALUES (?, ?)", username, hashed_password)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Log user in and remember them
        session["user_id"] = rows[0]["id"]

        # Finally, redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session["user_id"]

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get details of form submission and the stocks the user has
        symbol_input = request.form.get("symbol")
        shares_input = int(request.form.get("shares"))
        result = lookup(symbol_input)
        price = float(usd(result["price"]).lstrip("$"))
        starting_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        shares = int(db.execute("SELECT shares FROM transactions WHERE user_id = ? AND symbol = ?", user_id, symbol_input)[0]["shares"])

        # Check if user didn't select a symbol
        if symbol_input == "":

            return apology("select a symbol from your portfolio", 400)

        # Check if user didn't put a number of shares
        if shares_input == "":

            return apology("input a number of shares to sell", 400)

        # Check if user is trying to sell more shares than they own
        if shares_input > shares:

            return apology("you don't have enough shares", 400)

        # Check if user is trying to sell zero or negative shares
        if shares_input <= 0:

            return apology("input a positive number of shares")

        # If no errors, update user cash and insert the transaction
        sold_stocks = shares_input * price
        new_cash = starting_cash + sold_stocks

        # Update amount of cash
        db.execute("UPDATE users SET cash = ? where id = ? ", new_cash, user_id)

        # Record transaction, multipplying by -1 to get rid of the shares
        db.execute("INSERT INTO transactions (user_id, symbol, type, price, shares, timestamp) VALUES (?, ?, ?, ?, ?, ?)", user_id, symbol_input, "sell", price, shares_input * -1, datetime.datetime.now())

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)

    # Get details of user's stocks
    symbol = db.execute("SELECT symbol FROM transactions WHERE user_id = ?", user_id)

    return render_template("sell.html", symbol = symbol)

