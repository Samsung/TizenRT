#!/usr/bin/env perl

# FILE
#       cvs2fsm.pl  -
#
# FUNCTION
#
#       Take the csv state machine file and convert into fsm format
#       for fsmgen to munge.

# Trap errors as soon as possible
use warnings;
use strict;

# Extract the program name
my ($prog) = $0;
$prog =~ s/^.*[\\\/]//;

my ($outfile) = @ARGV;
$outfile =~ /(\S+).csv/;
$outfile = $1;

# Display help text if used without parameters
die <<EOT unless @ARGV;
Usage: $prog [file]
EOT

my @data;
my $last="State";
my $save;

while (<>) {
  # Change commas into tabs for the emacs fontifier. Except special FSM lines.
  if (!/__/) {
    # The park states don't have <any other event> so we see ,,,,
    # This screws us about change them.
    if (!/\w+,,,,,/ && (/\w+,,,,/ || /REPARKING_STO,,,,/)) {
      s/,,,,/ <any other event>, NULL, NULL, /g;
    }

    # The park states don't have the new state set if they jump to the
    # same state and some don't have action functions
    if (/(\w*PARK\w+),\w+,,\r/) {
      #print STDERR ("$1: $_");
      $save = $1;
      s/,,/,NULL,/g;
    }

    if (/(\w*PARK\w+),\w+,,\w+/) {
      # The park states don't have the new state set if they jump to the
      # same state.
      #print STDERR ("$1: $_");
      $save = $1;
      s/,,/,$save,/g;
    }

    # Get the first match
    /(\S[A-Z_0-9a-z]+)/;
    if ($1 && $1 ne $last) {
      push (@data, "----------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
      $last = $1;
    }
    # Change ',' into space
    s/,/ /g;
  }

  # Change template file to new one
  s/_H.fsm/_H.tpl/g;
  s/Sparse_/Sparse32_/g;

  # Remove the carriage return (^M) that Windows leaves on.
  # Remove double quotes
  s/\r//g; s/\"//g;

  # Push $_ (standard input) onto @data
  # This becomes an array in @data
  push (@data, $_);
}


my $line;
open NEWFILE, ">$outfile.fsm"  or  die "Can't write to $outfile.fsm: $!\n";
print STDERR "Writing $outfile.fsm\n";

select NEWFILE;
# Flush buffer on every write
$| = 1;
foreach $line (@data) {
  print "$line";
}

close NEWFILE;
