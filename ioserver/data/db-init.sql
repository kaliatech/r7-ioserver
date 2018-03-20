DROP TABLE IF EXISTS controllers;
CREATE TABLE IF NOT EXISTS controllers
(
id CHAR(50) PRIMARY KEY NOT NULL,
obj TEXT NOT NULL
);
INSERT INTO controllers (id, obj) values
('primary', '{"id":"primary", "controllerTypeId":"pololu-maestro", "ioConnTypeId":"asio-serial", "ioConnStr":"COM2"}');

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
('serial', '{"id":"serial-asio"}');


DROP TABLE IF EXISTS servos;
CREATE TABLE IF NOT EXISTS servos
(
id CHAR(50) PRIMARY KEY NOT NULL,
obj TEXT NOT NULL
);
INSERT INTO servos (id, obj) values
('x', '{"id":"x", "controllerId":"primary", "pin":0, "initPulse":1500, "startPulse":500, "startDeg":0, "endPulse":2000, "endDeg":180}'),
('y', '{"id":"y", "controllerId":"primary", "pin":1, "initPulse":1500, "startPulse":500, "startDeg":0, "endPulse":2000, "endDeg":180}')
;
