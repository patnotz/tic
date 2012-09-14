# tic
*Transitive Include Checker*

## About 


## Build
1. Create a build directory
    <pre><code>
    $ mkdir build
    $ cd build
    </code></pre>
1. Copy and tailor your CMake script
    <pre><code>
    $ cp ../doc/do-cmake
    $ emacs do-cmake # tweak your settings
	 # or, just
    # ln -s ../doc/do-cmake
    </code></pre>
1. Build it
    <pre><code>
    $ ./do-cmake # only necessary the first time
    $ make
    </code></pre>
1. Uncrustify it
    <pre><code>
    $ uncrustify -c ../doc/google.cfg -l CPP --replace --mtime *.cpp *.hpp
    </code></pre>
1. Run it
    <pre><code>
    $ ./util
    </code></pre>
1. Clean it
    <pre><code>
    $ git clean -fdx -e .project -e .cproject -e build
    </code></pre>
1. Tag it
    <pre><code>
    $ git tag -a -m "Version 0.1" v0.1 HEAD
    </code></pre>

## TODO
 - everything!
 - make a library of handy things
 - Fix version-gen.sh on rebuilds
 - Doxygen and github-pages

