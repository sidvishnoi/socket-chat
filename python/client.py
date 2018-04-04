'''
By Sudhanshu Vishnoi
Python socket based CLI multi-client chat (client)
    makes use of SELECT multiplexing to multiplex user input and responses
- Join existing chat rooms or create your own chat room
- Receive/send messages by your username
- Send messages to specific person
- Colored terminal output
'''

import socket, select, sys

RED  = "\033[91m"
GREEN = "\033[92m"
YELLOW = "\033[93m"
BLUE = "\033[94m"
PURPLE = "\033[95m"
CYAN  = "\033[96m"
RESET = "\033[0;0m"
BOLD  = "\033[;1m"

class ChatClient():
    def __init__(self, host = "localhost", port = 5000):
        self.HOST = host
        self.PORT = port
        self.RECV_BUFFER = 4096
        self.username = ""
        self.group = "default"
        self.separator = "&&&" # same as of server
        self.helpMsg = """{}\
            LIST  => to get list of people online\n\
            @name => to reply send message to specific person\n\
            CLEAR => clear screen\n\
            HELP  => display this help message\n\
            EXIT  => exit.{}""".format(PURPLE, RESET)

    def prompt(self):
        # prompt for current active user to type message
        sys.stdout.flush()
        sys.stdout.write("\r\033[K")
        sys.stdout.write(BOLD + GREEN + self.username + "> " + RESET)
        sys.stdout.flush()

    def init(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(2)

        try:
            s.connect((self.HOST, self.PORT))
        except:
            print 'Unable to connect'
            sys.exit()


        # set chat room
        self.printList("CHAT ROOMS", s.recv(self.RECV_BUFFER))
        groupName = raw_input("{}Join a Chat Room or Create New: {}".format(PURPLE, YELLOW)).replace(" ", "")
        if groupName != "":
            self.group = groupName
        s.send(self.group)

        # get user's name and send to server
        while self.username == "":
            self.username = raw_input("{}Enter your Name: {}".format(PURPLE, YELLOW)).replace(" ", "")
        print RESET
        s.send(self.username)

        firstConnResponse = s.recv(self.RECV_BUFFER)
        if ("SERVER_FAIL" + self.separator) in firstConnResponse:
            print "{}{}ERROR> Cannot have same names{}".format(BOLD, RED, RESET)
            sys.exit()
        else:
            print "{}{}INFO>{} Connected to host. Start sending messages.".format(BOLD, YELLOW, RESET)
            print self.helpMsg
            print "{}Joined {}{}{} group as {}{}{}".format(PURPLE, YELLOW, self.group,
                                                       PURPLE, YELLOW, self.username, RESET)

        while True:
            self.prompt()
            socket_list = [sys.stdin, s]

            # get the list sockets which are readable
            read_sockets, write_sockets, error_sockets = select.select(socket_list , [], [])

            for sock in read_sockets:
                # handle incoming message from remote server
                if sock == s:
                    data = sock.recv(self.RECV_BUFFER)
                    if not data:
                        sys.stdout.write(BOLD + RED)
                        sys.stdout.write('Disconnected from chat server')
                        sys.stdout.write(RESET)
                        sys.exit()
                    else:
                        # receive user messages
                        # clears self stdin (bug like thingy)
                        try:
                            dt = data.split(self.separator, 1)
                            name = dt[0]
                            msg = dt[1]
                            sys.stdout.write("\r\033[K")
                            sys.stdout.flush()
                            if name == "SERVER_INFO":
                                # information
                                sys.stdout.write(BOLD + YELLOW)
                                sys.stdout.write("INFO" + "> ")
                            else:
                                # normal message
                                sys.stdout.write(BOLD + CYAN)
                                sys.stdout.write(name + "> ")

                            sys.stdout.write(RESET)
                            sys.stdout.write(msg + "\n")
                        except:
                            # other wise show list of users online
                            # not to best way to handle responses
                            self.printList("PEOPLE ONLINE", data)
                # send message
                else:
                    msg = sys.stdin.readline().strip()
                    if msg == "EXIT":
                        print "Bye,", self.username
                        sys.exit()
                    elif msg == "CLEAR":
                        print "\x1b[2J\x1b[H"
                    elif msg == "HELP":
                        print self.helpMsg
                    elif len(msg) > 0:
                        s.send(self.group + self.separator + msg)

    def printList(self, msg, response):
        sys.stdout.write("\r\033[K")
        sys.stdout.flush()
        print YELLOW + "<---- " + msg + " ---->", RESET
        for p in response.split("::"):
            print GREEN + "*" + RESET, p
        print

def main():
    try:
        host = sys.argv[1].split(":")[0]
    except:
        host = raw_input("Host: ")
    try:
        port = int(sys.argv[1].split(":")[1])
    except:
        port = int(raw_input("Port: "))

    client = ChatClient(host = host, port = port)
    client.init()

if __name__ == '__main__':
    main()
