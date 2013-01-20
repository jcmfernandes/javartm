#!/bin/bash

# this is specific to my machine, modify this accordingly
SDE_DIR=~/projects/intel-htm/sde-bdw-external-5.38.0-2013-01-03-lin/

$SDE_DIR/sde64 -rtm_mode full -rtm_debug_log 1 -rtm_stats 1 -rtm_stats_call_stack 1 -rtm_cache_set_size 128 -- java javartm.Test
