Mozi
====

The Name
--------

[Mozi][lnk_mozi] was a renowned Chinese philosopher and craftsman, and
his name sounds very good for a programming project.


The Project
-----------

This project serves as a continuation and successor to my previous C++
utility project, [Nvwa][lnk_nvwa], which is showing its age.  However, I
do not want to make dramatic changes to its code and break backward
compatibility, so a new project is more appropriate for new stuff.

In creating Mozi, I am also taking this opportunity to leave behind some
bad habits, such as the use of prefix-less macros and reserved
identifiers (e.g. `_NOTHREADS` and `_M_line`).  Overall, Mozi will have
a more modern style.


Base C++ Standard
-----------------

This project primarily uses the C++17 standard.  I need `string_view`s,
fold expressions, inline variables, and constexpr if statements.  They
are necessary for code readability and performance.

Where appropriate, support for newer C++ standards is added.


Contents
--------

Mozi is supposed to evolve over time.  Initially, manual static
reflection for `enum`s and `struct`s is included, as well as macros that
are necessary in such code generation usage.  More will be coming later.


[lnk_mozi]: https://en.wikipedia.org/wiki/Mozi
[lnk_nvwa]: https://github.com/adah1972/nvwa


<!--
vim:autoindent:expandtab:formatoptions=tcqlm:textwidth=72:
-->
