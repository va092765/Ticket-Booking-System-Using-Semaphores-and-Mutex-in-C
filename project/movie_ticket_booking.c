#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define TOTAL_TICKETS       60
#define PRE_RESERVED        20
#define MAX_CONCURRENT_MEM   2
#define MAX_TICKETS_PER_MEM 10
#define NUM_MEMBERS           5   /* change this to simulate more/less members */

/* Shared resource */
int available_tickets = TOTAL_TICKETS - PRE_RESERVED;   /* 40 */

/* Synchronization primitives */
sem_t booking_sem;                 /* allows only MAX_CONCURRENT_MEM at a time */
pthread_mutex_t ticket_mutex;       /* protects available_tickets */

/* Simple struct to pass data to each thread */
typedef struct {
    int member_id;
    int tickets_requested;
} Member;

void *book_tickets(void *arg) {
    Member *m = (Member *)arg;

    printf("Member %d: waiting to enter booking counter...\n", m->member_id);

    /* --- Enter the semaphore-controlled section --- */
    sem_wait(&booking_sem);
    printf("Member %d: entered booking counter (max %d at a time)\n",
           m->member_id, MAX_CONCURRENT_MEM);

    /* Simulate time taken to process the request */
    sleep(1);

    /* --- Critical section: protected by mutex --- */
    pthread_mutex_lock(&ticket_mutex);

    if (m->tickets_requested > MAX_TICKETS_PER_MEM) {
        printf("Member %d: FAILED - cannot book more than %d tickets in one request\n",
               m->member_id, MAX_TICKETS_PER_MEM);
    } else if (m->tickets_requested <= available_tickets) {
        available_tickets -= m->tickets_requested;
        printf("Member %d: SUCCESS - booked %d ticket(s). Remaining tickets = %d\n",
               m->member_id, m->tickets_requested, available_tickets);
    } else {
        printf("Member %d: FAILED - only %d ticket(s) left, requested %d\n",
               m->member_id, available_tickets, m->tickets_requested);
    }

    pthread_mutex_unlock(&ticket_mutex);
    /* --- End critical section --- */

    printf("Member %d: leaving booking counter\n", m->member_id);
    sem_post(&booking_sem);
    /* --- Exit semaphore-controlled section --- */

    free(m);
    return NULL;
}

int main() {
    pthread_t threads[NUM_MEMBERS];

    /* Sample ticket requests for each member - feel free to change these */
    int requests[NUM_MEMBERS] = {5, 12, 8, 10, 15};

    printf("=====================================================\n");
    printf(" MOVIE TICKET BOOKING SYSTEM (Semaphores + Mutex Demo)\n");
    printf("=====================================================\n");
    printf("Total tickets       : %d\n", TOTAL_TICKETS);
    printf("Pre-reserved tickets: %d\n", PRE_RESERVED);
    printf("Available tickets   : %d\n", available_tickets);
    printf("Max concurrent members: %d\n", MAX_CONCURRENT_MEM);
    printf("Max tickets/member    : %d\n", MAX_TICKETS_PER_MEM);
    printf("-----------------------------------------------------\n\n");

    /* Initialize semaphore: allows MAX_CONCURRENT_MEM threads at once */
    sem_init(&booking_sem, 0, MAX_CONCURRENT_MEM);

    /* Initialize mutex */
    pthread_mutex_init(&ticket_mutex, NULL);

    /* Create a thread for each member trying to book tickets */
    for (int i = 0; i < NUM_MEMBERS; i++) {
        Member *m = (Member *)malloc(sizeof(Member));
        m->member_id = i + 1;
        m->tickets_requested = requests[i];

        pthread_create(&threads[i], NULL, book_tickets, (void *)m);
    }

    /* Wait for all member threads to finish */
    for (int i = 0; i < NUM_MEMBERS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n-----------------------------------------------------\n");
    printf("All bookings processed. Final available tickets = %d\n", available_tickets);
    printf("=====================================================\n");

    /* Cleanup */
    sem_destroy(&booking_sem);
    pthread_mutex_destroy(&ticket_mutex);

    return 0;
}
