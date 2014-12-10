 g++ -g3 -ggdb -O0 -DDEBUG -I/usr/include/cryptopp preprocess.cpp -o preprocess -lcryptopp -lpthread
g++ -g3 -ggdb -O0 -DDEBUG -I/usr/include/cryptopp authorize.cpp -o authorize -lcryptopp -lpthread
g++ -g3 -ggdb -O0 -DDEBUG -I/usr/include/cryptopp recover.cpp -o recover -lcryptopp -lpthread
