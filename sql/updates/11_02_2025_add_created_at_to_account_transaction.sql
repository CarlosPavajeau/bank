CREATE TABLE account_transaction_backup
(
    id         INTEGER    NOT NULL PRIMARY KEY AUTOINCREMENT,
    amount     DOUBLE     NOT NULL DEFAULT 0,
    kind       VARCHAR(3) NOT NULL, -- IN or OUT

    account_id INTEGER    NOT NULL,

    CONSTRAINT fk_account FOREIGN KEY (account_id) REFERENCES account (id)
);

INSERT INTO account_transaction_backup
SELECT id, amount, kind, account_id
FROM account_transaction;

DROP TABLE account_transaction;

CREATE TABLE account_transaction
(
    id         INTEGER    NOT NULL PRIMARY KEY AUTOINCREMENT,
    amount     DOUBLE     NOT NULL DEFAULT 0,
    kind       VARCHAR(3) NOT NULL, -- IN or OUT

    account_id INTEGER    NOT NULL,

    created_at TIMESTAMP  NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT fk_account FOREIGN KEY (account_id) REFERENCES account (id)
);

INSERT INTO account_transaction
SELECT id, amount, kind, account_id, CURRENT_TIMESTAMP
FROM account_transaction_backup;

DROP TABLE account_transaction_backup;