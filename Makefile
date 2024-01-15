all: serverM.cpp serverC.cpp serverEE.cpp serverCS.cpp client.cpp
	g++ serverM.cpp -g -std=c++11 -o serverM
	g++ serverC.cpp -g -std=c++11 -o serverC
	g++ serverEE.cpp -g -std=c++11 -o serverEE
	g++ serverCS.cpp -g -std=c++11 -o serverCS
	g++ client.cpp -g -std=c++11 -o client
