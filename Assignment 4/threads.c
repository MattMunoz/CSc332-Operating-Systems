#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// A struct that represents a reader-writer lock
typedef struct {
    // A mutex that protects the shared data and the counters
    pthread_mutex_t mutex;
    // A condition variable that signals when the lock is available
    pthread_cond_t cv;
    // A counter that keeps track of the number of active readers
    int readers;
    // A flag that indicates whether there is an active writer
    int writer;
} rwlock_t;

// A function that initializes a reader-writer lock
void rwlock_init(rwlock_t *lock) {
    // Initialize the mutex
    pthread_mutex_init(&lock->mutex, NULL);
    // Initialize the condition variable
    pthread_cond_init(&lock->cv, NULL);
    // Initialize the number of active readers to zero
    lock->readers = 0;
    // Initialize the flag of active writer to zero
    lock->writer = 0;
}

// A function that acquires a read lock
void readLock(rwlock_t *lock) {
    // Lock the mutex
    pthread_mutex_lock(&lock->mutex);
    
    // Wait until there is no active writer
    while (lock->writer) {
    	pthread_cond_wait(&lock->cv, &lock->mutex);
    }
    
    // Increment the number of active readers
    lock->readers++;

    // Unlock the mutex
    pthread_mutex_unlock(&lock->mutex);
}

// A function that releases a read lock
void readUnlock(rwlock_t *lock) {
    // Lock the mutex
    pthread_mutex_lock(&lock->mutex);

    // Decrement the number of active readers
    lock->readers--;

    // If there are no more active readers, signal the condition variable. Think carefully between pthread_cond_signal() and pthread_cond_broadcast()
    if (lock->readers == 0) {
    	pthread_cond_broadcast(&lock->cv);
    }

    // Unlock the mutex
    pthread_mutex_unlock(&lock->mutex);
}

// A function that acquires a write lock
void writeLock(rwlock_t *lock) {
    // Lock the mutex
    pthread_mutex_lock(&lock->mutex);
    
    // Wait until there is no active writer or reader
    while (lock->writer > 0 || lock->readers > 0) {
    	pthread_cond_wait(&lock->cv, &lock->mutex);
    }

    // Set the writer flag with appropriate value
    lock->writer = 1;

    // Unlock the mutex
    pthread_mutex_unlock(&lock->mutex);
}

// A function that releases a write lock
void writeUnlock(rwlock_t *lock) {
    // Lock the mutex
    pthread_mutex_lock(&lock->mutex);

    // Set the writer flag with appropriate value
    lock->writer = 0;

    // Signal the condition variable. Think carefully between pthread_cond_signal() and pthread_cond_broadcast()
    pthread_cond_broadcast(&lock->cv);

    // Unlock the mutex
    pthread_mutex_unlock(&lock->mutex);
    
}

// global variables that represent the shared data
// open a file named "file.txt" in read-write mode, creating it if it doesn't exist. Also ensure everytime you write it should follow append.
// the opened file descriptor should consider as a global variable.
// write your code
FILE *file;
int data = 0;

// A global variable that represents the reader-writer lock
rwlock_t lock;

// A function that simulates a reader thread
void *reader(void *arg) {
    // Get the thread id from the argument
    int id = *(int *)arg;

    // Acquire a read lock
    readLock(&lock);

    printf("Reader %d: data = %d\n", id, data);

    // Read the shared data
    // Read the file and print it's content in the terminal with the reader thread id (id) and data. (you can work with a fixed sized buffer. Minimum length 100)
    //  [Example Format:(Reader thread id: 1 -> with data: 1 -> file content: content of the file)]
    char buff[100];
    while (fgets(buff, 100, file) != NULL) {
    	printf("Writer thread: %d -> with data: %d -> file content: %s", id, data, buff);
    }
    

    // Release a read lock
    readUnlock(&lock);

    return NULL;
}

// A function that simulates a writer thread
void *writer(void *arg) {
    // Get the thread id from the argument
    int id = *(int *)arg;

    // Acquire a write lock
    writeLock(&lock);

    data++;
    printf("Writer %d: data = %d\n", id, data);

    // Write to the shared data
    // write the writer thread id (id) to the file with data. You should write with append. [Example Format: (Writer thread : 1 with data: 1)]
    // also add a new line when you perform a write operation
    fprintf(file, "Writer thread: %d with data: %d\n", id, data);
    
    

    // Release a write lock
    writeUnlock(&lock);
    
    
    return NULL;
}

// A constant that defines the number of reader threads
#define NUM_READERS 5

// A constant that defines the number of writer threads
#define NUM_WRITERS 3

// The main function
int main(int argc, char *argv[]) {
    // open file
    file = fopen("file.txt", "a+");
    
    // Initialize the reader-writer lock
    rwlock_init(&lock);
    
    // Create an array of reader thread ids
    int readerID[NUM_READERS];
    
    // Create an array of writer thread ids
    int writerID[NUM_WRITERS];

    // Create an array of reader thread handles
    pthread_t readerThreads[NUM_READERS];
    
    
    // Create an array of writer thread handles
    pthread_t writerThreads[NUM_WRITERS];
    

    // Loop through the writer thread ids
    for (int i = 0; i < NUM_WRITERS; i++) {
        // Assign the id to the current index
        // update the value of writer thread ids array
        writerID[i] = i + 1;
        
        // Create a writer thread with the corresponding id
        pthread_create(&writerThreads[i], NULL, writer, &writerID[i]);
    }

    // Loop through the reader thread ids
    for (int i = 0; i < NUM_READERS; i++) {
        // Assign the id to the current index
        // update the value of reader thread ids array 
        readerID[i] = i + 1;
        
        // Create a reader thread with the corresponding id
        pthread_create(&readerThreads[i], NULL, reader, &readerID[i]);
    }

    // Loop through the writer thread handles
    for (int i = 0; i < NUM_WRITERS; i++) {
        // Join the writer thread
        pthread_join(writerThreads[i], NULL);
    }

    // Loop through the reader thread handles
    for (int i = 0; i < NUM_READERS; i++) {
        // Join the reader thread
        pthread_join(readerThreads[i], NULL);
    }
    
    fclose(file);
    
    return 0;
}
