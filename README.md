Circle
======

An IRC robot written in C. Primarily built to interact with IRCaD, but will be open for external usage too.

So called because C + IRC = CIRC and it's only 2 letters away from Circle. Plus also "circle" is a rather poetic way to describe the infinite loop at the heart of it, that keeps everything ticking over until a failure condition is encountered.

Workings
--------

Incoming signals will be passed to a handler, which the developer/user can use to do whatever they want. Signals will be numbered, for ease of processing.
Outgoing signals will make use of command bindings, such as irc\_privmsg(), or irc\_action() etc.

Running
-------

```shell
gcc -o circle *.c
./circle
```



