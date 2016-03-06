# Introduction

This directory contains unit tests for `hex` and `pkt`.

The unit tests depend on the *vtest* unit test framework available as a *git*
submodule in the `vtest/` directory. The following can be used to retrieve the files:

    $ git submodule init
    $ git submodule update

See `man git-submodule` for more information.


# Usage

    $ make
    [...]
    $ ./test_hex
    
    -----------------
    PASS: 666
    FAIL: 0
    -----------------
    $ ./test_pkt
    
    -> $abc#26
    <- +
    -> $#00
    <- +
    -> $OK#9a
    <- -
    -> $OK#9a
    <- +
    -----------------
    PASS: 3
    FAIL: 0
    -----------------

