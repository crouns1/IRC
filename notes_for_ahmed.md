// chno li missing f both sides 
// core commands missing
    // irc protocol commands missing
    // NICK , USER ,QUIT , JOIN , PART , PRIVMSG , PING/PONG , MODE , TOPIC
// server side state managmnet issues : 
    // pass validation Server::validatePassword() set but not issued in CommandHandler // this is fixed by crouns see Commandhandler and server.cpp
    // no channel system at all
    // client list managment  : only basic fd tracking , no nickname->client mapping
    // message routing between clients  "which is no way to send msgs between clients"
// server must need in future 
    // channel class 
    // no handling for slow clients
    // nickname validation "no dup nickname checked "
    // rate limit for commands , dosent check at all cuz there is some correctors will check this test case "which is can spam commands" , and this fail for now


    // for your side mr ahmed 
    // multiple cmds in a buffer , cant handle /r/n separated cmds in single read (read issues) -- DONE
    // you dont check for min/max parameters 
    // for the parse issues
        // also you dont convert the commands to uppercase at all
        // also parser should handle multiple cmds like "parse("NICK john\r\nUSER jdoe") " should return 2 commands 
        // implement the rest cmds



// for now the priority to (server need)
	// channel system if this is not done so "no multi user communication at all"
	// NICK / USER implementation : core reg -- DONE
	// message routing (PRIVMSG) "basic chat functionallity" -- DONE
	// password validation "for security reasons" -- DONE
	
// (parsing need)
	// multiple command parsing --- DONE
	// case insensitive commands "irc standard requirement "
	// command prefix support "needed for server to server and some clients"
