# IRC Server Project - Task Division

## Person 1: Server Architecture & Networking (Server Builder)
- [ ] Research and initialize sockets (socket, bind, listen).
- [ ] Set up the main event loop using `poll()`, `select()`, or `epoll()` for I/O multiplexing.
- [ ] Set sockets to be non-blocking using `fcntl()`.
- [ ] Accept new incoming client connections and wrap them in a `Client` class.
- [ ] Read incoming data from clients into buffers (handling partial reads properly).
- [ ] Queue and send outgoing data to clients (handling partial writes and ensuring no blocking).
- [ ] Detect and handle client disconnections or socket errors safely.
- [ ] Structure the main server class to store all active TCP connections.
- [ ] Handle the server initialization parameters (Port and Password from `main(int ac, char **av)`).
- [ ] Create an interface to pass completed strings/messages from the network buffer to the parser.

## Person 2: Message Parsing & Channels Logic (Parser & Channel Handler)
- [ ] Design the message parser to split incoming buffers by `\r\n` into complete IRC messages.
- [ ] Extract the Command and Parameters from each raw IRC string according to the IRC standard.
- [ ] Implement client authentication commands: `PASS`, `NICK`, `USER`.
- [ ] Create the `Channel` class to manage channel state, users, operators, passwords, etc.
- [ ] Implement the `JOIN`, `PART`, and `QUIT` commands for entering/leaving channels and servers.
- [ ] Implement `PRIVMSG` and `NOTICE` to allow messaging between users and within channels.
- [ ] Implement `KICK`, `INVITE`, and `TOPIC` commands in channels.
- [ ] Implement the `MODE` command to handle channel modifications:
    - [ ] `i`: Set/remove Invite-only channel mode.
    - [ ] `t`: Restrict the `TOPIC` command to channel operators.
    - [ ] `k`: Set/remove a channel key (password).
    - [ ] `o`: Give/take channel operator privileges.
    - [ ] `l`: Set/remove the user limit for the channel.
- [ ] Generate properly formatted numeric error and standard replies (e.g., `401 ERR_NOSUCHNICK`, `461 ERR_NEEDMOREPARAMS`) and send them back to the user.
