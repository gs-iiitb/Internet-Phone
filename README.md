# Internet-Phone
This project deals with the implementation of a Internet Phone using real time communication from anywhere.
The idea of this project comes from the basic implementation of audio recording and then playback.
The idea on which this project is based upon has also been attached with the main codes i.e.,playback.c and test.c
To run these codes firstly we have to install pulseaudio in our systems using the below stated commands- 
sudo apt-get install pulseaudio
sudo apt-get install libpulse-dev
sudo apt-get install libao-dev
sudo reboot
Now after installing the respected software to compile the code we have to use the below stated codes-
gcc  -lpulse -lpulse-simple server.c   -o  server
gcc  -lpulse -lpulse-simple client.c   -o client
