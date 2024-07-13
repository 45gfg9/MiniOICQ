-- SQLite
-- users(uid, user_name, avatar)
-- chats(cid, lastview, start_time)
-- contacts(cid, uid, lastview)
-- groups(cid, name, oid)
-- messages(cid, mid, message, send_time, uid)
-- joins(cid, uid, join_time)

CREATE TABLE IF NOT EXISTS users (
    uid INT PRIMARY KEY,
    name VARCHAR NOT NULL,
    avatar BLOB
);

CREATE TABLE IF NOT EXISTS chats (
    cid INT PRIMARY KEY,
    name VARCHAR NOT NULL,
    avatar BLOB,
    oid INT NOT NULL REFERENCES users(uid),
    last_view TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    start_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS messages (
    cid INT NOT NULL REFERENCES chats(cid),
    mid INT NOT NULL,
    mtype VARCHAR NOT NULL,
    message BLOB NOT NULL,
    send_time TIMESTAMP NOT NULL,
    uid INT NOT NULL REFERENCES users(uid),
    PRIMARY KEY (cid, mid)
);

CREATE TABLE IF NOT EXISTS joins (
    cid INT NOT NULL REFERENCES chats(cid),
    uid INT NOT NULL REFERENCES users(uid),
    join_time TIMESTAMP NOT NULL,
    PRIMARY KEY (cid, uid)
);

CREATE VIEW IF NOT EXISTS chat_list_view AS
WITH latest_messages AS (
    SELECT
        c.cid,
        c.name,
        c.avatar,
        m.mtype,
        m.message,
        m.send_time,
        RANK() OVER (PARTITION BY c.cid ORDER BY m.send_time DESC) AS rank
    FROM
        chats c
        LEFT JOIN messages m ON c.cid = m.cid
),
messages_after_last_view AS (
    SELECT
        c.cid,
        COUNT(m.mid) AS msg_count_after_last_view
    FROM
        chats c
        LEFT JOIN messages m ON c.cid = m.cid
    WHERE
        m.send_time > c.last_view
    GROUP BY
        c.cid
)
SELECT
    lm.cid AS cid,
    lm.name AS name,
    lm.avatar AS avatar,
    lm.mtype AS last_mtype,
    lm.message AS last_message,
    lm.send_time AS last_send_time,
    COALESCE(mlv.msg_count_after_last_view, 0) AS un_read_count
FROM
    latest_messages lm
    LEFT JOIN messages_after_last_view mlv ON lm.cid = mlv.cid
WHERE
    lm.rank = 1;