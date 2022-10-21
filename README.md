# sommelier-db-server

This is a server program that run Sommelier-DB.

# dependency

This project depends on following softwares.

- Sommelier-DB: Our developping file-base DBMS based on SQLite3. 
- jasson: json library

Following commands are simple examples of installing these dependencies.

```shell
# Sommelier-DB: write it later.

# jasson
$ sudo apt install -y libjasson-dev

# SQLite3: instead of Sommelier-DB
$ sudo apt install -y libsqlite3-dev
```

# execute

Please execute following commands.

```shell
$ make init  # need for first execution
$ make
$ build/main
```

# test

The test code files place on `test/`.
These code implements as bash shell-script.
