/*
 * Synchronization primitives.
 * See synch.h for specifications of the functions.
 */

#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>

////////////////////////////////////////////////////////////
//
// Semaphore.

struct semaphore *
sem_create(const char *namearg, int initial_count)
{
	struct semaphore *sem;

	assert(initial_count >= 0);

	sem = kmalloc(sizeof(struct semaphore));
		
	if (sem == NULL) {
		return NULL;
	}

	sem->name = kstrdup(namearg);
	if (sem->name == NULL) {
		kfree(sem);
		return NULL;
	}

	sem->count = initial_count;
	return sem;
}

void
sem_destroy(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	spl = splhigh();
	assert(thread_hassleepers(sem)==0);
	splx(spl);

	/*
	 * Note: while someone could theoretically start sleeping on
	 * the semaphore after the above test but before we free it,
	 * if they're going to do that, they can just as easily wait
	 * a bit and start sleeping on the semaphore after it's been
	 * freed. Consequently, there's not a whole lot of point in 
	 * including the kfrees in the splhigh block, so we don't.
	 */

	kfree(sem->name);
	kfree(sem);
}

void 
P(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	/*
	 * May not block in an interrupt handler.
	 *
	 * For robustness, always check, even if we can actually
	 * complete the P without blocking.
	 */
	assert(in_interrupt==0);

	spl = splhigh();
	while (sem->count==0) {
		thread_sleep(sem);
	}
	assert(sem->count>0);
	sem->count--;
	splx(spl);
}

void
V(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);
	spl = splhigh();
	sem->count++;
	assert(sem->count>0);
	thread_wakeup(sem);
	splx(spl);
}

////////////////////////////////////////////////////////////
//
// Lock.
//


/*
 * TestAndSet()
 *
 * Arguments:
 * 	int *old_ptr: holds old value of lock's status.
 * 	int new:  holds new intended value for lock's status
 *
 * Returns:
 * 	old value of the lock's status
 *
 * Description:		
 *	Resets the status of the lock to the new value, but it 
 *	also tests the old status of such lock by returning it.
 *	For robustness, this function is executed atomically.
 */

int TestAndSet (int *old_ptr, int new)
{
	// disabling interrupts
	int spl = splhigh();  
	//fetech old value at old_prt
	int old = *old_ptr;
	//store 'new' into old_ptr
	*old_ptr  = new;

	// re-enabling interrupts	
	splx(spl);	

	return old;
}



/*
 * lock_create()
 * 	
 * Arguments:
 * 	const char *name:  holds the name of the lock to be created.
 *
 * Returns:
 * 	pointer to the newly created lock.
 *
 * Description:
 * 	Creates a new lock, with a name and a status variable.
 */

struct lock *lock_create(const char *name)
{
	struct lock *lock;

	lock = kmalloc(sizeof(struct lock));

	// if memory allocation failed
	if (lock == NULL)
	{
		return NULL;
	}

	// allocates memory for a copy of lock's name
	lock->name = kstrdup(name);

	// if name of new lock is NULL
	if (lock->name == NULL) 
	{
		kfree(lock);
		return NULL;
	}

	// initializes lock's status to 0
	int spl = splhigh();
	lock -> flag = 0;
	splx(spl);
	
	return lock;
}



/*
 * lock_destroy()
 *
 * Arguments:
 * 	struct lock *lock
 *
 * Returns:
 * 	nothing.
 *
 * Description:
 * 	Destroys a lock by making sure it is released
 * 	and freeing the memory that had been allocated for it.
 */

void lock_destroy(struct lock *lock)
{
	// making sure lock exists
	assert(lock != NULL);	
	
	// releasing the lock if it was being held
	if(lock -> flag == 1)
	{
		lock_release(lock);
	}

	// freeing the memory
	kfree(lock->name);
	kfree(lock->tname);
	kfree(lock);
}



/*
 * lock_acquire()
 *
 * Arguments:
 * 	struct lock *lock:  Lock to be acquired.
 *
 * Returns:
 * 	nothing
 *
 * Description:
 * 	Changes the status of the lock to indicate that it
 * 	is being held by a thread.  
 */

void lock_acquire(struct lock *lock)
{
	// Making sure the lock exists	
	assert(lock != NULL);

	// Spin lock with TestAndSet implementation for robustness 	


	while(TestAndSet(&lock->flag, 1) == 1)
	{
		thread_yield();
	}

	
}



/*
 * lock_double_acquire()
 *
 * Arguments:
 * 	struct lock *lock: first lock to be acquired
 * 	struct lock *lock2: second lock to be acquired
 *
 * Returns:
 * 	nothing
 *
 * Description:
 * 	Atomically, it changes the status of the locks to indicate
 *	that they are being held by two threads.  
 */

void lock_double_acquire(struct lock *lock, struct lock *lock2)
{
	// Making sure both locks exist
	assert(lock != NULL);
	assert(lock2 != NULL);

	
	// spin lock that breaks out of loop only if it could 
	// acquire both locks atomically

	while(1)
	{
		if(lock_do_i_hold(lock) == 0)// if first lock is available
		{
			lock_acquire(lock);
			if(lock_do_i_hold(lock2) == 0)// and second lock also available
			{
				lock_acquire(lock2);	// acquire both 
				break;			// and break from loop
			}
			else			// if second lock was not available
			{
				lock_release(lock);	// release lock first lock and yield
				thread_yield();
			}
		}
		else			// if first lock was not available
		{
			thread_yield();		// yield 
		}
	}


}



/*
 * lock_triple_acquire()
 *
 * Arguments:
 * 	struct lock *lock: first lock to be acquired
 * 	struct lock *lock2: second lock to be acquired
 *	struct lock *locc3: thir lock to be acquired
 * Returns:
 * 	nothing
 *
 * Description:
 * 	Atomically, it changes the status of the locks to indicate
 *	that they are being held by three threads.  
 */

void lock_triple_acquire(struct lock *lock, struct lock *lock2, struct lock *lock3)
{
	// Making sure the three locks exist
	assert(lock != NULL);
	assert(lock2 != NULL);
	assert(lock3 != NULL);
	
	// spin lock that breaks out of loop only if it could 
	// acquire the three locks atomically


	while(1)
	{
		if(lock_do_i_hold(lock) == 0)	// if first lock is available
		{
			lock_acquire(lock);
			if(lock_do_i_hold(lock2) == 0)	// and second lock is available
			{
				lock_acquire(lock2);
				
				if(lock_do_i_hold(lock3) == 0)	// and third lock is available
				{
					lock_acquire(lock3);		// acquire locks and 
					break;				// break from loop
				}
				else				// if third lock was not available
				{
					lock_release(lock);		// release first and second locks
					lock_release(lock2);		// and yield
					thread_yield();		
				}
			}
			else				// if second lock was not available
			{
				lock_release(lock);		// release first lock 
				thread_yield();			// and yield
			}
		}
		else				// if first lock was not available
		{	
			thread_yield();			// yield
		}


	}
}



/*
 * lock_release()
 *
 * Arguments:
 * 	struct lock *lock: Lock to be released.
 *
 * Returns:
 * 	Nothing.
 *
 * Description:
 * 	Changes the status of the lock to 0 to indicate
 * 	that the lock is available to be acquired by a thread.
 */

void lock_release(struct lock *lock)
{
	int spl = splhigh();

	// Making sure the three locks exist
	assert(lock != NULL);

	lock -> flag = 0; // set the flag to 0 to indicate that the lock is open

	splx(spl); // turn interruption on 
}



/*
 * lock_double_release()
 *
 * Arguments:
 * 	struct lock *lock: First lock to be released.
 *	struct lock *lock2: Second lock to be released.
 *
 * Returns:
 * 	Nothing.
 *
 * Description:
 * 	Atomically, it changes the status of both locks to 0 to indicate
 * 	that the locks are available to be acquired by a thread.
 */

void lock_double_release(struct lock *lock, struct lock *lock2)
{
	int spl = splhigh(); // disabling interrupts

	// Making sure the two locks exist
	assert(lock != NULL);
	assert(lock2 != NULL);

	lock -> flag = 0; // set the flags to 0 to indicate that the locks are open
	lock2 -> flag = 0;

	splx(spl); // turn interruption on 
}



/*
 * lock_triple_release()
 *
 * Arguments:
 * 	struct lock *lock: First lock to be released.
 *	struct lock *lock2: Second lock to be released.
 *	struct lock *lock3: Third lock to be released.
 *
 * Returns:
 * 	Nothing.
 *
 * Description:
 * 	Atomically, it changes the status of three locks to 0 to indicate
 * 	that the locks are available to be acquired by a thread.
 */

void lock_triple_release(struct lock *lock, struct lock *lock2, struct lock *lock3)
{
	int spl = splhigh(); // disabling interrupts

	// Making sure the three locks exist
	assert(lock != NULL);
	assert(lock2 != NULL);
	assert(lock3 != NULL);

	lock -> flag = 0; // set the flags to 0 to indicate that the locks are open
	lock2 -> flag = 0;
	lock3 -> flag = 0;

	splx(spl); // turn interruption on 
}



/*
 * lock_do_i_hold()
 *
 * Arguments:
 * 	struct lock *lock: lock being evaluated.
 *
 * Returns:
 * 	int value 1 if the lock is being held.
 * 	int value 0 otherwise.
 *
 * Description:
 * 	Simple function that returns the status of the lock,
 * 	as to whether or not it is being held by a thread.
 */

int lock_do_i_hold(struct lock *lock)
{

	if(lock->flag == 1)  
	{
		return 1; // lock is being held
	}
	return 0; // lock is not being held
}

////////////////////////////////////////////////////////////
//
// CV


struct cv *
cv_create(const char *name)
{
	struct cv *cv;

	cv = kmalloc(sizeof(struct cv));

	if (cv == NULL) {
		return NULL;
	}

	cv->name = kstrdup(name);
	if (cv->name==NULL) {
		kfree(cv);
		return NULL;
	}
	
	// add stuff here as needed
	
	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	// add stuff here as needed
	
	kfree(cv->name);
	kfree(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}
