# concurrent-threads

This C++ program demonstrates the usage of threads to perform row-wise summation of a matrix. It provides two options for load balancing: static and dynamic. The program accepts command-line parameters to specify the load balancing method and the number of threads to use.

The program begins by including the necessary header files for input/output operations, threading, mutexes, and vectors.

Global variables are declared to be used by threads in the application. These variables include the number of rows and columns in the matrix (rows and cols), a mutex for locking the standard output (stdout_lock), a mutex for locking the counter variable (counter_lock), a volatile counter variable (counter), vectors to store thread counts and sum values (tcount and sum), and a 2D array representing the matrix to be processed (work).

The program defines two functions: sum_static and sum_dynamic, which implement the logic to sum the rows of the matrix using static and dynamic load balancing, respectively.

The sum_static function takes in the thread ID (tid) and the number of threads (num_threads). It uses the thread ID to determine the first row for each thread and then advances the row number by num_threads to determine the next row to process. It iterates over the rows, calculates the sum of each row, and updates the corresponding sum value in the sum vector. The function also keeps track of the number of rows processed by each thread in the tcount vector. It locks the stdout_lock mutex to print thread-specific information such as the thread ID, starting and ending messages, and the corresponding thread's tcount and sum values.

The sum_dynamic function takes in the thread ID (tid). It uses a mutex lock (counter_lock) to access the global counter variable (counter) in a critical section. Each thread decrements the counter variable to determine the next row to process. It then calculates the sum of the row and updates the corresponding sum value in the sum vector. Similar to sum_static, it also keeps track of the number of rows processed by each thread in the tcount vector. It locks the stdout_lock mutex to print thread-specific information.

The usage function provides a usage statement for the program's command-line parameters.

In the main function, the number of concurrent threads supported by the system is printed using thread::hardware_concurrency(). The random number generator is seeded, and variables for dynamic load balancing (d) and the number of threads (num) are initialized. The program then parses the command-line parameters using getopt and updates the corresponding variables. If the number of threads specified is greater than the system's available cores, it limits the number of threads to the available cores.

The tcount and sum vectors are resized to have one element for each thread, initialized to 0. The work matrix is initialized with random values using the rand function.

Depending on the load balancing method chosen (d), the program creates threads and assigns them to either the sum_static or sum_dynamic functions. Each thread is stored in the threads vector.

After all threads have been created, the program waits for each thread to finish using join and cleans up the thread objects. It calculates the total number of rows processed by summing the values in the tcount vector. It also calculates the gross sum by summing the values in the sum vector. Finally, it prints the total work and gross sum values.

Overall, this program demonstrates the usage of threads to parallelize the summation of rows in a matrix. It provides options for static and dynamic load balancing, allowing efficient distribution of work among multiple threads.
