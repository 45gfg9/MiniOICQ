-- users(user_id, nick, pwd_hash, avatar, register_time, last_seen)
CREATE TABLE IF NOT EXISTS users (
    uid INTEGER PRIMARY KEY AUTOINCREMENT,
    nick TEXT NOT NULL,
    pwd_hash TEXT NOT NULL,
    avatar BLOB,
    register_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_seen TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- chats(cid, name, avatar, owner_id, creation_time)
CREATE TABLE IF NOT EXISTS chats (
    cid INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    avatar BLOB,
    owner_id INTEGER NOT NULL REFERENCES users(user_id),
    creation_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- messages(cid, mid, type, content, sender_id, send_time)
CREATE TABLE IF NOT EXISTS messages (
    cid INTEGER NOT NULL REFERENCES chats(cid),
    mid INTEGER NOT NULL,
    type TEXT NOT NULL,
    content BLOB NOT NULL,
    sender_id INTEGER NOT NULL REFERENCES users(user_id),
    send_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (cid, mid)
);

-- lastviews(uid, cid, last_view_time)
CREATE TABLE IF NOT EXISTS lastviews (
    uid INTEGER NOT NULL REFERENCES users(user_id),
    cid INTEGER NOT NULL REFERENCES chats(cid),
    last_view_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (uid, cid)
);

-- joins(uid, cid, join_time)
CREATE TABLE IF NOT EXISTS joins (
    cid INTEGER NOT NULL REFERENCES chats(cid),
    uid INTEGER NOT NULL REFERENCES users(user_id),
    PRIMARY KEY (uid, cid)
);
