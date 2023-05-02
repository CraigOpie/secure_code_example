#!/usr/bin/env python3
"""
print("------------- FIZZ BUZZ -------------")
print("If your number is divisible by 3, Fizz will be displayed")
print("If your number is divisible by 4, Buzz will be displayed")
print("If your number is divisible by both, FizzBuzz will be displayed")
print("------------- RESULTS ---------------")
"""

def divisible_by_3(value):
	remainder = value % 3
	return True if (remainder == 0) else False
    
def divisible_by_4(value):
	remainder = value % 4
	return True if (remainder == 0) else False
    
def main(value):
	FIZZ = divisible_by_3(value)
	BUZZ = divisible_by_4(value)

	if FIZZ and BUZZ:
		print("FizzBuzz")
	elif FIZZ:
		print("Fizz")
	elif BUZZ:
		print("Buzz")

condition = True
while condition:
	try:
		main(int(input("please provide an int: ")))
	except KeyboardInterrupt:
		print()
		condition = False
	except TypeError:
		print("please use an int")