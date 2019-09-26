import os
import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from helpers import User

user = User()

now = datetime.datetime.now()

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    """Show portfolio of stocks"""
    # Start off by pulling the users history.
    hist = db.execute('SELECT * FROM history WHERE id = :id',
                      id = user.get_user())
    print(hist)
    shares = []
    symbols = []
    for i in range(len(hist)):
        shares.append(hist[i]['shares'])
        symbols.append(hist[i]['symbol'])

    # Determine the current cost of the stocks
    cost_stock = []
    for i in range(len(symbols)):
        quote = lookup(symbols[i])
        name = quote.get('name')
        price = quote.get('price')
        symbol = quote.get('symbol')
        cost_stock.append(price)

    total_stock_value = 0
    for i in range(len(shares)):
        total_stock_value += float(shares[i]) * float(cost_stock[i])

    your_money = db.execute('SELECT cash FROM users WHERE id = :id',
                            id = user.get_user())


    return render_template('index.html', symbols=symbols, shares=shares, cost_stock=cost_stock,
                            total_stock_value=total_stock_value, cash=your_money)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock | only keeps track of time of last purchase"""
    if request.method == "POST":


        """Get stock quote."""
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("input symbol")
        shares = request.form.get("shares")
        if not shares:
            return apology("missing shares")

        #look up price of symbol
        quote = lookup(symbol)
        if not quote:
            return apology('Not a real stock')
        name = quote.get('name')
        price = quote.get('price')
        symbol = quote.get('symbol')

        price = float(shares) * float(price)

        # Can the user afford it?
        update_string = "SELECT cash FROM users WHERE id = {}".format(user.get_user())
        cash = db.execute(update_string)
        print('current user = ', user.get_user())
        print('variable cash = ', cash)
        cash = cash[0]['cash']
        print(cash, type(cash))
        if cash < price:
            return apology("you need more money to do this")

        # Add stock to users update the users money
        money_left = cash - price
        update_string = "UPDATE users SET cash = {} WHERE id = {}".format(money_left, user.get_user())
        db.execute(update_string)

        date = str(now)
        # Add stocks and time to history
        # First see if the user has bought the stock
        print('symbol = ', symbol)
        print('dictionary = ', db.execute("SELECT symbol FROM history"))
        list = db.execute("SELECT symbol FROM history WHERE id = :id",
                          id = user.get_user())
        found = False
        for i in range(len(list)):
            if list[i]['symbol'] == symbol:
                found = True
        if found == False:
            db.execute("INSERT INTO history (id, symbol, shares, date) VALUES(:id, :symbol, :shares, :date)",
                                      id=user.get_user(), symbol=symbol, shares=shares, date=date)

        else:
            print('shares should be ', db.execute("SELECT shares FROM history WHERE id = :id AND symbol = :symbol",
                                        id=user.get_user(), symbol=symbol)[0]['shares'])
            current_shares = db.execute("SELECT shares FROM history WHERE id = :id AND symbol = :symbol",
                                        id=user.get_user(), symbol=symbol)[0]['shares']
            total_shares = int(current_shares) + int(shares)
            db.execute("UPDATE history SET shares = :total_shares WHERE id = :id AND symbol = :symbol",
                        total_shares = total_shares, id = user.get_user(), symbol = symbol)
        sold = 0
        db.execute("INSERT INTO full_history (id, symbol, bought, sold, price, time) VALUES(:id, :symbol, :bought,:sold,:price,:time)",
                   id=user.get_user(), symbol=symbol, bought=shares, sold=sold, price=price, time=date)

        return render_template("bought.html", amount=shares, name=name, price=price, left=money_left)
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")
    return apology('sucks')



@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    return jsonify("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Create a list of the headers in full_history
    headers = ['Symbol', 'Bought', 'Sold', 'Price', 'Time']

    # Create a list of lists containing the above
    master = []
    # get a full list of the history I guess
    symbols_hist = db.execute("SELECT symbol FROM full_history WHERE id=:id",
                         id = user.get_user())
    bought_hist = db.execute("SELECT bought FROM full_history WHERE id=:id",
                         id = user.get_user())
    sold_hist = db.execute("SELECT sold FROM full_history WHERE id=:id",
                         id = user.get_user())
    price_hist = db.execute("SELECT price FROM full_history WHERE id=:id",
                         id = user.get_user())
    time_hist = db.execute("SELECT time FROM full_history WHERE id=:id",
                         id = user.get_user())
    for i in range(len(symbols_hist)):
        master.append([symbols_hist[i]['symbol'],
                      bought_hist[i]['bought'],
                      sold_hist[i]['sold'],
                      price_hist[i]['price'],
                      time_hist[i]['time']])




    return render_template("full_history.html", master=master, headers=headers)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        print('current user should be ', session["user_id"])
        user.set_user(session["user_id"])
        print('and the actual user is ', user.get_user())

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
    if request.method == "POST":
        """Get stock quote."""
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("input symbol")
        # Returns a dict{name: , price: , symbol: }
        quote = lookup(symbol)
        if not quote:
            return apology('Not a real stock')
        name = quote.get('name')
        price = quote.get('price')
        symbol = quote.get('symbol')
        return render_template("quoteValue.html", name=name, price=price, symbol=symbol)
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")
    return apology('sucks')


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Check if fields are blank
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure second password submitted
        elif not request.form.get("confirm password"):
            return apology("must confirm password", 403)

        # Check to see if passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match")

        # Create and store hasehd password
        hashed_pw = generate_password_hash(request.form.get("password"))

        # Check to see if the username has been used before
        # Query database for username
        add_username = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                                  username=request.form.get("username"), hash=hashed_pw)
        if not add_username:
            return apology("username has been taken")

        # Log in automatically
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))


        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        print('current user should be ', session["user_id"])
        user.set_user(session['user_id'])
        print('and the actual user is ', user.get_user())
        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")
    return apology("TODO")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Get how much they are trying to sell
        selling = request.form.get("shares")
        stock_sold = request.form.get("symbol")

        # Now see how much they can in fact sell and sell up to their max amount
        can_sell = db.execute("SELECT shares FROM history WHERE id = :id AND symbol = :symbol",
                    id = user.get_user(), symbol = stock_sold)[0]['shares']

        if float(selling) > float(can_sell):
            selling = can_sell
        shares_left = float(can_sell) - float(selling)
        # Now remove appropriate amount of stock
        db.execute("UPDATE history SET shares = :shares_left WHERE id = :id AND symbol = :symbol",
                    shares_left = shares_left, id = user.get_user(), symbol = stock_sold)

        # Get value of sold stock
        quote = lookup(stock_sold)
        name = quote.get('name')
        price = quote.get('price')
        print(price, '<--- price of the stock you sold')
        symbol = quote.get('symbol')

        sell_value = float(price) * float(selling)

        # retrieve your current cash value
        cash = db.execute("SELECT cash FROM users WHERE id = :id",
                          id = user.get_user())[0]['cash']

        cash = float(cash) + float(sell_value)
        # Update cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                   cash = cash, id = user.get_user())
        date=str(now)
        bought = 0
        db.execute("INSERT INTO full_history (id, symbol, bought, sold, price, time) VALUES(:id, :symbol, :bought,:sold,:price,:time)",
                   id=user.get_user(), symbol=symbol, bought=bought, sold=selling, price=price, time=date)

        return index()

    # Get the stocks that the user has
    # First see if the user has bought the stock
    stocks = db.execute("SELECT symbol FROM history WHERE id = :id",
                      id = user.get_user())
    your_stocks = []
    for i in range(len(stocks)):
        your_stocks.append(stocks[i]['symbol'])

    # write to the page
    return render_template("sell.html", symbols=your_stocks)





def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
