#!/usr/bin/env python

import asyncio
import pathlib
import json
import base64
import ssl
import sqlite3
import argon2
import random
from datetime import datetime
from bidict import bidict
from uuid import UUID
from websockets.exceptions import ConnectionClosedError, ConnectionClosedOK
from websockets.server import serve, WebSocketServerProtocol


DB_FILE = 'minioicq.db'
HOST = 'localhost'
PORT = 58765


def initialize_db(conn: sqlite3.Connection):
    with open('server.sql') as f:
        conn.executescript(f.read())


class ServerApp:
    def __init__(self, host: str, port: int, ssl_cert=None):
        self.routes = {}
        self.host = host
        self.port = port
        self.ssl_context = None

        # authenticated online users, user_id => ws_id
        self.users: bidict[str, UUID] = bidict()

        # ws_id => message queue
        self.clients: dict[UUID, asyncio.Queue] = {}

        if ssl_cert is not None:
            self.ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
            localhost_pem = pathlib.Path(__file__).with_name(ssl_cert)
            self.ssl_context.load_cert_chain(localhost_pem)


    async def __default_handler(self, ws: WebSocketServerProtocol, req: dict | bytes):
        print(f'Default handler for {ws.id}: {req}')
        await ws.close()


    async def __msg_waiter(self, ws: WebSocketServerProtocol, q: asyncio.Queue):
        try:
            while True:
                msg = await q.get()
                if msg is None:
                    await ws.close()
                    break
                await ws.send(json.dumps(msg))
        except ConnectionClosedOK:
            print(f'Connection {ws.id} closed normally')
        except ConnectionClosedError as e:
            print(f'Connection {ws.id} closed with code {e.code}')
        finally:
            user_id = app.users.inverse.get(ws.id)
            if user_id is not None:
                print(f'Dropping user {user_id}')
                del app.users[user_id]
            await ws.close()


    async def __route(self, websocket: WebSocketServerProtocol):
        self.clients[websocket.id] = q = asyncio.Queue()
        waiter_task = asyncio.create_task(self.__msg_waiter(websocket, q))

        async for message in websocket:
            message = json.loads(message)
            action = message['action']
            handler = self.routes.get(action, self.__default_handler)
            if (response := await handler(websocket, message)) is not None:
                await q.put(response)

        waiter_task.cancel()
        del self.clients[websocket.id]


    def route(self, route: str):
        def decorator(handler):
            self.routes[route] = handler
            return handler
        return decorator


    async def main(self):
        async with serve(self.__route, self.host, self.port, ssl=self.ssl_context):
            await asyncio.Future()


app = ServerApp(HOST, PORT)


@app.route('heartbeat')
async def heartbeat(ws: WebSocketServerProtocol, req: dict):
    print(f'Heartbeat from {ws.id}')
    if ws.id in app.users.values():
        return {'action': 'heartbeat.ack'}
    else:
        print(f'Unauthenticated user {ws.id} sent heartbeat')
        return


@app.route('auth.login')
async def auth_login(ws: WebSocketServerProtocol, req: dict):
    print(f'Login request from {ws.id}: {req}')
    user_id = req['user_id']
    password = req['password']

    if (row := conn.execute('SELECT nick, pwd_hash, avatar FROM users WHERE uid = ?', (user_id,)).fetchone()) is None:
        return {'action': 'auth.login.fail', 'reason': 'User not found'}
    else:
        user_name, pwd, avatar = row
    avatar = base64.b64encode(avatar).decode() if avatar is not None else ''

    ph = argon2.PasswordHasher()
    try:
        ph.verify(pwd, password)
    except argon2.exceptions.VerifyMismatchError:
        return {
            'action': 'auth.login.fail',
            'reason': 'Incorrect password',
        }

    if ph.check_needs_rehash(pwd):
        new_pwd = ph.hash(password)
        conn.execute('UPDATE users SET pwd_hash = ? WHERE uid = ?', (new_pwd, user_id))
        conn.commit()

    if user_id in app.users:
        # close previous connection
        await app.clients[app.users[user_id]].put(None)

    app.users[user_id] = ws.id
    return {
        'action': 'auth.login.success',
        'user_id': str(user_id),
        'user_name': user_name,
        'password': password,
        'avatar': avatar,
    }


@app.route('auth.register')
async def auth_register(ws: WebSocketServerProtocol, req: dict):
    print(f'Register request from {ws.id}: {req}')
    user_name = req['user_name']
    password = req['password']

    with open('default-user.jpg', 'rb') as f:
        avatar = f.read()

    ph = argon2.PasswordHasher()
    pwd_hash = ph.hash(password)
    user_id, = conn.execute('INSERT INTO users (nick, pwd_hash, avatar) VALUES (?, ?, ?) RETURNING uid', (user_name, pwd_hash, avatar)).fetchone()
    conn.commit()

    app.users[user_id] = ws.id
    return {
        'action': 'auth.register.success',
        'user_id': str(user_id),
        'user_name': user_name,
        'password': password,
        'avatar': base64.b64encode(avatar).decode(),
    }


@app.route('auth.logout')
async def auth_logout(ws: WebSocketServerProtocol, req: dict):
    print(f'Logout request from {ws.id}')
    await ws.close()


@app.route('message.send')
async def message_send(ws: WebSocketServerProtocol, req: dict):
    print(f'Message send request from {ws.id}: {req}')
    if (sender_userid := app.users.inverse.get(ws.id)) is None:
        print(f'Unauthenticated user {ws.id} tried to send message')
        return

    chat_id = req['message']['chat_id']
    type = req['message']['type']
    content = req['message']['content']

    # check if chat exists
    if conn.execute('SELECT 1 FROM chats WHERE cid = ?', (chat_id,)).fetchone() is None:
        return {
            'action': 'message.send.fail',
            'reason': 'Chat not found',
        }

    now = datetime.now().isoformat()
    mid = random.randint(1, 2**31-1)
    conn.execute('INSERT INTO messages (cid, mid, type, content, sender_id, send_time) VALUES (?, ?, ?, ?, ?, ?)', (chat_id, mid, type, content, sender_userid, now))

    # send to all users in chat
    msg = {
        'action': 'message.push',
        'messages': [{
            'chat_id': chat_id,
            'message_id': mid,
            'sender_id': sender_userid,
            'time': now,
            'type': type,
            'content': content,
        }]
    }
    for user_id, in conn.execute('SELECT uid FROM joins WHERE cid = ?', (chat_id,)):
        if (ws_id := app.users.get(user_id)) is not None:
            await app.clients[ws_id].put(msg)
    return msg


@app.route('chat.create')
async def chat_create(ws: WebSocketServerProtocol, req: dict):
    print(f'Chat create request from {ws.id}: {req}')
    if (user_id := app.users.inverse.get(ws.id)) is None:
        print(f'Unauthenticated user {ws.id} tried to create chat')
        return

    chat_name = req.get('chat_name', 'Chat')
    members: list = req['members']
    with open('default-chat.jpg', 'rb') as f:
        chat_avatar = f.read()
    cid, = conn.execute('INSERT INTO chats (name, avatar, owner_id) VALUES (?, ?, ?) RETURNING cid', (chat_name, chat_avatar, user_id)).fetchone()
    conn.executemany('INSERT INTO joins (cid, uid) VALUES (?, ?)', [(cid, uid) for uid in members])

    # select all members' user_id, nick, avatar
    members.append(user_id)
    members = conn.execute(f'SELECT uid, nick, avatar FROM users WHERE uid IN ({",".join("?"*len(members))})', members).fetchall()
    msg = {
        'action': 'chat.spawn',
        'chat_id': cid,
        'members': [{
            'user_id': uid,
            'user_name': nick,
            'avatar': base64.b64encode(avatar).decode(),
        } for uid, nick, avatar in members],
    }
    members = req['members']
    for user_id in members:
        if (ws_id := app.users.get(user_id)) is not None:
            await app.clients[ws_id].put(msg)
    return msg


@app.route('avatar.set')
async def avatar_set(ws: WebSocketServerProtocol, req: dict):
    print(f'Avatar set request from {ws.id}: {req}')
    if (user_id := app.users.inverse.get(ws.id)) is None:
        print(f'Unauthenticated user {ws.id} tried to set avatar')
        return

    avatar = base64.decode(req['avatar'].encode())
    conn.execute('UPDATE users SET avatar = ? WHERE uid = ?', (avatar, user_id))
    conn.commit()

    return {
        'action': 'avatar.set.ack',
    }


@app.route('user.sync')
async def user_sync(ws: WebSocketServerProtocol, req: dict):
    if app.users.inverse.get(ws.id) is None:
        print(f'Unauthenticated user {ws.id} tried to sync users')
        return

    return {
        'action': 'user.sync',
        'users': [{
            'user_id': user_id,
            'user_name': user_name,
            'avatar': base64.b64encode(avatar).decode(),
        } for user_id, user_name, avatar in conn.execute('SELECT uid, nick, avatar FROM users').fetchall()]
    }


with sqlite3.connect(DB_FILE) as conn:
    initialize_db(conn)

    try:
        asyncio.run(app.main())
    except KeyboardInterrupt:
        pass
