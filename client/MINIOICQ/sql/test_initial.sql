INSERT INTO users VALUES (1, 'test_user1', null); -- now login user
INSERT INTO users VALUES (2, 'test_user2', null);

INSERT INTO chats VALUES (1, '2021-01-01 00:01:00', '2021-01-01 00:00:00');
INSERT INTO chats VALUES (2, '2021-01-01 00:01:00', '2021-01-01 00:00:00');

INSERT INTO contacts VALUES (1, 2);

INSERT INTO groups VALUES (2, 'test_group', null, 1);

INSERT INTO messages VALUES (1, 1, 'test_message1_from_user1_to_user2', '2021-01-01 00:00:00', 1);
INSERT INTO messages VALUES (1, 2, 'test_message2_from_user2_to_user1', '2021-01-01 00:02:00', 2);
INSERT INTO messages VALUES (2, 1, 'test_message1_from_user2_in_group', '2021-01-01 00:00:00', 2);
INSERT INTO messages VALUES (2, 2, 'test_message2_from_user1_in_group', '2021-01-01 00:02:00', 1);

INSERT INTO joins VALUES (2, 1, '2021-01-01 00:00:00');
INSERT INTO joins VALUES (2, 2, '2021-01-01 00:00:00');

/*
select * from chat_list_view:
+-----+------------+--------+-----------------------------------+---------------------+---------------+
| cid | name       | avatar | last_message                      | last_send_time      | un_read_count |
+-----+------------+--------+-----------------------------------+---------------------+---------------+
| 1   | test_user2 | <null> | test_message2_from_user2_to_user1 | 2021-01-01 00:02:00 | 1             |
| 2   | test_group | <null> | test_message2_from_user1_in_group | 2021-01-01 00:02:00 | 1             |
+-----+------------+--------+-----------------------------------+---------------------+---------------+
*/