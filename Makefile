all:
	g++ -Wall -o ham-ip src/*.cpp

clean:
	rm -f src/*.o ham-ip

