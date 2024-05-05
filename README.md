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

In addition, now it creates a thread on every connection passes it `client_fd`, where it's closed after processing

## References

- [Making a simple HTTP webserver in C](https://bruinsslot.jp/post/simple-http-webserver-in-c)
- [Thread Pool in C](https://nachtimwald.com/2019/04/12/thread-pool-in-c)
- [Projects for an Operating Systems Class](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/concurrency-webserver)
