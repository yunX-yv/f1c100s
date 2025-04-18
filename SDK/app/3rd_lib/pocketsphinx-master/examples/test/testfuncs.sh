# Utility functions and parameters for regression tests
# No longer very useful with CMake but whatever

# Predefined directories you may need
builddir="/home/hah/Downloads/pocketsphinx-master/examples"
sourcedir="/home/hah/Downloads/pocketsphinx-master"
tests=$sourcedir/test
data=$sourcedir/test/data
model=$sourcedir/model
programs=$builddir

# Automatically report failures on exit
failures=""
trap "fail $0" 4 6 7 10 11
trap "report_failures" 0

run_program() {
    program="$1"
    shift
    "$programs/$program" "$@"
}

debug_program() {
    program="$1"
    shift
    gdb --args "$programs/$program" "$@"
}

memcheck_program() {
    program="$1"
    shift
    valgrind --leak-check=full "$programs/$program" "$@"
}

pass() {
    title="$1"
    echo "$title PASSED"
}

fail() {
    title="$1"
    echo "$title FAILED"
    failures="$failures,$title"
}

compare_table() {
    title="$1"
    shift
    if perl "$tests/compare_table.pl" "$@" | grep SUCCESS >/dev/null 2>&1; then
	pass "$title"
    else
	fail "$title"
    fi 
}

report_failures() {
    if test x"$failures" = x; then
	echo "All sub-tests passed"
	exit 0
    else
	echo "Sub-tests failed:$failures" | sed -e 's/,/ /g'
	exit 1
    fi
}
