st
==


> simulation toolkit (st), yet another

This is my personal general propose simulation toolkit.  C APIs (with C++
interfaces) (application programming interfaces) and some scripts.  Tools
to write computer programs that run in operator-in-the-loop real-time or
not.  It's a just starting.



## Building with quickbuild

Put quickbuild in your PATH (see below).
From the top st source directory tree run something like:

```quickbuild --prefix=DIR && make -j4 install```

Running quickbuild will generate make files named GNUmakefile.



Ports
-----

GNU/Linux is the starting platform.



st Dependencies
----------------

### marked

We get the source by running:

```git clone https://github.com/chjj/marked.git```

Put *marked* in your PATH.


### quickbuild

Get quickbuild source by running:

  >```
  >git clone https://github.com/lanceman2/quickbuild.git
  >cd quickbuild
  >make
  >```

Put *quickbuild* in you PATH.



#### GitHub Flavored Markdown

+ https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet
+ https://help.github.com/articles/github-flavored-markdown/

