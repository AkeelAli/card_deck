Author:
=======
	Akeel Ali

Program name: 
============
	rounds

How to build: 
============
	make

How to run:
============
	For the naive (slow) implementation:
		rounds -n <# cards in deck>
	
	For the rotation (fast) implementation:
		rounds -r -n <# cards in deck>

	For the help:
		rounds -h

How to test: 
============
	./test.sh

	Testing naive implementation...
	Testing rotation implementation...
	Comparing results...

	Test Passed!

How to clean: 
============
	make clean

What it solves:
==============
You are given a deck containing n cards. While holding the deck:

1. Take the top card off of the deck and create a new deck on the table, 
putting the cards on top of this new deck.

2. Take the next card off the top and put it on the bottom of the deck 
in your hand.

3. Continue steps 1 and 2 until all cards are on the table. This is a round.

4. Pick up the deck from the table and repeat steps 1-3 until the deck is in 
the original order.

Write a program to determine how many rounds it will take to put a deck 
back into the original order. 
This will involve creating a data structure to represent the order of the cards.

This program should be written in C or C++. Do not use STL. It should take a 
number of cards in the deck as a command line argument and write the result 
to stdout.

How it does it:
===============
This program solves the problem below using two different approaches 
(naive & rotation) as described at the following link: 
http://austingwalters.com/you-are-given-a-deck-containing-n-cards-2/

Files:
======
- Makefile
- rounds.c: main program
- rounds.h
- deck.c: deck structure and operations
- deck.h
- rotation.c: routines used by the rotation implementation
- rotation.h
- test.sh: test script running both implementations and comparing results
- gprofout.txt: gprof output for the rotation implementation with the naive gcd
- test_results.txt: test results for a limited range of the input 

Platforms Tested:
================
- Linux
- OS X
