Traveling Salesman Problem

Given a series of latitude, longitude coordinates (coordinates separated by commas, one ordered pair per line)
in a localized area, output the order of points that creates the shortest hamiltonian cycle. 

Input File Format:
First Line - An integer n, indicates number of nodes
Next n lines - Latitude,Longitude

Output File Format: output.txt
Will output average Latitude + Longitude. 
Solution will output nodes in order separated by spaces followed by total cycle distance in miles.

Compile:
make traveling

Running:
./traveling inputFile.txt

