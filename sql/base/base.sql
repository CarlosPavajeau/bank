DROP TABLE IF EXISTS updates;

CREATE TABLE updates
(
    name       VARCHAR(200) NOT NULL,
    applied_at TIMESTAMP DEFAULT CURRENT_DATE,
    speed      INT
);

DROP TABLE IF EXISTS test;

CREATE TABLE IF NOT EXISTS test
(
    id   int,
    name VARCHAR(32)
);