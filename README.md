st
==


> simulation toolkit (st), yet another

This is my personal general propose simulation toolkit.  C APIs
(application programming interfaces) and some scripts.  Tools to write
computer programs that run in operator-in-the-loop real-time or not.  It's
just starting.  If anyone uses this code, Coool.  If not that's okay too.
I still need it.



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

Put *quickbuild* in your PATH.



#### GitHub Flavored Markdown

+ https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet
+ https://help.github.com/articles/github-flavored-markdown/

### open-axiom

open-axiom is used to generate some of the C code.
It is used to compute coefficients for taking derivatives
numerically and other C code.

  >```sudo apt-get install open-axiom```



# st Homepage

https://github.com/lanceman2/st


### st Package Software License

    Copyright (C) 2015  Lance Arsenault

    This package is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This package is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
