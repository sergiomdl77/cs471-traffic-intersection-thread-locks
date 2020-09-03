/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>
#include <machine/spl.h>

/*
 * Constants for directions 
 */
#define N 0 
#define E 1
#define S 2
#define W 3

/*
 * Number of cars created.
 */
#define NCARS 20

/*
 * Global lock variables
 */
struct lock *SE;
struct lock *SW;
struct lock *NE;
struct lock *NW;

/*
 * Global variable to keep count of cars that
 * went through intersection
 */
int carsthrough;


/*
 *
 * Function Definitions
 *
 */



/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Description:
 *      This function implements passing straight through the
 *      intersection from any direction.
 *      It acquires the 2 locks (atomically) on the appropriate quadrants.
 */

static void gostraight(unsigned long cardirection, unsigned long carnumber)
{
	// locking up the appropriate quadrants according to the direction car is approaching from
      	switch(cardirection)
	{
		case N: 
		
			lock_double_acquire(NW, SW);	
			
			kprintf("carnumber : %ld, Approching from North going South\n", carnumber);
			
			kprintf("carnumber : %ld, in reg NW \n", carnumber );
			
			kprintf("carnumber : %ld, in reg SW \n", carnumber );

			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_double_release(NW, SW);	

			break;
		case E: 
			
			lock_double_acquire(NE, NW);

			kprintf("carnumber : %ld, Approching from East going West\n", carnumber);
	
			kprintf("carnumber : %ld, in reg NE \n", carnumber );
		
			kprintf("carnumber : %ld, in reg Nw \n", carnumber );
			
			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_double_release(NE, NW);

			break;
		case S:  
			
			lock_double_acquire(SE, NE);	
			 
			kprintf("carnumber : %ld, Approching from South going North\n", carnumber);
						
			kprintf("carnumber : %ld, in reg SE \n", carnumber );
			
			kprintf("carnumber : %ld, in reg NE \n", carnumber );
						
			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_double_release(SE, NE);

			break;
		case W:  
			
			lock_double_acquire(SW, SE);	
						 
			kprintf("carnumber : %ld, Approching from  West going  East\n", carnumber);
			
			kprintf("carnumber : %ld, in reg SW \n", carnumber );
			
			kprintf("carnumber : %ld, in reg SE \n", carnumber );
			
			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_double_release(SW, SE);

			break;
			
	}

}



/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Description:
 *      This function implements making a left turn through the 
 *      intersection from any direction. 
 *      It acquires the 3 locks (atomically) on the appropriate quadrants.
 */

static void turnleft(unsigned long cardirection, unsigned long carnumber)
{
	// locking up the appropriate quadrants according to the direction car is approaching from
  	switch(cardirection)
	{
		case N: 
			
			lock_triple_acquire(NW, SW, SE);	
			 
			kprintf("carnumber : %ld, Approching from North going East\n", carnumber);
			
			kprintf("carnumber : %ld, in reg NW \n", carnumber );
			
			kprintf("carnumber : %ld, in reg SW \n", carnumber );
		
			kprintf("carnumber : %ld, in reg SE \n", carnumber );

			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_triple_release(NW, SW, SE);

			break;
<<<<<<< stoplight.c
		case E:

=======
		case E:
			
			lock_triple_acquire(NE, NW, SW);	
>>>>>>> 1.2
			
			kprintf("carnumber : %ld, Approching from East going South\n", carnumber);
			
			kprintf("carnumber : %ld, in reg NE \n", carnumber );
			
			kprintf("carnumber : %ld, in reg NW \n", carnumber );
			
			kprintf("carnumber : %ld, in reg SW \n", carnumber );		
			
			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_triple_release(NE, NW, SW);

			break;
		case S:  
			
			lock_triple_acquire(SE, NE, NW);	

			kprintf("carnumber : %ld, Approching from South going West\n", carnumber);
			
			kprintf("carnumber : %ld, in reg SE \n", carnumber );
			
			kprintf("carnumber : %ld, in reg NE \n", carnumber );
			
			kprintf("carnumber : %ld, in reg NW \n", carnumber );	
			
			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_triple_release(SE, NE, NW);

			break;
		case W:  
			
			lock_triple_acquire(SW, SE, NE);
			
			kprintf("carnumber : %ld, Approching from West from North\n", carnumber);
			
			kprintf("carnumber : %ld, in reg SW \n", carnumber );
			
			kprintf("carnumber : %ld, in reg SE \n", carnumber );
			
			kprintf("carnumber : %ld, in reg NE \n", carnumber );

			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_triple_release(SW, SE, NE);
			break;
			
	}

}



/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Description:
 *      This function implements making a right turn through the 
 *      intersection from any direction.
 *      It acquires the one lock on the appropriate quadrant.
 */

static void turnright(unsigned long cardirection,unsigned long carnumber)
{
        // now we will evaluate the cardirection using switch

	switch(cardirection)
	{
		case N: 
					
			lock_acquire(NW);
		
			kprintf("carnumber : %ld, Approching from North going  West\n", carnumber);
			
			kprintf("carnumber : %ld, in reg NW \n", carnumber );

			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_release(NW);
	
			break;
		case E:  
		
			lock_acquire(NE);
	
			kprintf("carnumber : %ld, Approching from East going North\n", carnumber );
			
			kprintf("carnumber : %ld, in reg NE \n", carnumber );
	
			kprintf("carnumber : %ld, Finishied \n", carnumber );
			
			lock_release(NE);
	
			break;
		case S: 
	
			lock_acquire(SE);
	
			kprintf("carnumber : %ld, Approching form South going East\n", carnumber );
			
			kprintf("carnumber : %ld, in reg SE \n", carnumber );
					
			kprintf("carnumber : %ld, Finished \n", carnumber );
			
			lock_release(SE);

			break;
		case W: 
	
			lock_acquire(SW);
	
			kprintf("carnumber : %ld, Approching from West going South\n", carnumber );
			
			kprintf("carnumber : %ld, in reg SW \n", carnumber );

			kprintf("carnumber : %ld, Finished \n", carnumber );
		
	
			lock_release(SW);


			break;
			
	}
}


/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Description:
 *      This function implements a random assignment of approaching direction
 *      and trajectory of a car as it approaches the intersection.
 *      The function to generate random numbers was provided before hand.
 */
 
static void approachintersection(void * unusedpointer, unsigned long carnumber)
{
        int cardirection;  // car's id
	int cartraj;	// car's trajectory on the intesection

        // To avoid unused variable and function warnings.
        (void) unusedpointer;
//      (void) carnumber;
//	(void) gostraight;
//	(void) turnleft;
//	(void) turnright;

        
        // cardirection and trajectory are set randomly.
        cardirection = random() % 4;
	cartraj = random() % 3;

	switch(cartraj)
	{
		case 0: //if 0 turn left
			turnleft(cardirection, carnumber);
			break;
		case 1:// if 1 go straight
			gostraight(cardirection, carnumber);
			break;
		case 2:// if 2 turn right
			turnright(cardirection, carnumber);
			break;
	}
	

	int spl = splhigh();
	carsthrough++;
	splx(spl);
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Description:
 *      Driver code to start up the approachintersection() threads.  
 *	This function is simply in charge of creating the four
 *	locks (one for each quadrant in the intersection.
 *	It is also executes the creation of the cars (threads)
 *	that will go through the stoplight intesection.
 */

int createcars(int nargs, char ** args)
{
        int index, error;
	carsthrough = 0; 
       
        // To avoid unused variable warnings.
        (void) nargs;
        (void) args;


	// initializing locks by using lock_create from synch.c
	SE = lock_create("SE");
	SW = lock_create("SW");
	NE = lock_create("NE");
	NW = lock_create("NW");

	// creating the 20 threads (each of which represent a car)
        for (index = 0; index < NCARS; index++)
	{

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection now: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }


	while (carsthrough < NCARS)
	{
		thread_yield();
	}

        return 0;
}
