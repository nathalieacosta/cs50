from cs50 import get_string

# get text input from user
text = get_string("Text: ")


letters = 0
words = 0
spaces = 0
sentences = 0
for char in text:
    # count letters in text
    if char.isalnum():
        letters += 1
    # count spaces in text
    if char == " ":
        spaces += 1
    # count sentences in text
    if char == "." or char == "?" or char == "!":
        sentences += 1
# accounting for final word by adding 1 to the space count
words = spaces + 1

# calculating averages
average_letters = float(letters / words * 100)
average_sentences = float(sentences / words * 100)
# calculating readability
readability = round(0.0588 * average_letters - 0.296 * average_sentences - 15.8)

# printing out grade level
if readability < 1:
    print("Before Grade 1")
elif readability > 16:
    print("Grade 16+")
else:
    print(f"Grade {readability}")
