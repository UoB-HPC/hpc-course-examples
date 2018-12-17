Example 4: Blocking and Non-blocking Communication
==================================================

This directory contains three programs aimed at highlighting
the differences between _blocking_ and _non-blocking_ function
calls.

A blocking function will not return until it's designated
task is completed.
In the context of MPI, this may be sending
or receiving a message.

A non-blocking function can return immediately, but we cannot
assume that the function's designated task has been completed.
Instead we should think of calling this function as posting a
_request_ for some action.
We must check at some point later
in our program whether the action has been completed.
In the
context of MPI, and non-blocking receive function records the
request to receive a message but may well return before that
message has actually arrived.
`MPI_Test()` and `MPI_Wait()` are
two functions used to determine whether a request has been
satisfied.

blocking
--------

This is a simple point-to-point message passing program using
the blocking communications functions `MPI_Send()` and `MPI_Recv()`.
Processes are set to sleep for various periods of time so that
the messages are sent at different times, and so the calls to
`MPI_Recv()` take some seconds to return.

non-blocking
------------

In this program, we substitute `MPI_Send()` and `MPI_Recv()` with
their non-blocking counterparts, `MPI_Isend()` and `MPI_Irecv()`.
Both these functions can return immediately, and not wait
for the communication transaction to be completed.

Again delays are introduced to the program via the sleep
function and `MPI_Wait()` is used to check on the receive
requests.

bugged-non-blocking
-------------------

This program omits the call to `MPI_Wait()` and so subsequent
stages of the program are reached before they should be, i.e.
before key messages are received.

Exercise
--------

Experiment with the `MPI_Test()` function:

```C
int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
```

