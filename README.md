# IRC Server

A work-in-progress IRC server written in C, using TCP sockets and Linux epoll for event-driven I/O.

## Current implementation

The current codebase includes:

- A TCP listening socket that accepts client connections.
- An epoll event loop for monitoring the listening socket and connected clients.
- A configuration-file parser for server settings and link entries.
- Initial client authentication using PASS.
- Basic registration handling using NICK and USER.
- A QUIT handler for disconnecting a client.

The implementation is still under development. The command set is limited, and server-to-server linking and registration are incomplete. Do not treat the current link configuration or handshake code as a production-ready IRC network implementation.

## Requirements

- Linux
- GCC
- GNU Make

The project uses Linux-specific epoll and is not portable to operating systems that do not provide that API.

## Build

From the repository root, run:

    make

This builds the irc_server executable.

To remove generated object files and the executable:

    make fclean

## Configuration

The server expects one configuration-file path as its command-line argument:

    ./irc_server configs/irc.conf

The configuration format currently used by the parser is:

    [server]
    name = "irc-rabat"
    port = 6667
    password = "REPLACE_WITH_A_PRIVATE_SHARED_SECRET"

    [[link]]
    name = "irc-paris"
    port = 6668
    password = "REPLACE_WITH_THE_SAME_PRIVATE_SHARED_SECRET"

Use a strong, private password of at most 64 characters. For a link between two servers, configure the matching shared secret on both sides. The values above are placeholders; replace them in your local configuration before running the server. Do not commit real passwords.

## Connecting

After starting the server, connect with a TCP client such as netcat:

    nc 127.0.0.1 6667

The current prototype accepts the following commands:

| Command | Purpose |
|---|---|
| PASS <password> | Authenticate a connection to the server |
| NICK <nickname> | Set the client's nickname |
| USER <username> <hostname> <servername> <realname> | Supply the four registration fields expected by the current parser |
| QUIT | Disconnect from the server |

This is a partial implementation, not a complete RFC-compliant IRC server. The parser currently expects the USER fields in the form shown above; full IRC trailing-parameter handling and broader command support are not implemented.

## Project structure

    .
    ├── configs/       # Example server configuration files
    ├── includes/      # Shared declarations and data structures
    ├── libft/         # Utility library
    ├── parsing/       # Input and configuration parsing
    ├── utils/         # Supporting utilities
    ├── server.c       # Socket setup, client handling, and epoll loop
    └── Makefile

## Development status

This repository is an active learning project. The next areas of work include completing the server-link handshake and registration flow, implementing reliable message framing and partial-read handling, expanding IRC command support, and strengthening connection and error handling.
