#!/usr/bin/perl -w

# FILE
#       fsmgen.pl  -
#
# FUNCTION
#
#       Take the state machine file and convert into definition and
#       constant header files.

use Getopt::Std;
use strict;

use vars qw($state_max $event_max $action_max
            $state_len $event_len $action_len
            %states @states %events @events %actions @actions
            %fsm_files $template_file $file $lmp_def_file
            %fsm_vars %subs $forcount @forlist @markerstack %markers
            $skip_empty $opt_c $opt_p $opt_v $opt_w
            $fsm_event_base $commentstart $commentend
            %offsets $offset_len);

$state_max = $state_len = 0;
$event_max = $event_len = 0;
$action_max = $action_len = 0;
$offset_len = 0;
@states = @events = @actions = ();
%states = %events = %actions = %fsm_vars = ();
$opt_c = $opt_p = $opt_v = $opt_w = 0;
$fsm_event_base = 0;

getopts("cpvw") or exit 1;

# Get input file
my $infile = shift;
my $dir = shift;

my $commentstyle = undef;

my $lm_master_unused = 0;
my $lm_master_unused_str = 0;

if ($infile) {
  $infile .= ".fsm"  if ! -f $infile && -f "$infile.fsm";
  if (@ARGV || ! -f $infile) {
    die "Usage: $0 $infile\n";
  }
}else {
  die "Usage: $0 fsm_database_file.fsm\n";
}

if (!$dir) {
  $dir = ".";
}


#
# Scan the input .csv file.
#

open FILE, $infile  or  die "Can't read $infile: $!\n";

while (<FILE>) {
    my @line = ();
    my $cur = '';

    # Skip comment lines
    next if (/^\#/);
    next if (/-----/);

    # Don't want the <any other event>
    next if (/<any other event>/);

    # Strip the input line into its seperate parts and push them
    # individually ont line. Hence will give,  $line[0]=state,
    # $line[1]=event etc..
    while ($_ !~ /^\s*$/) {
      if (/__FSM/) {
        push @line, $_;
        last;
      } elsif (/(\S+)/) {
        # Don't want the <any other event>
        last if ($1 =~ /[<>]/);
        push @line, $1;
      }
      $_ = $';
    }

    # The first line is special and we expect 'State'
    if ($. == 1) {
        if ($line[0] ne 'State') {
            warn "fsmgen.pl WARNING: unexpected header at $infile line $., was '$line[0]' continuing.\n";
        }
    } elsif (!$line[0] || $line[0] eq '') {
        next;
    } elsif ($line[0] =~ /^_/) {
        $_ = $line[0];
        if (/^__FSM_COMMENTS_C/) {
            $commentstyle = "C";
            warn "Using C-style comments" if $opt_v;
        } elsif (/^__FSM_(START|END|SRC_LIST|COMMENTS)/) {
            warn "Ignoring $_\n"  if $opt_v;
        }
        elsif (/__FSM_VAR\(\s*(\S+)\s*,\s*(\S+)\)/) {
            $fsm_vars{lc $1} = $2;
            warn "Setting FSM variable \"" . lc($1) . "\" to \"$2\"\n"  if $opt_v;
        } elsif (/__FSM_TEMPLATE_FILE\((.*)\)/) {
            $template_file = $1;
            $template_file =~ s/\\/\//g;
            warn "Setting FSM template file $template_file\n"  if $opt_v;
            warn "fsmgen.pl WARNING: template file does not exist: $template_file at $infile line $., continuing.\n"  unless -e $template_file;
        } elsif (/__FSM_OUTPUT_FILE\((\S+),\s*(\S+),\s*(\S+),\s*(\S+)\)/) {
            $fsm_files{$3} = [ $template_file, "$1", "$2", "$4", $commentstyle ];
            warn "Setting FSM files for: $3 to: \"$fsm_files{$3}[0]\", \"$fsm_files{$3}[1]\", \"$fsm_files{$3}[2]\", \"$fsm_files{$3}[3]\", \"$fsm_files{$3}[4]\"\n"
              if $opt_v;
        } elsif (/__FSM_LMP_DEF_FILE\((\S+)\)/) {
            $lmp_def_file = $1;
            $lmp_def_file =~ s/\\/\//g;
            $lmp_def_file =~ s/.xls/.csv/g;
            warn "Setting LMP def file to: \"$lmp_def_file\"\n"
              if $opt_v;
        } else {
          print "**$line[0]**\n";
          print "**$_**\n";
          warn "fsmgen.pl WARNING: skipping FSM information: $_ at $infile line $.,continuing.\n";
        }
    } else {
        if (!exists $states{$line[0]}) {
            push @states, $line[0];
            $state_max++;
            $state_len = length $line[0]  if length $line[0] > $state_len;
            $states{$line[0]} = { };
        }
        # This holds the state information i.e
        # current_state, event, next_state, action, comment stored in the
        # hash as $state{curstate}{event} hence will look something like
        #    {S1} {E1} = [NS1, A1, comment]
        #    {S1} {E2} = [NS2, A2, comment]
        #    etc..
        # $states{curstate}{event} = [ nextstate, action, comment ]
        if ($line[1]) {
          $states{$line[0]}{$line[1]} = [ $line[2], $line[3], $line[4] ];
        } else {
          $states{$line[0]}{0} = [];
        }

        if ($line[1] && length $line[1] && !exists $events{$line[1]}) {
            next if $line[1] =~ /^\s*(\<.*\>)\s*$/;
            # The event is stored in a hash as event_name,number.
            push @events, $line[1];
            $events{$line[1]} = $event_max;
            $event_max++;
            $event_len = length $line[1]  if length $line[1] > $event_len;
        }
        if ($line[3] && length $line[3] && !exists $actions{$line[3]}) {
            next if $line[3] eq 'NULL'; # handled specially
            push @actions, $line[3];
            $actions{$line[3]} = $action_max;
            $action_max++;
            $action_len = length $line[3]  if length $line[3] > $action_len;
        }
    }
}

close FILE;

#
# If we have a LMP definition file open it and read the predefined
# event number into the events hash.
#
if ($lmp_def_file) {
  my $item;
  my $used_events=0;
  my $base;
  my $group;
  my %groups;
  open FILE1, $lmp_def_file  or  die "Can't read $lmp_def_file: $!\n";
  while (<FILE1>) {
    # Have we been given a minimum number to start at
    if (/__FSM_EVENT_BASE\((\S+)\)/) {
      $fsm_event_base = $1;
      # Add the fsm_event_base to the events
      foreach $item (@events) {
        $events{$item} += $1;
        $used_events++;
      }

      warn "FSM_EVENT_BASE set to $fsm_event_base\n" if $opt_v;
    } elsif (/__FSM_EVENT_DEFGROUP\((\S+)\)/) {
      die "Already in group $group" if defined($group);
      $group = $1;
      @{$groups{$1}} = (); # Allow empty groups
    } elsif (/__FSM_EVENT_ENDGROUP/) {
      die "Not in group" unless defined($group);
      $group = undef;
    } elsif (/__FSM_EVENT_VALUE\((\S+),\s*(\S+)\)/) {
      if (defined($group)) {
        push @{$groups{$group}}, [$1, $2];
      } else {
        $events{$1} = $2;
      }
    } elsif (/__FSM_EVENT_GROUP\((\S+),\s*(\S+),\s*(\S+)\)/) {
      # group, prefix, offset
      die "No such group: $1" unless defined($groups{$1});
      my $name = $2 . $1 . "_OFFSET";
      $offsets{$name} = $3;
      $offset_len = length $name unless $offset_len >= length $name;
      my $max = 0;
      foreach my $entry (@{$groups{$1}}) {
        $events{$2 . $entry->[0]} = $entry->[1] + $3;
        $max = $entry->[1] if $entry->[1] > $max;
      }
      # only define the group max if there's something in the group
      if (scalar @{$groups{$1}} > 0) {
        $offsets{$2 . $1 . "_MAX"} = $max;
      }
    }
  }
  # Value zero is not used and is not in the definitions file
  # so add it here. This means the original output from the xls
  # file stays the same
  $events{NOT_USED} = 0;
  close FILE1;

  # Now compress the number range as we have placed holes in it
  # by using the definitions file. We know this must be ordered
  # so look for the first number >= fsm_event_base and less than
  # fsm_event_base + used_events
  $base = $fsm_event_base;
  foreach $item (@events) {
    if (($events{$item} >= $fsm_event_base) &&
        ($events{$item} <= ($fsm_event_base + $used_events))) {
      $events{$item} = $base;
      $base++;
    }
  }
  my $last = undef;
  foreach $item (sort values %events) {
    die "Duplicate event: $item" if defined($last) && $last == $item;
    $last = $item;
  }
}

my $bad;                        # tee hee
while ((my $state, my $sref) = each %states) {
  while ((my $event, my $eref) = each %$sref) {
    if ($eref->[0] && $states{$eref->[0]} && length($eref->[0]) && !defined($states{$eref->[0]})) {
      warn "No such state: $eref->[0] in transition from $state via $event\n";
      $bad++;
    }
  }
}
exit(1) if $bad;

# for recursive use
sub handle_line ($);

sub handle_for (@) {
    my $fortype = shift;
    my ($item, $line);
    my $i = 0;

    chomp $fortype;

    if ($fortype =~ /__FSM_FOR_STATES/) {
        foreach $item (@states) {
            $subs{__FSM_STATE_NAME} = $item;
            $subs{__FSM_STATE_NAME_UPPER} = uc $item;
            $subs{__FSM_STATE_NAME_LOWER} = lc $item;
            $subs{__FSM_STATE_NO} = $i++;
            $subs{__FSM_PAD_STATE} = ' ' x ($state_len - length $item);
            foreach $line (@_) {
                handle_line $line;
            }
        }
        delete $subs{__FSM_STATE_NAME};
        delete $subs{__FSM_STATE_NAME_UPPER};
        delete $subs{__FSM_STATE_NAME_LOWER};
        delete $subs{__FSM_STATE_NO};
        delete $subs{__FSM_PAD_STATE};
    } elsif ($fortype =~ /__FSM_FOR_EVENTS(_IN_CLASS\((.*)\))?/) {
        defined $1 and length $1  and
          defined $2 and length $2 and return;  # don't know what to do yet.
        foreach $item (@events) {
            my $state = $subs{__FSM_STATE_NAME};
            my $stateref = defined($state) ? $states{$state}{$item} : undef;
            $skip_empty = 0;
            if (defined $stateref) {
                my $nextstate = $stateref->[0];
                my $action = $stateref->[1];
                $subs{__FSM_NEXT_STATE} = $nextstate;
                if (defined $action) {
                    $subs{__FSM_ACTION_NAME} = $action;
                    $subs{__FSM_ACTION_NAME_UPPER} = uc $action;
                    $subs{__FSM_ACTION_NAME_LOWER} = lc $action;
                    # $subs{__FSM_ACTION_NO} = $actions{$action};
                    $subs{__FSM_PAD_ACTION} =
                      ' ' x ($action_len - length $action);
                    if ($opt_c && defined $stateref->[2]) {
                        print "    /* $stateref->[2] */\n"
                    }
                }
            }
            $subs{__FSM_EVENT_NAME} = $item;
            $subs{__FSM_EVENT_NAME_UPPER} = uc $item;
            $subs{__FSM_EVENT_NAME_LOWER} = lc $item;
            $subs{__FSM_EVENT_NO} = $events{$item};
            $subs{__FSM_PAD_EVENT} = ' ' x ($event_len - length $item);
            foreach $line (@_) {
                handle_line $line;
            }
            delete $subs{__FSM_NEXT_STATE};
            delete $subs{__FSM_ACTION_NAME};
            delete $subs{__FSM_ACTION_NAME_UPPER};
            delete $subs{__FSM_ACTION_NAME_LOWER};
            # delete $subs{__FSM_ACTION_NO};
            delete $subs{__FSM_PAD_ACTION};
        }
        $skip_empty = 0;
        delete $subs{__FSM_EVENT_NAME};
        delete $subs{__FSM_EVENT_NAME_UPPER};
        delete $subs{__FSM_EVENT_NAME_LOWER};
        delete $subs{__FSM_EVENT_NO};
        delete $subs{__FSM_PAD_EVENT};
    } elsif ($fortype =~ /__FSM_FOR_OFFSETS/) {
        foreach $item (sort keys %offsets) {
            $subs{__FSM_OFFSET_NAME} = $item;
            $subs{__FSM_OFFSET_NAME_UPPER} = uc $item;
            $subs{__FSM_OFFSET_NAME_LOWER} = lc $item;
            $subs{__FSM_OFFSET_NO} = $offsets{$item};
            $subs{__FSM_PAD_OFFSET} = ' ' x ($offset_len - length $item);
            foreach $line (@_) {
                handle_line $line;
            }
        }
        delete $subs{__FSM_OFFSET_NAME};
        delete $subs{__FSM_OFFSET_NAME_UPPER};
        delete $subs{__FSM_OFFSET_NAME_LOWER};
        delete $subs{__FSM_OFFSET_NO};
        delete $subs{__FSM_PAD_OFFSET};
        # Generate Debugging Event Strings
    } elsif ($fortype =~ /__FSM_FOR_EVENT_LOGS/) {
        my $last = 0;
        # length $1  and  length $2 and return; # don't know what to do yet.
        # We never use event zero

        # Sort Events into numeric order
        foreach $item (keys %events) {
          if (!defined $item) {
            warn "Key value undefined in events.\n";
          }
          if (!defined $events{$item}) {
            warn  "No value for event $item.\n";
          }
        }
        foreach $item (sort {$events{$a} <=> $events{$b}} keys %events ) {
          # Fill in any blanks
          if (($last + 1) < $events{$item}) {
            print "    (const char *) lm_master_ev_not_used,\n"
              x ($events{$item} - ($last + 1));
          }
          # Update where we got to
          $last = $events{$item};
          $skip_empty = 0;
          $subs{__FSM_LOG_EVENT_NAME_LOWER} = lc $item;
          $subs{__FSM_LOG_EVENT_NAME_UPPER} = uc $item;
          foreach $line (@_) {
            handle_line $line;
          }
        }
        $skip_empty = 0;
        delete $subs{__FSM_LOG_EVENT_NAME_UPPER};
    } elsif ($fortype =~ /__FSM_FOR_EVENT_STR/) {
        my $last = 0;
        # length $1  and  length $2 and return; # don't know what to do yet.
        # We never use event zero

        # Sort Events into numeric order
        foreach $item (keys %events) {
          if (!defined $item) {
            warn "Key value undefined in events.\n";
          }
          if (!defined $events{$item}) {
            warn  "No value for event $item.\n";
          }
        }
        foreach $item (sort {$events{$a} <=> $events{$b}} keys %events ) {
          # Fill in any blanks
          if ($last+1 != $events{$item}) {
            if ($lm_master_unused_str == 0 && ($events{$item} - $last-1) > 0) {
              print "static const char lm_master_ev_not_used[] = \"LM_MASTER_EV_NOT_USED\";\n";
              $lm_master_unused_str = 1;
            }
          }
          # Update where we got to
          $last = $events{$item};
          $skip_empty = 0;
          $subs{__FSM_LOG_EVENT_NAME_LOWER} = lc $item;
          $subs{__FSM_LOG_EVENT_NAME_UPPER} = uc $item;
          foreach $line (@_) {
            handle_line $line;
          }
        }
        $skip_empty = 0;
        delete $subs{__FSM_LOG_EVENT_NAME_UPPER};
    } elsif ($fortype =~ /__FSM_FOR_ACTIONS/) {
        foreach $item (@actions) {
            $subs{__FSM_ACTION_NAME} = $item;
            $subs{__FSM_ACTION_NAME_UPPER} = uc $item;
            if ($opt_p) {
                $subs{__FSM_ACTION_NAME_LOWER} = $item;
            } else {
                $subs{__FSM_ACTION_NAME_LOWER} = lc $item;
            }
            $subs{__FSM_ACTION_NO} = $i++;
            $subs{__FSM_PAD_ACTION} = ' ' x ($action_len - length $item);
            foreach $line (@_) {
                handle_line $line;
            }
        }
        delete $subs{__FSM_ACTION_NAME};
        delete $subs{__FSM_ACTION_NAME_UPPER};
        delete $subs{__FSM_ACTION_NAME_LOWER};
        delete $subs{__FSM_ACTION_NO};
        delete $subs{__FSM_PAD_ACTION};
    } else {
        die "For loop $fortype not handled.\n";
    }
}

@forlist = @markerstack = ();
$forcount = 0;

sub unescape {
  my($esc) = @_;

  return chr($1*84 + $2*8 + $3) if ($esc =~ /^([0-7])([0-7])([0-7])/);
  return chr(hex($1)) if ($esc =~ /^x([0-9a-fA-F][0-9a-fA-F])/);
  return "\n" if ($esc eq "n");
  return "\r" if ($esc eq "r");
  return "\t" if ($esc eq "t");
  die "Unknown escape sequence '\\$esc' at $file line $.\n";
}

sub handle_line ($) {
    local $_ = shift;

    if ($skip_empty && !defined $subs{__FSM_NEXT_STATE}) {
        return 1;
    } elsif (@markerstack) {
        # Set when we have handled a marker from the template file
        # and need to dump the text up to the end marker in the
        # current file.
        if (/(.*__FSM_END_MARKER)(.*)/) {
            my $intro = $1;
            if (! m%/\*.*\*/%) {
                $_ = "$commentstart}}$intro($subs{__FSM_NAME_UPPER}, ${markerstack[0]} $commentend\n";
            }
            print;
            pop @markerstack;
        }
    } elsif (/^__FSM_FOR_/) {
        $forcount++;
        push @forlist, $_;
    } elsif ($forcount) {
        # Set when we are reading a for loop and looking for the
        # end line.
        $forcount-- if /^__FSM_END_FOR$/;

        if ($forcount == 0) {
            # Careful in case we need to handle nested for loops.
            my @oldlist = @forlist;
            @forlist = ();
            handle_for @oldlist;
        } else {
            push @forlist, $_;
        }
    } elsif (/^__FSM_END_FOR/) {
        die "Unhandled FSM_END_FOR:\n$_  at $file line $.,\n  fatal.\n";
    } elsif (/(.*__FSM_MARKER\()((?:|.*\s*,\s*)(\S[^,]+)\))/) {
        my $intro = $1;
        my $trailer = $2;
        my $type = $3;
        if (! m%/\*.*\*/%) {
            $_ = "$commentstart"."{{${intro}$subs{__FSM_NAME_UPPER}, $trailer $commentend\n";
        }
        print;
        foreach (@{$markers{$type}}) {
            handle_line $_;
        }
        push @markerstack, $trailer;
    } elsif (/__FSM_(|END_)MARKER/) {
        die "Unhandled FSM marker:\n$_  at $file line $.,\n  fatal.\n";
    } elsif (/^__FSM_END$/) {
        return undef;
    } elsif (/^__FSM_SKIP_EMPTY_TRANSITIONS$/) {
        $skip_empty = 1;
    } else {
        my ($key, $val);
        # Do longest substitution first.
        # This can be made more efficent by burying %subs in
        # wrappers.
        foreach $key (sort { length($b) <=> length($a) } keys %subs) {
            die "undefined substitution key" unless defined $key;
            $val = $subs{$key};
            die "undefined substitution for $key" unless defined $val;
            s/$key/$val/g;
        }
        s/__FSM_NO_NEWLINE[\r\n]*$//;
        s/__FSM_VAR\(([^\)]+)\)/exists(${fsm_vars{lc $1}})?${fsm_vars{lc $1}}:''/eg;
        if (my ($var, $val) = (/^__FSM_SET_VAR\(\s*(\S+)\s*,\s*(.*?)\s*\)$/)) {
          $var = lc $var;
          if ($val =~ s/^"(.*)"$/$1/) {
            $val =~ s{\\([0-7][0-7][0-7]|x[0-9A-Fa-f][0-9A-Fa-f]|.)}
              {unescape($1)}ge;
          }
          $fsm_vars{$var} = $val;
        } elsif (/^__FSM_INC_VAR\(\s*(\S+)\s*\)$/) {
          die "Unknown FSM variable $1 at $file line $.,\nfatal.\n"
            if (!defined($fsm_vars{lc $1}));
          $fsm_vars{lc $1}++;
        } else {
          print;
        }
    }

    1;
}

my $filetype;

%subs = ( __FSM_TOOL_NAME => "fsmgen.pl",
          __FSM_TOOL_VERSION => "2D",
          __FSM_NUM_STATES => $state_max,
          __FSM_NUM_EVENTS => $event_max,
          __FSM_NUM_ACTIONS => $action_max,
);

# no obvious way of getting this...
my $name = $infile;
$name =~ s%.*/%%;
if ($name =~ /(.*)_fsm/) {
    $name = $1;
} else {
    $name = $fsm_vars{stateprefix};
    $name =~ s/_ST_$//;
}
$subs{__FSM_NAME} = uc $name;
$subs{__FSM_NAME_UPPER} = uc $name;
$subs{__FSM_NAME_LOWER} = lc $name;

for $filetype (keys %fsm_files) {

    my $fileref = $fsm_files{$filetype};
    my $template = 0;
    $file = $fileref->[1] .  $fileref->[3] . $fileref->[2];

    my $tempfile = $fileref->[0];
    my $commentstyle = $fileref->[4];

    if (!defined($commentstyle)) {
        die "Comment style not set for file $file";
    } elsif ($commentstyle eq "C") {
        $commentstart = '/*';
        $commentend = '*/';
    } else {
        die "Comment style not set for file $file";
    }

    # Scan the template file for markers, whether we are going
    # to be using it for output or not.  These bits need substituting
    # in the target file even if it already exists.
    %markers = ();
    open FILE, $tempfile  or  die  "Can't read template file $tempfile: $!\n";
    while (<FILE>) {
        s/\r\n/\n/;
        if (/__FSM_MARKER\((\S+)\)/) {
            my $cur = $1;
            my @lines;
            while (<FILE>) {
                s/\r\n/\n/;
                last if /__FSM_END_MARKER/;
                push @lines, $_;
            }
            $markers{$cur} = [ @lines ];
        }
    }
    close FILE;

    $subs{__FSM_OUTPUT_FILE_UPPER} = uc $fileref->[1];
    $subs{__FSM_OUTPUT_FILE_LOWER} = lc $fileref->[1];

#    my $status;
#    $status = system ("cp $dir/$file $dir/$file.old");

    open NEWFILE, ">$dir/$file"  or  die "Can't write to $dir/$file: $!\n";
    select NEWFILE;
    $| = 1;

    if (!-f "$dir/$file.old") {
      # Use template file.
      $file = $fileref->[0];
      $template = 1;
    } else {
      $file = "$dir/$file.old";
    }

    open FILE, "$file"  or  die "Can't read $file.old: $!\n";

    if ($template) {
        while (<FILE>) {
            s/\r\n/\n/ if $template;
            last if /^__FSM_START$/;
        }
    }

    while (<FILE>) {
        s/\r\n/\n/ if $template;
        last unless handle_line $_;
    }

    close FILE;
    select STDOUT;
    close NEWFILE;
}
