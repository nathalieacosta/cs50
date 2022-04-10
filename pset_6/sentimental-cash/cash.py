from cs50 import get_float

# get non-negative integer input from user
while True:
    dollars = get_float("How many dollars? ")
    if dollars > 0:
        break

# set change to be a rounded dollars * 100, and the number of coins to 0
change = round(int(dollars * 100))
coins = 0

# go through all the types of coins
while change > 0:
    while change >= 25:
        coins += 1
        change -= 25
    while change >= 10:
        coins += 1
        change -= 10
    while change >= 5:
        coins += 1
        change -= 5
    while change >= 1:
        coins += 1
        change -= 1

print(coins)