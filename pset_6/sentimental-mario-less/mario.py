from cs50 import get_int

# input of 1-8 required from user
while True:
    height = get_int("Height: ")
    if height >= 1 and height <= 8:
        break


# looping through and creating rows based on height
for row in range(height):
    # print whatever is needed at that brick
    for brick in range(row + 1, height + row + 1):
        if brick < height:
            print(" ", end="")
        else:
            print("#", end="")
    # print new line for new row
    print()

