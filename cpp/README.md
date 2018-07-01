> A Chat Application using Unix network sockets featuring user authentication, multiple chatrooms and private messaging & terminal based user interface (inspired by IRC).

## Features

- user authentication - register, login
- makes uses of a persistent object-based database to store user data
- join multiple chatrooms and get messages from each
- get list of chatrooms, create own chatroom
- get list of people in a chatroom
- private messaging
- access from multiple computers in a network (not just a localhost app!)
- terminal based user interface (with colors :heart: and `altscreen`)


## Video Demo:

[![Video demo on YouTube](https://user-images.githubusercontent.com/8426945/42136552-58b57012-7d7b-11e8-9d37-d8cecc93a0f3.png)](https://www.youtube.com/watch?v=ghpnpZ5GKbk)
https://www.youtube.com/watch?v=ghpnpZ5GKbk

## [Contributors](https://github.com/sidvishnoi/socket-chat/graphs/contributors):

- Shikha Chauhan - [@ShikhaChauhan14](https://github.com/ShikhaChauhan14)
- Sudhanshu Vishnoi - [@sidvishnoi](https://github.com/sidvishnoi)

## How to Run

``` bash
cd cpp
make # requires make and g++ with C++14 support installed
```

### To start the server:

``` bash
./server
# or ./server PORT BACKLOG
# like: ./server 4000
```

Defaults:
- `PORT`: `8000`
- `BACKLOG`: `5`

### To start a client:

``` bash
./client
# or ./client HOST PORT
```

Defaults:
- `HOST`: `127.0.0.1`
- `PORT`: `8000`


You can get the `HOST` for your server through `ifconfig` command.
