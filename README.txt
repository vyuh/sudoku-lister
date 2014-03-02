The program gives a sorted list of all solutions that satisfy the
constraints of an input sudoku grid.

__EXAMPLES__
$ cc sud.c #single file having complete source
$ ./a.out 800000000003600000070090200050007000000045700000100030001000068008500010090000400
$ ./a.out #press control C to dump state and terminate program
$ date>>log; ./a.out;date >>log;tail log #time the computation

__USES__
it can be used in mathematical exploration of properties of sudoku.
or as a fast sudoku solver/generator.

__INPUT__
input grid is given as a string of 81 bytes as first argument.
each character represents a cell, in row order.
all characters except 1-9 are interpreted as empty cells.
if input string is smaller than 81 characters the rest of the
cells are assumed to be empty.

__OUTPUT__
the program lists sudokus in form of 81 character strings.
all of which satisfy the constraints expressed in the input string.

if input string means all empty cells.
it will list all possible sudoku grids

if no input string is given.
it will list all sudokus which have 123456789 as the first row.
ie. "123456789" is the default first argument.

__DUMPING STATE OF COMPUTATION__
if too many sudokus satisfy input constraints, then computation
can take a lot of time.
The keyboard interrupt signal (Ctrl+C) terminates the program after
dumping its state to a text file.

if the program is run without arguments with a dumpfile in the
current directory, it loads state from dumpfile and resumes a
previously suspended computation.

__ENUMERATING RESULTS__
the program shows number of sudokus listed in each session.
i.e. from resume/start to suspension/termination of computation.
you need to manually add them up if you want to count all listed
values in whole computation.
the counter will not overflow for any input string.

__PORTABILITY__
the source code and any generated dump files are usable on any
platform that has an ANSI C compiler with standard C library
implemented.

compile it with tiny c compiler. it is a good compiler.

__READING THE CODE__
the categorized source code is given in src/ directory.
src/extra/tut.c is a tutorial aimed at explaining the algorithm.


__ME__
this has took 4 years of my part time attention.
born out of my jealousy about a friend solving newspaper sudokus
faster than me. :D

please point out if it does not work as exhibited. i will correct it.

i am in search of a computer with enough resouces to undertake the task
of histogramming all sudokus.

or should i make a distributed system!?
but terabytes of data transfer?!
anyway.

webster15july@gmail.com
Fri Feb 28 00:38:23 IST 2014
pk
