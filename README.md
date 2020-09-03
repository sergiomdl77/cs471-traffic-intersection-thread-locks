# cs471-traffic-intersection-thread-locks

BEFORE REVIEWING THIS PROJECT, IT IS IMPORTANT TO KNOW THE FOLLOWING:

This assignment will familiarize you with an operating system called OS/161 and also with System/161
which is the machine simulator on which OS/161 runs.  OS/161 is the operating system that I had to 
augment in this programming assignment.

The OS/161 distribution contains a full operating system source tree (provided to me in order
to complete this project), including some utility programs and libraries. 

This project was created to put into practice our my knowledge on synchronization of threads. The
language used for this project was C. 

Project Summary:

Synchronization Problem: Traffic Management at Podunk
You must solve this problem using the locks that you implemented above. Other solutions are not
acceptable.
Traffic through the main intersection in the town of Podunk, KS has increased over the past few years.
Until now the intersection has been a four-way stop but now the impending gridlock has forced the
residents of Podunk to admit that they need a more efficient way for traffic to pass through the
intersection. Your job is to design and implement a solution using the synchronization primitives (locks)
that you have developed in the previous part.

For the purposes of this problem we will model the intersection as shown above, dividing it into quarters
and identifying each quarter with which lane enters the intersection through that portion. (Just to clarify:
Podunk is in the US, so we're driving on the right side of the road.) Turns are represented by a
progression through one, two, or three portions of the intersection (for simplicity assume that U-turns do
not occur in the intersection). So if a car approaches from the North, depending on where it is going, it
proceeds through the intersection as follows:

Right: NW

Straight: NW-SW

Left: NW-SW-SE

The file you are to work with is in ~/os161/os161-1.11/kern/asst1. Ignore catsem.c and
catlock.c. You only need to work with stoplight.c

Implement (mutual exclusion) locks for OS/161. The interface for the lock structure is defined in
kern/include/synch.h. Stub code is provided in kern/thread/synch.c.
