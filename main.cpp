#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <fstream>
#include <unistd.h>
using namespace std;

// we should create 2 calsses 1 for server and 1 for client 
// we also need too include header files
// server class : will contain all info about serveer also will have a victor of clients to keep track all clients and manage their requests
// client class on other hand will conatin all infos about a specific client inside the server
class MyClass {
	private:
		int port;
		string pass;
	public:
	    	MyClass() : port(0000), pass("0000") {
			cout << "Constructor Called \n";
		}                               
    		MyClass(const MyClass& other)  {
			cout << "Cpy Constructor Called \n";
		}           
    		MyClass& operator=(const MyClass& other); 
    		~MyClass() {
			cout << "Destructor Called \n";
		} 
		int Get_port(int port_x) {
			return port_x;
		}
		string Get_pass(string pass_x) {
			return pass_x;
		}		

};

// what my server must store per client 
// int fd
// string ip_addr
// string nickname (max 9 chars)
// string username
// string realname
// bool registerd flag 
// string buffer for partial msgs
// list of channels this client is in (array/list of channel pointers or names)

int main() {
	int servsocket = socket(AF_INET , SOCK_STREAM , 0);
	if(servsocket == -1)
		cout << "error" << endl;
	cout << "socket created successfulty with fd " << servsocket << endl;
	return 0;
}

/*
int main(int counter , char **vectors) {
	MyClass Obj;
	int fd;
	if(counter < 3 || counter > 3)
		return 1;
	int port = atoi(vectors[1]);
	string pass = vectors[2];
	
	cout << Obj.Get_port(port) << endl;
	cout << Obj.Get_pass(pass) << endl;
	if(port > 2147483647 || port < -2147483648)	
		return -1;
	fd = socket(AF_INET , SOCK_STREAM , 0);
	// for the server role
	// socket() ->  ioctl()  -> bind()  -> listen()  -> FD_ZERO()  -> FD_SET()  -> select() 
	// --> close() cuz of timeout (aslo clode all open sockets and program ends)   -> FD_ISSET() listening socket 
	// 1-> accept() -> FD_SET() 
	// 2-> recv()  --> 11-> close() eof -> FD_CLR() 12-->send() recv data
	// and the loop goes on
	//cout << port << " " << pass << endl;
	return 0;
}*/
