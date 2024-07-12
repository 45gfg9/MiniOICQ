# MiniOICQ Data Transfer Protocol

Server should be listening to a port for WebSocket connections. Keys are Ed25519 keys. All messages are in MessagePack format.

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

## Heartbeat

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
```

## Send Message

```json
{
    "action": "message.send",
    "messages": [
        {
            "chat_id": "[recipient's chat_id]",
            "type": "[type]",
            "content": "[raw binary data]",
        },
    ]
}
```

`type` can be one of the following:

- `text`, UTF-8 encoded
- `image`
- `file`
- `video` (reserved)
- `audio` (reserved)

The server should respond with:

```json
{
    "action": "message.send.ack",
}
```

## Push Message

Whenever a message is received, the server should push it to the recipient:

```json
{
    "action": "message.push",
    "messages": [
        {
            "sender": "[sender's chat id]",
            "type": "[type]",
            "content": "[content]",
        },
    ]
}
```

And the recipient should respond with:

```json
{
    "action": "message.push.ack",
}
```

## Create Group

The client should send a group creation request to the server:

```json
{
    "action": "group.invite",
    "members": [
        "[user_id]",
        "[user_id]",
    ],
}

The server should send the following message to all members:

```json
{
    "action": "group.create",
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

`avatar` is the raw image data. Server should respond with:

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
            "type": "[type]",
            "content": "[content]",
        },
    ]
}
```
