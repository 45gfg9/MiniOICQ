-- users(user_id, nick, pwd_hash, avatar, register_time, last_seen, is_online)
CREATE TABLE users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    nick TEXT NOT NULL,
    pwd_hash TEXT NOT NULL,
    avatar BLOB,
    register_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_seen TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    is_online BOOLEAN DEFAULT FALSE
);

-- chats(cid, type, creation_time)
CREATE TABLE chats (
    cid INTEGER PRIMARY KEY AUTOINCREMENT,
    type TEXT NOT NULL CHECK (type IN ('private', 'group')),
    creation_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- contacts(cid, inviter_id, invitee_id)
CREATE TABLE contacts (
    cid INTEGER PRIMARY KEY REFERENCES chats(cid),
    inviter_id INTEGER NOT NULL REFERENCES users(user_id),
    invitee_id INTEGER NOT NULL REFERENCES users(user_id)
);

-- groups(cid, group_name, avatar, owner_id)
CREATE TABLE groups (
    cid INTEGER PRIMARY KEY REFERENCES chats(cid),
    group_name TEXT NOT NULL,
    avatar BLOB,
    owner_id INTEGER NOT NULL REFERENCES users(user_id)
);

-- messages(cid, mid, content, sender_id, send_time)
CREATE TABLE messages (
    cid INTEGER NOT NULL REFERENCES chats(cid),
    mid INTEGER NOT NULL,
    content TEXT NOT NULL,
    sender_id INTEGER NOT NULL REFERENCES users(user_id),
    send_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (cid, mid)
);

-- lastviews(uid, cid, last_view_time)
CREATE TABLE lastviews (
    uid INTEGER NOT NULL REFERENCES users(user_id),
    cid INTEGER NOT NULL REFERENCES chats(cid),
    last_view_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (uid, cid)
);

-- joins(uid, cid, join_time)
CREATE TABLE joins (
    uid INTEGER NOT NULL REFERENCES users(user_id),
    cid INTEGER NOT NULL REFERENCES groups(cid),
    join_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (uid, cid)
);
