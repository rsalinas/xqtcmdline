# xqtcmdline

Adds bash-completion support to QCommandLineParser

The basic idea is to enrich the current system to have more semantics, that can be later used to autocomplete files/users/executables etc.

# Compiling

It is a cmake project.

 * Clone the repository.
 * mkdir build
 * cd build
 * cmake ..

# Trying out the thing

Build it and then execute:

eval `./xqtcmdline-test --setenv`

Voilà! 

It sets your PATH properly and sets a 'complete' rule in bash to complete the arguments of this sample.

Now you can start typing xqtcmdline-test and press Tab and enjoy it in your own applications. 

# Future work

 * Put these changes into the Qt library itself.
 * Try to contribute some of these improvements to the Qt mainstream. 
