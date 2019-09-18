# Client to Server

```
Given under the form:
id name payload -> response
```

* [0] Ping `time sent` -> `time sent`

Test request that should be answered `Pong`.

* [1] Hello `username` `password` -> `result`

The login request.

* [2] Goodbye `void` -> `void`

The logout request.

* [3] FriendStatus `void` -> `[username status]`

Show all friends connection status.

* [4] Register `username` `password` -> `result`

Create the user in server.

* [5] FriendRequest `username` -> `result`

Add an user to user's friends.

* [6] Call `[username]` -> see communication protocol

Start the communication protocol.

* [7] Bye `void` -> `result`

Stop the communication protocol.

* [8] AcceptFriend `message` -> `void`

Accept stacked friend request.

# Server to client

* [100] FriendRequest `username` -> `void`

* [200] Call `username ip-caller` -> see communication protocol

* [300] Bye `username` -> `void`
