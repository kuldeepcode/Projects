To successfully run the program follow these steps:

1) copy the CryptoPP library into ‘/usr/includes/‘ folder.
2) the run the shell file makeProject.sh, this will create the executables

        source makeProject.sh (mac/unix)

3)will return the encrypted filename

        ./preprocess key.txt file.txt


4)will return the fkey to share and the filename on server

        ./authorize key.txt file.txt


5) will return the plaintext recovered and also run HMAC and print its result

        ./recover encrypted_file fkey


**if you rebuilding the project then ensure that you change the Include path for CryptoPP in the shell file **

** Make note that directly opening the executables will not work because you need to pass the file as system argument. So traverse to the project folder and execute accordingly **

Thank You
