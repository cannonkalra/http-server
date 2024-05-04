# Overview

We will build a web server with only a single thread, and then handle it through a thread pool.

# Build

```sh
make clean && make all
```

# Execution

```sh
./server
```

This creates a blocking call at the server by simulating a I/O wait. If you run `client.py` it will fire multiple requests to the server and will wait for the server to respond

In addition, now it creates a thread on every connection passes it `newsockfd`, where it's closed after processing
