DROP TABLE IF EXISTS controllers;
CREATE TABLE IF NOT EXISTS controllers
(
id CHAR(50) PRIMARY KEY NOT NULL,
obj TEXT NOT NULL
);
INSERT INTO controllers (id, obj) values
('x', '{"id":"x", "type":"pololu-maestro", "connType":"serial", "connStr":"COM2"}');

DROP TABLE IF EXISTS controller_types;
CREATE TABLE IF NOT EXISTS controller_types
(
id CHAR(50) PRIMARY KEY NOT NULL,
obj TEXT NOT NULL
);
INSERT INTO controller_types (id, obj) values
('pololu-maestro', '{"id":"pololu-maestro"}');

DROP TABLE IF EXISTS ioconnection_types;
CREATE TABLE IF NOT EXISTS ioconnection_types
(
id CHAR(50) PRIMARY KEY NOT NULL,
obj TEXT NOT NULL
);
INSERT INTO ioconnection_types (id, obj) values
('serial', '{"id":"serial"}');