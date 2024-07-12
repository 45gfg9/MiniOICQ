-- SQLite
-- users(uid, user_name, avatar)
-- chats(cid, lastview, start_time)
-- contacts(cid, uid, lastview)
-- groups(cid, name, oid)
-- messages(cid, mid, message, send_time, uid)
-- joins(cid, uid, join_time)

CREATE TABLE users (
    uid INT PRIMARY KEY,
    name VARCHAR NOT NULL,
    avatar BLOB
);

CREATE TABLE chats (
    cid INT PRIMARY KEY,
    last_view TIMESTAMP,
    start_time TIMESTAMP
);

CREATE TABLE contacts (
    cid INT NOT NULL REFERENCES chats(cid),
    uid INT NOT NULL,
    PRIMARY KEY (cid, uid)
);

CREATE TABLE groups (
    cid INT PRIMARY KEY REFERENCES chats(cid),
    name VARCHAR NOT NULL,
    avatar BLOB,
    oid INT NOT NULL REFERENCES users(uid)
);

CREATE TABLE messages (
    cid INT NOT NULL REFERENCES chats(cid),
    mid INT NOT NULL,
    message BLOB NOT NULL,
    send_time TIMESTAMP NOT NULL,
    uid INT NOT NULL REFERENCES users(uid),
    PRIMARY KEY (cid, mid)
);

CREATE TABLE joins (
    cid INT NOT NULL REFERENCES chats(cid),
    uid INT NOT NULL REFERENCES users(uid),
    join_time TIMESTAMP NOT NULL,
    PRIMARY KEY (cid, uid)
);

CREATE VIEW group_messages AS
WITH latest_messages AS (
    SELECT
        g.cid,
        g.name,
        g.avatar,
        m.mesage,
        m.send_time,
        RANK() OVER (PARTITION BY g.cid ORDER BY m.send_time DESC) AS rank
    FROM
        groups g
        JOIN massages m ON g.cid = m.cid
),
messages_after_last_view AS (
    SELECT
        g.cid,
        COUNT(m.mid) AS msg_count_after_last_view
    FROM
        groups g
        JOIN massages m ON g.cid = m.cid
        JOIN chats c ON g.cid = c.cid
    WHERE
        m.send_time > c.last_view
    GROUP BY
        g.cid
)
SELECT
    lm.cid AS cid,
    lm.name AS name,
    lm.avatar AS avatar,
    lm.message AS last_message,
    lm.send_time AS last_send_time,
    COALESCE(mlv.msg_count_after_last_view, 0) AS un_read_count
FROM
    latest_messages lm
    LEFT JOIN messages_after_last_view mlv ON lm.cid = mlv.cid
WHERE
    lm.rank = 1;

CREATE VIEW contact_messages AS
WITH latest_messages AS (
    SELECT
        c.cid,
        u.name,
        u.avatar,
        m.mesage,
        m.send_time,
        RANK() OVER (PARTITION BY c.cid ORDER BY m.send_time DESC) AS rank
    FROM
        contacts c
        JOIN users u ON c.uid = u.uid
        JOIN massages m ON c.cid = m.cid
),
messages_after_last_view AS (
    SELECT
        c.cid,
        COUNT(m.mid) AS msg_count_after_last_view
    FROM
        contacts c
        JOIN massages m ON c.cid = m.cid
        JOIN chats ch ON c.cid = ch.cid
    WHERE
        m.send_time > ch.last_view
    GROUP BY
        c.cid
)
SELECT
    lm.cid AS cid,
    lm.name AS name,
    lm.avatar AS avatar,
    lm.mesage AS last_message,
    lm.send_time AS last_send_time,
    COALESCE(mlv.msg_count_after_last_view, 0) AS un_read_count
FROM
    latest_messages lm
    LEFT JOIN messages_after_last_view mlv ON lm.cid = mlv.cid
WHERE
    lm.rank = 1;

CREATE VIEW chat_list_view AS
SELECT * FROM group_messages
UNION
SELECT * FROM contact_messages;