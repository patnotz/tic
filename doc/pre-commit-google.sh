#!/bin/sh

do_uncrustify_check=true
do_cpplint_check=false
show_uncrustify_diff=true
show_cpplint_output=true
proj_root_rel=$(git rev-parse --show-cdup)
uncrustify_cfg=${proj_root_rel}doc/google.cfg
cpplint_py="${proj_root_rel}doc/cpplint.py --filter=-build/header_guard,-runtime/int,-readability/streams"

skip_cpp_format_check() {
	 ext=${1##*.}
	 cpp_ext="C cpp cxx H h hpp hxx"
	 for e in $cpp_ext ; do
		  if test "$ext" = "$e" ; then
				return 1
		  fi
	 done
	 return 0
}

check_format() {
	 working_file=$1
	 actual_file=$2
    tmpfile=$working_file.uncrustified
	 uncrustify_cmd="uncrustify -c $uncrustify_cfg -l CPP"
	 $uncrustify_cmd -f $working_file -o $tmpfile 2>/dev/null
	 diff -q $working_file $tmpfile >/dev/null 2>&1 
	 crusty=$?
	 if test 0 -eq $crusty ; then
		  echo "    uncrustify check: clean."
	 else
		  echo "    uncrustify check: dirty - please uncrustify:"
		  echo "        $uncrustify_cmd $actual_file"
		  if $show_uncrustify_diff ; then
				diff $working_file $tmpfile
		  fi
    fi
	 rm -f $tmpfile
	 return $crusty
}

check_lint() {
	 working_file=$1
	 actual_file=$2
	 $cpplint_py $working_file >/dev/null 2>&1
	 linty=$?
	 if test 0 -eq $linty ; then
		  echo "    cpplint.py check: clean."
	 else
		  echo "    cpplint.py check: dirty - please delint:"
		  echo "        $cpplint_py $actual_file"
		  if $show_cpplint_output ; then
				$cpplint_py $working_file
		  fi
    fi
	 return $linty
}

check_file() {
	 file=$1
	 echo "pre-commit-google.sh: checking $file..."

	 # Be careful of partial commits -- work on the files in the git index
	 ext=${1##*.} # preserve the extension
	 actual_file=$proj_root_rel$file
	 working_file=${actual_file%.*}~pre-commit-google.$ext
	 git show :$actual_file > $working_file

	 file_err=0

	 if $do_uncrustify_check ; then
		  check_format $working_file $actual_file
		  if test 0 -ne $? ; then
				file_err=1
		  fi
	 fi
	 if $do_cpplint_check ; then
		  check_lint $working_file $actual_file
		  if test 0 -ne $? ; then
				file_err=1
		  fi
	 fi
	 
	 if test 1 -eq $file_err ; then
	     # If the user is committing only parts of the working copy
	     # warn them because the uncrustify or cpplint output may
        # be inconsistent with their working copy
		  diff -q $working_file $actual_file >/dev/null 2>&1 
		  partial_commit=$?
		  if test 0 -ne $partial_commit ; then
				echo
				echo "NOTICE: you're only committing some of the the changes in $actual_file"
				echo "The output from uncrustify or cpplint.py may be different from"
				echo "that of your working copy"
				echo
		  fi
	 fi
	 rm -f $working_file
	 return $file_err
}

cmt_err=0
for file in $(git diff --cached --name-only); do

	 if skip_cpp_format_check $file ; then 
		  echo "pre-commit-google.sh: skipping non-C++ file $file"
	 else
		  check_file $file
		  file_err=$?
		  if test 0 -ne $file_err ; then
				cmt_err=1
		  fi
	 fi
done
exit $cmt_err
