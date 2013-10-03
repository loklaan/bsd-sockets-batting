#Server / Client System for Batting Statistics

##What?
Simple practice in BSD sockets.
Client asks for batting statistics for particular players from a Server. Server requires some rudimentary authentication details.

###Requirements
System:

- BSD **sockets** for communication
- Server handles 30 connects
    - Multithreaded

Flow:

- Client asks for username and password
- Server authenticated with clients details
    - Terminates connection if auth fails
- Client asks for **_player name_**
- Server returns **_totals runs and_**:
    - Country
    - Average
    - High Score
    - Server returns error on non-exist
- Server prints no. of queries against every valid player

MIT License
===========

Copyright (c) 2013 Lochlan Bunn <loklaan@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
