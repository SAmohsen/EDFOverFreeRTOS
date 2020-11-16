# EDFOverFreeRTOS

<h2>Earliest deadline first (EDF)</h2> is a dynamic priority scheduling algorithm used in real-time operating systems to place processes in a priority queue. Whenever a scheduling event occurs (task finishes, new task released, etc.) the queue will be searched for the process closest to its deadline. This process is the next to be scheduled for execution.

The algorithm is suited to work in an environment where these assumptions applies:
 (A1) The requests for all tasks for which hard deadlines exist are periodic, with constant interval between requests.
 (A2) Deadlines consist of run-ability constraints only, i.e. each task must be completed before the next requests for it occurs.
 (A3) The tasks are independent in that requests for a certain task do not depend on the initialization or the completion of requests for other tasks.
 (A4) Run-time for each task is constant for that task and does not vary with time.Run-time refers to the time which is taken by a processor to execute the task without          interruption.
 (A5) Any non-periodic tasks in the system are special; they are initialization or failurerecovery routines; they displace periodic tasks while they themselves are being run, and do not themselves have hard, critical deadlines.
