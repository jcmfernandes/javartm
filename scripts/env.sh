#!/bin/bash

# get our path
# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in
SCRIPT_DIR="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

PROJECT_DIR=$SCRIPT_DIR/../
LIBS=`ls -d -1 $PROJECT_DIR/lib/* | paste -sd ":"`
CLASSES_DIR=$PROJECT_DIR/build/classes/
JNI_DIR=$PROJECT_DIR/build/jni/javartm/

export CLASSPATH=$CLASSES_DIR:$LIBS:$CLASSPATH
export LD_LIBRARY_PATH=$JNI_DIR:$LD_LIBRARY_PATH

# put custom gcc 4.8 git build in path
# this is specific to my machine, modify this accordingly if needed
export PATH=~/projects/intel-htm/gcc/bin/:$PATH
