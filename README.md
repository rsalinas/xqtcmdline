# xqtcmdline

Adds bash-completion support to QCommandLineParser

The basic idea is to enrich the current system to have more semantics, that can be later used to autocomplete files/users/executables etc.

# Motivation

We live in a world code is plenty of replication.  Whenever we want to add a new option to a program, we typically must:

 * Add the code that parses the option.  Fortunately many people are already using one of the numerous libraries available out there. But still many prefer to do it completely by hand, or they start with something simple and then everything gets out of hands after a while.
 * Update the help. 
 * Update the bash completion script, if any. 
 * Update the manpage, if any.
  
 My thesis is that the first three elements can be derived from a single source if it is rich enough semantically, thus saving lots of time and grief and allowing for creative usages of this kind of "instrospection into programs".


# Compiling

It is a cmake project.

 * Clone the repository.
 * mkdir build
 * cd build
 * cmake ..

# Trying out the thing

Build it and then execute:

eval $(./xqtcmdline-test --setenv)

Voilà! 

It sets your PATH properly and sets a 'complete' rule in bash to complete the arguments of this sample.

Now you can start typing xqtcmdline-test and press Tab and enjoy it in your own applications. 

Example: 

```
$ xqtcmdline-test -
--chdir    --command  --debug    --file     --group    --pid      --sudo     --user     -v  
```

Note on the `--setenv`: 
```
$ xqtclp-build/xqtcmdline-test --setenv
complete -C "xqtcmdline-test -C" xqtcmdline-test;
export PATH=$PATH:/home/rsalinas/Sync/Projects/xqtclp-build;
```

# Future work

 * Port this to other major command line libraries.
 * Put these changes into the Qt library itself.
 * Try to contribute some of these improvements to the Qt mainstream. 
