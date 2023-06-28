//*************************************************************************
//
//    Jonathan Chavez
//    Z1866268
//    CSCI 463 Section 1
//
//    I certify that this is my own work and where appropriate an extension 
//    of the starter code provided for the assignment.  
//
//****************************************************************************/
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cassert>
#include <mutex>
#include <thread>
using namespace std;

// Global variables for use by threads in the application
constexpr int rows = 1000;  
constexpr int cols = 100;

std :: mutex stdout_lock;

std :: mutex counter_lock ;
volatile int counter = rows ;

std :: vector < int > tcount ;
std :: vector < uint64_t > sum ;

int work [ rows ][ cols ];

//**************************************************************************
// Implements the logic needed to sum the rows of the matrix using static load balancing to determine
// which rows will be processed by each thread.
//
// Uses the thread ID (passed in from main()) to determine the first row for each thread and then advance
// the row number by num_threads to determine the next row to process.
//
// @Note: A lot of this code is based of the C++ threads video.
//
// @param int tid: Represents the thread ID
// @param int num_threads: Represents the number of threads
//
//****************************************************************************/
void sum_static(int tid, int num_threads)
{
	// J will represent our new row.
        int j = 0;
        // Will be used to increase to advance my num_threads by incrementing it in our while loop
	int count = 0;
	int increment_value = 0;	

	stdout_lock.lock();
        cout << "Thread " << tid << " starting" << endl;
        stdout_lock.unlock();

	bool done = false;
	while(!done)
        {
		increment_value = num_threads * count;
		j = tid + increment_value;
                count++;

		// If our new row is greater than the number of rows the while loop ends
		if(j >= rows)
		   done = true;
	// Will grab our sum and pass it to our sum array
     if(!done)
        {
		// Will hold our sum
		uint64_t sum2 = 0;

		++tcount[tid];

		 for(int i = 0; i < cols ; i++)
                {
		         sum2 += work[j][i];
                }

                sum[tid] += sum2;
        }
        }
	// Once we exit our we print out our sum, tcount, and thread id
        stdout_lock.lock();
        cout << "Thread " << tid << " ending " << "tcount="<< tcount[tid] << " sum="<< sum[tid] << endl;
        stdout_lock.unlock();
}
//**************************************************************************
// Implements the logic needed to sum the rows of the matrix using dynamic load balancing to determine
// which rows will be processed by each thread.
//
// Each thread uses a mutex lock to access the global (and volatile) counter
// variable in the critical section to determine the next row to process.
//
// @Note: A lot of this code is based of the C++ threads video.
//
// @param int tid: Represents the thread ID
//
//****************************************************************************/
void sum_dynamic(int tid)
{
	// Will print our Thread id.
	stdout_lock.lock();
	cout << "Thread " << tid << " starting" << endl;
	stdout_lock.unlock();


	bool done = false;
	while(!done)
	{
		int count_copy;

		counter_lock.lock();
		{
			if(counter>0)
				--counter;

			else
				done = true;
		// Will calculate row
		count_copy = counter;
		}
		 counter_lock.unlock();

	if(!done)
	{
		uint64_t sum2 = 0;

		(void)count_copy;
		++tcount[tid];
		for(int i = 0; i < cols; i++)
		{
			// Will grab the sum of the column
			 sum2 += work[count_copy][i];
		}
		// Puts our sum to our sum array.
		sum[tid] += sum2;
	}
	}
	// Once we exit our we print out our sum, tcount, and thread id
	stdout_lock.lock();
	cout << "Thread " << tid << " ending " << "tcount="<< tcount[tid] << " sum="<< sum[tid] << endl;
	stdout_lock.unlock();
}
//**************************************************************************
// Our usage statement based of the usage statement from past assignments
// just less commands.
//****************************************************************************/
static void usage()
{
        cerr << "Usage: reduce [-d] [-t num]" << endl;
        cerr << "    -d use dynamic load-balancing" << endl;
        cerr << "    -t specifies the number of threads to use" << endl;
        exit(1);
}
/*******
 Provides a main() function so that it accepts the command-line parameters

 @Note: A lot of this code is based of the C++ threads video.
************/
int main(int argc, char **argv)
{
	// To determine the number of cores in the system.
	cout << thread::hardware_concurrency() << " concurrent thread supported." << endl;
  
 	 // Seeds your random number generator like this
        srand(0x1234);
        bool d = false; // If dynamic load-balancing picked.
	int opt;
	// By default two threads
        uint num = 2;
        while ((opt = getopt(argc, argv, "dt:")) != -1)
        {
                switch(opt)
                {
			// If case d
			case 'd':
			{
				d = true;
			}
			break;
			// If case t
			case 't':
			{
				 num = atoi(optarg);
				//  DO NOT start more threads than the system has cores!
				if(num > thread::hardware_concurrency())
				{
				num = thread::hardware_concurrency();
			        }
			}
			break;

			default:
				usage();
		}
}
        
	  vector<thread*> threads;

	  tcount.resize(num, 0); // make the tcount vector have one element for each thread & initialize them to 0
	  sum.resize(num, 0); // make the sum vector have one element for each thread & initialize them to 0

	// Initializes the data in the global work matrix using the rand() function from the standard C library
	for(int i = 0; i < rows;i++)
	{
	// This double for loop initializes my matrix from left to right, top-down, starting from the top.
		for(int j = 0; j < cols; j++)
		{
			work[i][j] = rand();
		}
	}
	// if d was not entered go to the default function
	// which is sum_static
	if(d == false)
	{
		for(uint i = 0; i < num; i++)
			  threads.push_back(new thread(sum_static,i, num));
	}
	// if d was entered go to the sum_dynamic function
	else if(d == true)
	{
		for(uint i = 0; i < num; i++)
			threads.push_back(new thread(sum_dynamic,i));
	}


	// Got this code from C++ thread video
	int total_work = 0;
	for(uint i = 0; i < num; i++)
	{
		threads.at(i)->join();
		delete threads.at(i);
		total_work += tcount.at(i);
	}

	// Will grab gross_sum very similar to the code for the total_work
        uint64_t gross_sum = 0;
                for (uint i = 0; i < num; i++)
                {
                        //Grabs the gross sum by iterating through the sum array.
                        gross_sum += sum[i];
                }
	// Will print the gross_sum and total_work
	cout << "main() exiting, total_work=" << total_work << " gross_sum=" << gross_sum << endl;
	return 0;
}
