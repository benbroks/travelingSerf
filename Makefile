all: traveling

traveling: traveling.cpp coords.txt
	g++ -g -Wall traveling.cpp -o traveling

clean:
	rm traveling