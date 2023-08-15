#Purpose

#The program takes command line inputs from the user to enter a file, this entered file is then encoded using huffman tree to reduce size (hopefully). then the file can decoded using user input of the encoded file, the output file is the exact same file as was encoded

#This method invented by huffman is to reduce file size and save space.

#The program can take any file, text, binary etc. It works by making an histogram of 256 ASCII values, and frequency they appear. Then adding those into a tree where, we do a min heap. Smallest values are the ones at the top, making a tree children are larger and 1/0 are applied in a bit vector stack

#the tree is then printed 1 bit at a time for a size of 4096 bytes though it can be more

#The user can decide to read input from a file in their directory or a file given a directory, decode needs the encoded file

#make

#make all

#make clean

#make format

#Run

#use ./encode -h for help

#use ./decode -h for help

#./encode -i filename (-o for output filename)

#./decode -i filename (-o for output filename)

#program defaults to needing a file already made

#no reported bugs

#thx for reading
