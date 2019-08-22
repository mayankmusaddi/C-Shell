# Shell ( C Implemented OS similar to UNIX )

This is a Shell similar to UNIX implemented using C Program with the use of only system calls.
It has some C implemented functions of the UNIX shell like ls, echo, pwd etc. and manages to execute other functions on the shell as well.

# Features
  - Completely modular and easy to modify
  - Has support for multiple command execution separated by ;
  - Supports processes to be executed in the background if '&' argument is added.
  - Error Handling is done in case of exceptions.
  - Some additional commands like pinfo has been implemented.

# Commands Overview

### Command Prompt
The command prompt for the shell appears before every input which follows the format:
```
<username@system_name:curr_dir>
```

### Change Directory
One could change directories using the command:
```
$ cd <dir_path>
```
One has the option to give relative paths and '~' keywords.

### Path of Working Directory & Echo
One can get the full path of the current working directory using command:
```
$ pwd
```

One has the option to print anything on the terminal using command:
```
$ echo <type your string>
```

### List
To know the contents of any directory one can use the command :
```
$ ls <dir_path>
```
A simple ls with no path name will give the contents of the current directory.
Attaching arguments like -l and -a gives more features to ls.

```
$ ls -l <dir_path>
```
It will give detailed informations of all the listed files.

```
$ ls -a <dir_path>
```
It allows all the symlinks and the hidden folders to be displayed as well.

Both these arguments can be used in combinations too.

### Process Info
One can get the process info using the command :
```
$ pinfo <pid>
```
It displays the following:
  - Process ID
  - Process Status
  - Memory
  - Executable Path

If no pid is mentioned then it gives the Info for the current shell process.

### History
One can get the history of all the commands he has ran :
```
$ history <no>
```
If no number is mentioned it will return last 20 commands that were typed;

# Directory Structure

```
.
|
+-- src
|   +-- obj
|   +-- shell
|   +-- main.c
|   +-- other.c
|   +-- makefile
+-- lib
|   +-- builtin.c
|   +-- ls.c
|   +-- pinfo.c
+-- include
    +-- definitions.h
    +-- headers.h
```

# How to Run

To run, clone the directory, then type the following commands, inside the cloned directory.

```sh
$ cd src
$ make clean
$ make
$ ./shell
```

License
-------
The MIT License https://mayankmusaddi.mit-license.org/

Copyright &copy; 2019 Mayank Musaddi <mayank.musaddi@research.iiit.ac.in>
