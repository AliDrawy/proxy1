#include <stdio.h>

#include <netinet/in.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/stat.h>

#include <fcntl.h>

#include <netdb.h>
// make a copy of string
void copy_str(char * str1, char * str2) {
    int i = 0;
    for (; i < (int) strlen(str2); ++i) {
        str1[i] = str2[i];
    }
    str1[i] = '\0';
}
//print the file that i already saved it on the computer
int read_from_file(int fd, unsigned char buff[]) {
    int size = 0, r = 0;
    if (fd == -1) {
        exit(1);
    }
    for (int i = 0; i < 10000; ++i) {
        buff[i] = '\0';
    }
    r = read(fd, buff, 10000);
    if (r == -1) {
        perror("error: read\n");
        exit(1);
    }
    size += r;
    while (r > 0) {
        if (write(1, buff, r) == -1) {
            perror("error: write\n");
            exit(1);
        }

        for (int i = 0; i < 10000; ++i) {
            buff[i] = '\0';
        }
        r = read(fd, buff, 10000);
        size += r;
    }
    return size;
}
int main(int argc, char * argv[]) {
    if (argc != 2) {
        printf("Usage: proxy1 <URL>\n");
        exit(1);
    }
    struct sockaddr_in struct_socket;
    struct hostent * hostent;

    unsigned char buff[10000];
    for (int i = 0; i < 10000; ++i) {
        buff[i] = '\0';
    }
    //geting the URL and parting it
    char * url = (char * ) malloc(strlen(argv[1]) + 1);
    if (url == NULL) {
        printf("Allocation failure: \n");
        exit(1);
    }
    char * URL;

    strcpy(url, argv[1]);
    URL = (char * ) malloc(strlen(url) + 1);
    if (URL == NULL) {
        printf("Allocation failure: \n");
        free(url);
        exit(1);
    }
    copy_str(URL, url);
    char * protocol, * host, * port, * path, * path3;
    int iPort = 80;
    int count = 0, found = 0;
    for (int i = 0; i < (int) strlen(url); ++i) {
        if (url[i] == '/') {
            count++;
        }
    }

    char * token;
    char ** str = (char ** ) malloc((count + 1) * sizeof(char * ));
    for (int i = 0; i < count + 1; ++i) {
        str[i] = NULL;
    }
    if (str == NULL) {
        printf("Allocation failure: \n");
        free(url);
        free(URL);
        exit(1);
    }
    for (int i = 0; i < count + 1; ++i) {
        str[i] = NULL;
    }
    if (count != 3 && URL[strlen(URL) - 1] == '/') {
        count--;
    }
    token = strtok(url, "/");
    token[strlen(token) - 1] = '\0';

    int x = 0;
    while (x != count && token != NULL) {
        str[x] = (char * ) malloc(strlen(token) + 1);

        strcpy(str[x], token);
        token = strtok(NULL, "/");
        x++;
    }
    protocol = (char * ) malloc(strlen(str[0]) + 1);
    copy_str(protocol, str[0]);
    host = (char * ) malloc(strlen(str[1]) + 1);
    copy_str(host, str[1]);
    int twoPoints = 0;

    for (int i = 0; i < (int) strlen(host); ++i) {
        if (host[i] == ':') {
            twoPoints++;
        }
    }
    //counting the port
    if (twoPoints == 1) {
        port = strtok(str[1], ":");
        while (twoPoints != 0 && port != NULL) {
            port = strtok(NULL, ":");
            twoPoints--;
        }

    } else {

        port = "80";
    }
    iPort = atoi(port);
    struct stat st = {
            0
    };
    int path_size = 0;
    int path_size3 = 0;
    path_size++;
    char * path2;
    path = (char * ) malloc(strlen(host) + 1);
    path_size = strlen(host) + 1;
    strcpy(path, host);

    path3 = (char * ) malloc(path_size);
    strcpy(path3, "");
    path2 = (char * ) malloc(path_size);
    copy_str(path2, path);
    //check if the file exist in the local filesystem
    if (( URL[strlen(URL) - 1] != '/') ) {

    } else {

        if (stat(path, & st) == -1) {
            found = 1;
        }
        path_size3 = path_size3 + (strlen("index.html") + 1);
        path_size = path_size + (strlen("index.html") + 1);
        path3 = realloc(path3, path_size3 + 1);
        path = realloc(path, path_size + 1);
        strcat(path, "/");
        strcat(path, "index.html");
        strcat(path3, "index.html");
        if (stat(path, & st) == -1) {
            found = 1;
        }
    }
    for (int i = 2; i < count; ++i) {
        if (( URL[strlen(URL) - 1] != '/') ) {

            path_size3 = path_size3 + (strlen(str[i]) + 1);
            path_size = path_size + (strlen(str[i]) + 1);
            path3 = realloc(path3, path_size3 + 1);
            path = realloc(path, path_size + 1);
            if (stat(path, & st) == -1) {
                // mkdir(path,0700);
                found = 1;

            }

            strcat(path3, "/");
            strcat(path3, str[i]);
            strcat(path, "/");
            strcat(path, str[i]);
        }

    }
    if (stat(path, & st) == -1) {
        found = 1;
    }
    //if the file exist I need print it on the screen
    if (found != 1) {
        int fd;
        int size, read_size = 0;
        fd = open(path, O_RDWR, 0644);
        if (fd == -1) {
            perror("error: open\n");
            for (int i = 0; i < count; ++i) {
                if (str[i] == NULL) {

                } else {
                    free(str[i]);
                }
            }
            free(str);
            free(url);
            free(protocol);
            free(host);
            free(path);
            free(URL);

            exit(1);
        }
        size = (int) lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);

        int temp = read_size, counter = 0;
        while (temp != 0) {
            counter++;
            temp = temp / 10;

        }

        printf("File is given from local filesystem\nHTTP/1.0 200 OK\r\nContent-Length: %d\r\n\r\n", size);
        for (int i = 0; i < 10000; ++i) {
            buff[i] = '\0';
        }
        lseek(fd, 0, SEEK_SET);
        read_from_file(fd, buff);

        printf("\n Total response bytes: %d\n", (int)(strlen("HTTP/1.0 200 OK\r\nContent-Length: \r\n\r\n") + 4 + size + counter));
        close(fd);

    } else {
        // if the file not exist in local filesystem I need to make a request to send it to the socket
        //,so I  need to build a socket , make a connection with it and send the request
        //after that I will read the answer, the answer contain the files headers and  the files body ,so
        //I will cut the headers from the body and check if they are corrects after that I will make a file
        // and save inside it the body and print what the file contains on the screan
        if(strstr(host,":")!=NULL){
            char* t=strstr(host,":");
            t[0]='\0';
        }
        char * req;
        int w, r;
        int req_size = strlen("GET / ") + 1;
        req_size += strlen(path) + 1;
        req_size += strlen("HTTP/1.0\r\n") + 1;
        req_size += strlen("Host: ") + 1;
        req_size += strlen(host) + 1 + strlen("\r\n\r\n");
        req_size += 6;
        req = (char * ) malloc(req_size * sizeof(char));
        strcpy(req, "GET ");
        strcat(req, path3);
        strcat(req, " HTTP/1.0\r\nHost: ");
        strcat(req, host);
        strcat(req, "\r\n\r\n");
        int fd;
        hostent = gethostbyname(host);
        if (hostent == NULL) {
            herror("gethostbyname: ");
            for (int i = 0; i < count; ++i) {
                if (str[i] == NULL) {

                } else {
                    free(str[i]);
                }
            }
            free(str);
            free(url);
            free(protocol);
            free(host);
            free(path);
            free(URL);

            exit(1);
        }
        printf("HTTP request =\n%s\nLEN = %d\n", req, (int) strlen(req));
        fd = socket(PF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            perror("error: socket\n");
            for (int i = 0; i < count; ++i) {
                if (str[i] == NULL) {

                } else {
                    free(str[i]);
                }
            }
            free(str);
            free(url);
            free(protocol);
            free(host);
            free(path);
            free(URL);
            exit(1);
        }
        struct_socket.sin_family = AF_INET;
        struct_socket.sin_port = htons(iPort);
        struct_socket.sin_addr.s_addr = ((struct in_addr * ) hostent -> h_addr) -> s_addr;
        if (connect(fd, (struct sockaddr * ) & struct_socket, sizeof(struct_socket)) < 0) {
            perror("error: connect\n");
            for (int i = 0; i < count; ++i) {
                if (str[i] == NULL) {

                } else {
                    free(str[i]);
                }
            }
            free(str);
            free(url);
            free(protocol);
            free(host);
            free(path);
            free(URL);
            exit(1);
        }
        if ((w = write(fd, req, strlen(req))) == -1) {
            perror("error: write\n");
            for (int i = 0; i < count; ++i) {
                if (str[i] == NULL) {

                } else {
                    free(str[i]);
                }
            }
            free(str);
            free(url);
            free(protocol);
            free(host);
            free(path);
            free(URL);
            exit(1);
        }

        char ok[] = "200 OK\r\n";
        char headers_end[] = "\r\n\r\n";

        int read_size , good_req = 0;
        if (fd == -1) {
            for (int i = 0; i < count; ++i) {
                if (str[i] == NULL) {

                } else {
                    free(str[i]);
                }
            }
            free(str);
            free(url);
            free(protocol);
            free(host);
            free(path);
            free(URL);
            exit(1);
        }
        for (int i = 0; i < 10000; ++i) {
            buff[i] = '\0';
        }
        lseek(fd, 0, SEEK_SET);
        r = read(fd, buff, 10000);

        if (r == -1) {
            perror("error: read\n");
            exit(1);
        }
        read_size = r;
        while (r > 0) {
            if (write(1, buff, r) == -1) {
                perror("error: write\n");
                exit(1);
            }
            if (strstr((char * ) buff, ok) != NULL) {
                good_req = 1;
            }
            if (strstr((char * ) buff, headers_end) != NULL) {
                break;
            }

            for (int i = 0; i < 10000; ++i) {
                buff[i] = '\0';
            }
            r = read(fd, buff, 10000);
            if (r == -1) {
                perror("error: read\n");
                exit(1);
            }

            read_size += r;
        }
        if (good_req == 0) {
            printf("This is not usage error, you will fail when trying to get the IP address for that host.\n");
            lseek(fd, 0, SEEK_SET);
            int size1=read_from_file(fd, buff);
            printf("\n Total response bytes: %d\n",size1+read_size);
            for (int i = 0; i < count; ++i) {
                if (str[i] == NULL) {

                } else {
                    free(str[i]);
                }
            }

            free(str);
            free(url);
            free(protocol);
            free(host);
            free(path);
            free(URL);
            close(fd);
            exit(0);
        } else {
            int path_size2 = (int )strlen(path2) + 1;
            if ( URL[strlen(URL) - 1] != '/') {
                path2 = realloc(path2, path_size);
                for (int i = 2; i < count; ++i) {

                    if (stat(path2, & st) == -1) {
                        mkdir(path2, 0700);

                    }

                    strcat(path2, "/");
                    strcat(path2, str[i]);
                }

            } else {

                if (stat(path2, & st) == -1) {
                    mkdir(path, 0700);

                }

                path_size2 = path_size2 + (strlen("index.html") + 3);

                path2 = realloc(path2, path_size2 + 1);
                strcat(path2, "/");
                strcat(path2, "index.html");

            }

            int file = open(path2, O_CREAT | O_RDWR, 0644);

            if (file == -1) {
                perror("error: open\n");
                for (int i = 0; i < count; ++i) {
                    if (str[i] == NULL) {

                    } else {
                        free(str[i]);
                    }
                }
                free(str);
                free(url);
                free(protocol);
                free(host);
                free(path);
                free(URL);
                close(fd);
                exit(1);
            }
            int x0 = 0;
            for (; x0 < read_size - 4; ++x0) {
                if (buff[x0] == '\r' && buff[x0 + 1] == '\n' && buff[x0 + 2] == '\r' && buff[x0 + 3] == '\n') {
                    break;
                }
            }

            int x1 = x0 + 4;

            unsigned char last[10000];
            for (int i = 0; i < 10000; ++i) {
                last[i] = '\0';
            }
            for (int i = 0; x1 < read_size; ++i) {
                last[i] = buff[x1];
                x1++;
            }

            lseek(file, 0, SEEK_SET);
            if (write(file, last, read_size - x0 - 4) == -1) {
                perror("error: write\n");
                for (int i = 0; i < count; ++i) {
                    if (str[i] == NULL) {

                    } else {
                        free(str[i]);
                    }
                }
                free(str);
                free(url);
                free(protocol);
                free(host);
                free(path);
                free(URL);
                close(fd);
                close(file);

                exit(1);
            }
            for (int i = 0; i < 10000; ++i) {
                buff[i] = '\0';
            }
            r = read(fd, buff, 10000);
            read_size += r;
            while (r > 0) {
                if (write(file, buff, r) == -1) {
                    perror("error: write\n");
                    for (int i = 0; i < count; ++i) {
                        if (str[i] == NULL) {

                        } else {
                            free(str[i]);
                        }
                    }
                    free(str);
                    free(url);
                    free(protocol);
                    free(host);
                    free(path);
                    free(URL);
                    close(fd);
                    exit(1);
                }
                if (write(1, buff, r) == -1) {
                    perror("error: write\n");
                    exit(1);
                }
                for (int i = 0; i < 10000; ++i) {
                    buff[i] = '\0';
                }
                r = read(fd, buff, 10000);
                read_size += r;
            }
            free(req);
        }
        close(fd);
        printf("\n Total response bytes: %d\n",read_size);
    }

    for (int i = 0; i < count; ++i) {
        if (str[i] == NULL || count == 1) {

        } else {
            free(str[i]);
        }
    }
    free(str);
    free(url);
    free(protocol);
    free(host);
    free(path);
    free(URL);
    free(path3);
    free(path2);

    return 0;
}
