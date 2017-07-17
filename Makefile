all:
	g++ main.cpp -o xss -std=c++11
	chmod +x xss
	./xss
