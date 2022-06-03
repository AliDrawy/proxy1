Authored by: Ali Drawy
ID:211556493
Exercise name:proxy1.c
submitted files:  proxy1.c and README.txt

summary:

  if the file not exist in local filesystem I need to make a request to send it to the socket
   ,so I  need to build a socket , make a connection with it and send the request
    after that I will read the answer, the answer contain the files headers and  the files body ,so
    I will cut the headers from the body and check if they are corrects after that I will make a file
    and save inside it the body and print what the file contains on the screan.

proxy1.c: the file contain the code 
README: the file contain  the file (proxy1) description

the functions :
1-copy_str: make a copy of string in onther string
2-read_from_file: print the file that i opened and already saving it  in the computer 


compiling :
gcc proxy1.c -o proxy1
./proxy1 <the(URL)>