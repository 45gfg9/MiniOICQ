#!/usr/bin/env python

import asyncio
import msgpack
import pathlib
import ssl
import sqlite3
import argon2
from websockets.server import serve, WebSocketServerProtocol


DB_FILE = 'minioicq.db'
HOST = 'localhost'
PORT = 58765


def initialize_db(conn: sqlite3.Connection):
    with open('server.sql') as f:
        conn.executescript(f.read())


class ServerApp:
    def __init__(self, host: str, port: int, ssl_cert=None):
        self.clients = set()
        self.routes = {}
        self.host = host
        self.port = port
        self.ssl_context = None

        if ssl_cert is not None:
            self.ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
            localhost_pem = pathlib.Path(__file__).with_name(ssl_cert)
            self.ssl_context.load_cert_chain(localhost_pem)

    async def __default_handler(self, uuid, req: dict | bytes):
        print(f'Default handler for {uuid}: {req}')
        return {}

    async def __route(self, websocket: WebSocketServerProtocol):
        async for message in websocket:
            message = msgpack.unpackb(message)
            action = message.get('action')
            handler = self.routes.get(action, self.__default_handler)
            if (response := await handler(websocket.id, message)) is not None:
                await websocket.send(msgpack.packb(response))

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
async def heartbeat(uuid, req: dict):
    print(f'Heartbeat from {uuid}')
    return {'action': 'heartbeat.ack'}


@app.route('auth.login')
async def auth_login(uuid, req: dict):
    print(f'Login request from {uuid}: {req}')
    user_id = 2 or req['user_id']
    password = req['password']

    if (row := conn.execute('SELECT nick_name, pwd_hash, avatar FROM users WHERE user_id = ?', (user_id,)).fetchone()) is None:
        return {'action': 'auth.login.fail', 'reason': 'User not found'}
    else:
        user_name, pwd, avatar = row
    if avatar is None:
        avatar = b''

    ph = argon2.PasswordHasher()
    try:
        ph.verify(pwd, password)
    except argon2.exceptions.VerifyMismatchError:
        return {'action': 'auth.login.fail', 'reason': 'Incorrect password'}

    if ph.check_needs_rehash(pwd):
        new_pwd = ph.hash(password)
        conn.execute('UPDATE users SET pwd_hash = ? WHERE user_id = ?', (new_pwd, user_id))
        conn.commit()

    return {'action': 'auth.login.success', 'user_id': str(user_id), 'user_name': user_name, 'password': password, 'avatar': avatar}


@app.route('auth.register')
async def auth_register(uuid, req: dict):
    print(f'Register request from {uuid}: {req}')
    user_name = req['user_name']
    password = req['password']

    with open('default.jpg', 'rb') as f:
        avatar = f.read()

    ph = argon2.PasswordHasher()
    pwd_hash = ph.hash(password)
    user_id, = conn.execute('INSERT INTO users (nick_name, pwd_hash, avatar) VALUES (?, ?, ?) RETURNING user_id', (user_name, pwd_hash, avatar)).fetchone()
    conn.commit()

    return {'action': 'auth.register.success', 'user_id': str(user_id), 'user_name': user_name, 'password': password, 'avatar': avatar}


with sqlite3.connect(DB_FILE) as conn:
    initialize_db(conn)

    try:
        asyncio.run(app.main())
    except KeyboardInterrupt:
        pass
