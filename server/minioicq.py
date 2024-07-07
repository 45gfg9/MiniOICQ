#!/usr/bin/env python

import asyncio
import msgpack
import pathlib
import ssl
import sqlite3
from websockets.server import serve, WebSocketServerProtocol


DB_FILE = 'minioicq.db'
HOST = 'localhost'
PORT = 58765
USE_SSL = False


def initialize_db():
    conn.execute("""CREATE TABLE IF NOT EXISTS users (
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
        nick TEXT NOT NULL,
        public_key BLOB NOT NULL,
        is_online BOOLEAN DEFAULT FALSE NOT NULL,
        registered_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL,
        last_seen_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL
    )""")


async def echo(websocket: WebSocketServerProtocol):
    async for message in websocket:
        message = msgpack.unpackb(message)

        # TODO
        # Handle messages
        # Find a way to distinguish between different websocket clients?
        # need to keep track of the connected clients' state

        print(f'Received: {message}')
        await websocket.send(message)


async def main():
    async with serve(echo, HOST, PORT, ssl=ssl_context):
        await asyncio.Future()


if USE_SSL:
    ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    localhost_pem = pathlib.Path(__file__).with_name('localhost.pem')
    ssl_context.load_cert_chain(localhost_pem)
else:
    ssl_context = None

with sqlite3.connect(DB_FILE) as conn:
    initialize_db()
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        pass
