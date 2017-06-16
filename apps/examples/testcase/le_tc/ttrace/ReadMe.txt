Readme -> ttrace_tc_main
~~~~~~~~~~~~~~~~~~~~

This is a sample application to perform T-trace operations.

Steps:
1. Run ttrace executable($ ttrace -s apps libs lock ipc)
2. Run ttrace sample application($ ttrace_tc_sample)
3. Finish ttrace executable($ ttrace -f, $ ttrace -p)

List of APIs:
~~~~~~~~~~~~~
1. To mark starting point with string:
void trace_begin(int tag, char *str, ...)
Example:
    trace_begin(TTRACE_TAG_APPS, "Begin")

2. To mark starting point with unique id:
void trace_begin(int tag, int8_t uid)
Example:
    int id = 1234;
    trace_begin(TTRACE_TAG_APPS, id)

3. To mark end point pair with trace_begin:
void trace_end(int tag)
Example:
    trace_end(TTRACE_TAG_APPS)

4. To mark end point pair with trace_begin_u:
void trace_end_u(int tag)
Example:
    trace_end_u(TTRACE_TAG_APPS)

5. To record transition of scheduler:
int trace_sched(struct tcb_s *prev_tcb, struct tcb_s *next_tcb)
Example:
    struct tcb_s *prev;
    struct tcb_s *next;
    trace_sched(prev, next)
