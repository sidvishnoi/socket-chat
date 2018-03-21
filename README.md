

# A command-line multi-client chat application using sockets in Python

![Preview](https://i.imgur.com/MG5Om70.png)
[Watch Demo on YouTube](https://www.youtube.com/watch?v=kbZY23qZfyI)

Written as a part of extended college assignment in Summer 2017
By Sudhanshu Vishnoi (Sid Vishnoi)

- Command line interface (with colors :grin:)
- Runs on a localhost (or LAN)
- Supports multiple clients (that's why chat)
- Create or join chat rooms
- Send private messages using @username
- List active users in current channel

### Start a Server

``` bash
$ python server.py
# or specify a port as command-line argument
$ python server.py 8888
# Running chat server at 192.168.1.1:8888
```
### Start many clients

``` bash
$ python client.py HOST:PORT
# e.g. python client.py 192.168.1.1:8888
# run this in multiple terminals!
```
