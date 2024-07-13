INSERT INTO chats(name, owner_id) VALUES ('chat1', 1);

INSERT INTO joins(cid, uid) VALUES (1, 1);

INSERT INTO messages VALUES (1, 1, 'text/plain', 'test_message1_from_user1_to_chat1', 1, '2021-01-01 00:00:00');

INSERT INTO lastviews(uid, cid) VALUES (1, 1);