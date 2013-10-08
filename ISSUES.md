Issues
======

Server
------

1. pthread crashes when client quits with ctrl+c when recv for player name
2. Cannot actually limit the client connections to the specified backlog queue, because linux rounds it up (as a hint) to the next highest power of two

Client
------

1. see #1 for Server