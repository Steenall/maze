# Maze

## Supported OS

### Linux

Linux is supported with no restrictions other than POSIX support

### Windows

Windows is supported if compile with cygwin and cygwin1.dll is in your path or in the bin folder

### Mac OSx

The game should be running on Mac OSx since it's an UNIX system but hasn't been tested and is not supported.

### Android (with termux)

The game can be runned through Termux app

## Compile

### Linux 

Install this package (available on most system delivered with apt or pacman)

- **make**
- **gcc**

Then run this command

```bash
make
```

### Windows

Install cygwin and download this package :

- **cygwin-devel** for termios support (check when a key is pressed)
- **gcc-core** to compile the c files
- **make** to build the project in one command

## Play a game

In your terminal, go to the bin directory and launch the file.

If you are on windows and want to run the game after building it, you can :

- Add the bin folder of cygwin to the path
- Copy the cygwin1.dll file from the bin folder of cygwin and paste it in the bin directory of the game

