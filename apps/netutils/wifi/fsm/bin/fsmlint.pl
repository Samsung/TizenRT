#!/usr/bin/perl -w

# FILE
#       fsmlint.pl
#
# FUNCTION
#
#       Check the state machine file for undesirable edge transitions.
#       Specifically, we look for ACK edges to any state that has LMP_
#       edges out of it, and ensure that the original state handles
#       the same LMP_ messages.
#
#       The '-e' command line option adds a list of uncaught events
#       to the output.

use strict;
use Getopt::Std;

use vars qw($opt_e);

getopts("e") or exit 1;

my $infile = shift;
die "Usage: $0 [-e] <fsm-file>\n" unless defined $infile;

open FILE, $infile or die "Can't read $infile: $!\n";

my %states;

while (<FILE>)
{
    next if /-----/;
    next if /<any other event>/;
    next if /^\s*$/;

    my ($initial_state, $event, $final_state, $function) = split;

    if ($. == 1)
    {
        warn "fsmlint: WARNING: unexpected header at $infile line 1\n"
            if $initial_state ne 'State';
        next;
    }

    if ($initial_state !~ /^__FSM/)
    {
        if (!exists $states{$initial_state})
        {
            # New state.  Yippee.
            $states{$initial_state} = {};
        }
        if (exists $states{$initial_state}{$event})
        {
            warn "fsmlint: WARNING: repeated transition from $initial_state on $event in $infile at line $.\nFirst occurrance on line " . $states{$initial_state}{$event}{line_no} . "\n";
        }
        else
        {
            # Save this data in the state table
            $states{$initial_state}{$event} = {
                line_no => $.,
                final_state => $final_state,
                function => $function
            };
        }
    }
}

close($infile);

# Now pass through the state machine, looking for acks
# (LMMSG_LMP_DELIVERED messages).

foreach my $state (keys %states)
{
    if (exists $states{$state}{LMMSG_LMP_DELIVERED})
    {
        # Collect up the LMP_ edges out of the final state here, and
        # check that our initial state has transitions for all of
        # them.
        my $target = $states{$state}{LMMSG_LMP_DELIVERED}{final_state};
        my @events = grep /^LMP_/, keys %{$states{$target}};
        my @missing = grep !exists $states{$state}{$_}, @events;
        if (@missing)
        {
            warn "fsmlint: WARNING: unhandled: $state transitions on LMMSG_LMP_DELIVERED to $target\n";
            if ($opt_e)
            {
                warn "fsmlint:   and does not catch the following events:\n";
                foreach my $event (@missing)
                {
                    warn "fsmlint:      $event\n";
                }
            }
        }
        # Check for same edge for ACK and TIMEOUT
        if ( !exists $states{$state}{LMP_TIMEOUT}{final_state} )
        {
            warn "fsmlint: WARNING: notimeout: $state transitions on LMMSG_LMP_DELIVERED but has no edge for LMP_TIMEOUT\n";
        }
        else
        {
            my $target2 = $states{$state}{LMP_TIMEOUT}{final_state};
            my $func = $states{$state}{LMMSG_LMP_DELIVERED}{function};
            my $func2 = $states{$state}{LMP_TIMEOUT}{function};
            #if ( "$target2" eq "$target" )
            if ( 1 )
            {
                # Two edges go to same state; check the perform same action
                if ( "$func" ne "$func2" || "$target2" ne "$target")
                {
                    warn "fsmlint: WARNING: function1: $state transitions on LMMSG_LMP_DELIVERED to $target with $func\n";
                    warn "fsmlint: WARNING: function2: $state transitions also on    LMP_TIMEOUT to $target2 with $func2\n";
                }
                # Check for solely ACK and TIMEOUT edges; implies a pointless state
                my $count = scalar( keys %{$states{$state}} );
                if ( 2 == $count )
                {
                    warn "fsmlint: WARNING: dead time: $state has only LMP_TIMEOUT and LMMSG_LMP_DELIVERED edges\n";
                }
            }
        }
    }
}
