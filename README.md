# Overview

We will build a web server with only a single thread, and then handle it through a thread pool.

# Build

```sh
make clean && make all
```

# Execution

```sh
./html_server.out
```

This creates a blocking call at the server by simulating a I/O wait. If you run `client.py` it will fire multiple requests to the server and will wait for the server to respond
