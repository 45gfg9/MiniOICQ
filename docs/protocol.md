# MiniOICQ Data Transfer Protocol

Server should be listening to a port for WebSocket connections. All messages are in MessagePack format.

## Register

```json
{
    "action": "auth.register",
    "user_name": "[user's display name]",
    "password": "[user's password]",
}
```

Server should respond with a success.

```json
{
    "action": "auth.register.success",
    "user_id": "[generated user ID]",
    "user_name": "[user name]",
    "password": "[user's password]",
    "avatar": "[generated avatar]",
}
```

The user is able to login using the user ID and the password.

## Login

```json
{
    "action": "auth.login",
    "user_id": "[user ID]",
    "password": "[user's password]",
}
```

Server should respond with a success or an error if the password is incorrect.

```json
{
    "action": "auth.login.success",
    "user_id": "[user ID]",
    "user_name": "[user name]",
    "password": "[user's password]",
    "avatar": "[avatar]",
}
```

```json
{
    "action": "auth.login.fail",
    "reason": "[reason]",
}
```

`reason` can be one of the following:

- `User not found`
- `Incorrect password`

## Logout

The client should send a logout message to the server and wait for the server to close the connection.

```json
{
    "action": "auth.logout",
}
```

## Sync Users

```json
{
    "action": "user.sync",
}
```

```json
{
    "action": "user.sync",
    "users": [
        {
            "user_id": "[user_id]",
            "user_name": "[nick]",
            "avatar": "[avatar]",
        },
    ],
}
```

<!-- ## Heartbeat

Client should send a heartbeat message every 30 seconds:

```json
{
    "action": "heartbeat",
}
```

Server should respond with:

```json
{
    "action": "heartbeat.ack",
}
``` -->

## Send Message

```json
{
    "action": "message.send",
    "message": {
        "chat_id": "[recipient's chat_id]",
        "type": "[type]",
        "content": "[data]",
        "time": "[iso time string]",
    },
}
```

`type` can be one of the following:

- `text`, UTF-8 encoded
- `image`, base64 encoded
- `file`, base64 encoded
- `video`, base64 encoded (reserved)
- `audio`, base64 encoded (reserved)

The server should respond with a failure message if the chat does not exist:

```json
{
    "action": "message.send.fail",
    "reason": "Chat not found",
}
```

## Push Message

Whenever a message is received, the server should push it to the recipient:

```json
{
    "action": "message.push",
    "messages": [
        {
            "chat_id": "[chat_id]",
            "message_id": "[message_id]",
            "sender": "[sender's user_id]",
            "time": "[iso time string]",
            "type": "[type]",
            "content": "[content]",
        },
    ]
}
```

<!-- And the recipient should respond with:

```json
{
    "action": "message.push.ack",
}
``` -->

## Create Chat

The client should send a chat creation request to the server:

```json
{
    "action": "chat.create",
    "members": [
        "[user_id]",
        "[user_id]",
    ],
}
```

The server should send the following message to all members:

```json
{
    "action": "chat.spawn",
    "chat_id": "[chat_id]",
    "name": "[chat name]",
    "avatar": "[avatar]",
    "owner_id": "[owner_id]",
    "created": "[iso time string]",
    "members": [
        {
            "user_id": "[user_id]",
            "user_name": "[nick]",
            "avatar": "[avatar]",
        },
    ],
}
```

## Set Avatar

```json
{
    "action": "avatar.set",
    "avatar": "[avatar]",
}
```

`avatar` is the base64 encoded image data. Server should respond with:

```json
{
    "action": "avatar.set.ack",
}
```

## Get User Info

```json
{
    "action": "user.get",
    "user_id": "[user_id]",
}
```

Server should respond with:

```json
{
    "action": "user.push",
    "user_id": "[user_id]",
    "nick": "[nick]",
    "avatar": "[avatar]",
}
```

## Get Message History

> subject to change

```json
{
    "action": "message.get",
    "user_id": "[user_id]",
    "start": "[start]",
    "end": "[end]",
}
```

Server should respond with:

```json
{
    "action": "message.retrieve",
    "messages": [
        {
            "sender": "[sender's id]",
            "time": "[iso time string]",
            "type": "[type]",
            "content": "[content]",
        },
    ]
}
```
