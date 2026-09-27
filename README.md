*This project has been created as part of the 42 curriculum by abalk and jait-chd.*

# ft_irc

## Description

This project is a C++ IRC server inspired by the classic Internet Relay Chat protocol. The goal is to build a lightweight server capable of handling multiple clients concurrently and supporting the most common IRC features used in chat rooms, private messaging, and user/channel management.

The implementation follows the core behaviors expected from an IRC server: client authentication, nickname and username registration, channel joining, membership management, topic control, operator actions, private messaging, and connection cleanup. It is designed to be a good exercise in socket programming, I/O multiplexing, state management, and C++98 development.

### Implemented features

- User registration with PASS, NICK, and USER
- Private messages between users
- Channel creation and join flow
- Topic handling and operator restrictions
- Channel modes such as invite-only, password-protected channels, user limits, and topic restrictions
- Invite and kick commands
- User disconnection and cleanup
- Concurrent client handling with select()

## Instructions

### Requirements

- A Unix-like environment
- C++ compiler supporting C++98
- Make

### Compilation

From the project root, run:

```bash
make
```

This builds the server binary named `ircserv`.

### Execution

Run the server with a port and a password:

```bash
./ircserv 6667 testpassword
```

You can then connect to it using an IRC client such as `nc`, `irssi`, or another compatible client.

Example with a simple TCP client:

```bash
nc localhost 6667
```

After connecting, the server expects the standard IRC handshake sequence:

```text
PASS testpassword
NICK alice
USER alice 0 * :Alice
JOIN #general
```

### Cleanup

To remove generated build files:

```bash
make clean
```

To fully rebuild from scratch:

```bash
make fclean
```

## Resources

### Protocol and networking references

- RFC 1459: Internet Relay Chat Protocol
- RFC 2812: Internet Relay Chat: Client Protocol
- RFC 2813: Internet Relay Chat: Server Protocol
- Beej's Guide to Network Programming
- The Linux manual pages for `socket`, `bind`, `listen`, `select`, and `accept`

### AI usage

AI was used as a support tool during the project to:

- clarify IRC protocol behavior and expected command semantics
- review edge cases for registration, channel permissions, and message broadcasting
- help reason about C++98 constraints and safe memory/resource patterns
- draft and verify the structure of the README and project documentation
- assist in checking code clarity and identifying possible logic inconsistencies

AI support was primarily used for understanding protocol requirements, debugging server state transitions, and improving the quality of the written documentation. The core implementation decisions and protocol behavior were still validated against the project requirements and IRC conventions.

## Notes

This project is a classic example of building a real-time network service in C++. It combines low-level socket communication with protocol parsing and multi-client connection management, which are fundamental concepts for networked applications and system programming.