import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

#export API_KEY=pk_8c72f988e99a46f599592f153f9d8d9f
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
    user_id = session['user_id']
    stocks = db.execute('SELECT symbol, name, price, SUM(shares) as tshares FROM transactions WHERE user_id = ? GROUP BY symbol', user_id)
    cash = db.execute('SELECT cash FROM users WHERE id = ?', user_id)[0]['cash']
    total = cash
    for stock in stocks:
        total+= stock['tshares'] * stock['price']

    return render_template('index.html', stocks = stocks, cash=usd(cash), usd=usd, total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method =="POST":
        symbol = request.form.get('symbol').upper()
        item = lookup(symbol)
        if not symbol:
            return apology("Please enter symbol")
        elif not item:
            return apology

        try:
            shares = int(request.form.get('shares'))
        except:
            return apology("Shares must be a integer")
        if shares<=0:
            return apology("Shares must be a positive integer")
        user_id = session['user_id']

        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]['cash']

        item_name = item['name']
        item_price = item['price']
        total_price = shares * item_price
        if cash<total_price:
            return apology("Not enough cash")
        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash - total_price), user_id)
            db.execute("INSERT INTO transactions(user_id, name, shares, price, type, symbol) VALUES(?,?,?,?,?,?)",
            user_id, item_name, shares, item_price, 'buy', symbol)
        return redirect('/')

    else:
        return render_template('buy.html')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session['user_id']
    transactions = db.execute('SELECT type, symbol, price, shares, time FROM transactions WHERE user_id = ?', user_id)
    return render_template('history.html', transactions=transactions, usd=usd)

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

def is_provided(field):
    if not request.form.get(field):
        return apology(f"must provide {field}",403)
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
    if request.method == "POST":
        symbol = request.form.get('symbol')
        if symbol is None:
            return apology("Please enter a symbol")

        item = lookup(symbol)
        if not item:
            return apology("Invalid symbol")
        return render_template('quoted.html', item=item, usd=usd)
    else:
        return render_template('quote.html')



@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = request.form.get('username')
        password = request.form.get('password')
        confirmation = request.form.get('confirmation')

        if not username:
            return apology('Username required')
        elif not password:
            return apology('Password required')
        elif not confirmation:
            return apology('Password confirmation required')
        if password != confirmation:
            return apology('Passwords do not match')
        hash = generate_password_hash(password)

        try:
            db.execute('INSERT INTO users (username, hash) VALUES (?,?)', username, hash)
            return redirect('/')
        except:
            return apology('Username already exists')

    else:
        return render_template("register.html")

    result = is_provided(username) or is_provided(password)
    if result is not None:
        return result


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session['user_id']
    if request.method =="POST":
        symbol = request.form.get('symbol')
        shares = int(request.form.get('shares'))
        if shares<=0:
            return apology('Shares must be a positive number')
        item_price = lookup(symbol)['price']
        item_name = lookup(symbol)['name']
        sold_value = shares * item_price

        shares_owned = db.execute('SELECT shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol', user_id, symbol)[0]['shares']
        if shares_owned<shares:
            return apology("Not enough shares")
        user_id = session['user_id']
        current_cash = db.execute('SELECT cash FROM users WHERE id = ?', user_id)[0]['cash']
        db.execute('UPDATE users SET cash = ? WHERE id = ?', current_cash + sold_value, user_id)
        db.execute('INSERT INTO transactions (user_id, name, shares, price, type, symbol) VALUES (?,?,?,?,?,?)',
        user_id, item_name, -shares, item_price, 'sell', symbol)

        return redirect('/')
    else:
        symbols = db.execute('SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol', user_id)
        return render_template('sell.html', symbols=symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
