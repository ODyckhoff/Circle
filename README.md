cbot
====

An IRC robot written in C. Primarily built to interact with IRCaD, but will be open for external usage too.

Workings
--------

Incoming signals will be passed to a handler, which the developer/user can use to do whatever they want. Signals will be numbered, for ease of processing.

Outgoing signals will make use of command bindings, such as privmsg(), or action() etc.
