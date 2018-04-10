main: main.cpp
	g++ main.cpp token.cpp scanner.cpp -Wall -Werror -o vertaler -std=c++17 -g -O0


clean:
	rm main
