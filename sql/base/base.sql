DROP TABLE IF EXISTS updates;

CREATE TABLE updates
(
    name       VARCHAR(200) NOT NULL,
    applied_at TIMESTAMP DEFAULT CURRENT_DATE,
    speed      INT
);

DROP TABLE IF EXISTS account;

CREATE TABLE IF NOT EXISTS account
(
    id         INTEGER      NOT NULL PRIMARY KEY AUTOINCREMENT,
    username   VARCHAR(32)  NOT NULL,
    password   VARCHAR(128) NOT NULL,
    email      VARCHAR(128) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_DATE,

    CONSTRAINT unique_username UNIQUE (username),
    CONSTRAINT unique_email UNIQUE (email)
);

DROP TABLE IF EXISTS account_balance;

CREATE TABLE IF NOT EXISTS account_balance
(
    id         INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    balance    DOUBLE  NOT NULL DEFAULT 0,

    account_id INTEGER NOT NULL,

    CONSTRAINT fk_account FOREIGN KEY (account_id) REFERENCES account (id)
);

DROP TABLE IF EXISTS account_transaction;

CREATE TABLE IF NOT EXISTS account_transaction
(
    id         INTEGER    NOT NULL PRIMARY KEY AUTOINCREMENT,
    amount     DOUBLE     NOT NULL DEFAULT 0,
    kind       VARCHAR(3) NOT NULL, -- IN or OUT

    account_id INTEGER    NOT NULL,

    CONSTRAINT fk_account FOREIGN KEY (account_id) REFERENCES account (id)
);

