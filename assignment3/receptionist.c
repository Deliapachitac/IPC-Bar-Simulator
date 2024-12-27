#include "segment.h"

Order generateRandomOrder() {
    Order order;
    order.water = 0;
    order.wine = 0;
    order.cheese = 0;
    order.salad = 0;

    // Initialize random number generator
    srand(time(NULL)^getpid());

    // Randomly generate order
    // Randomly choose water or wine (or both)
    if (rand() % 2) {
        order.water = 1;
    }
    if (rand() % 2) {
        order.wine = 1;
    }
    // Ensure at least one drink is chosen
    if (order.water == 0 && order.wine == 0) {
        if (rand() % 2) {
            order.water = 1;
        } else {
            order.wine = 1;
        }
    }

    // Randomly choose cheese (optional)
    if (rand() % 2) {
        order.cheese = 1;
    }

    // Randomly choose salad (optional)
    if (rand() % 2) {
        order.salad = 1;
    }

    return order;
}



int main(int argc, char *argv[]) {
    
    //Reading and saving the variables from the command line  
    int ordertime ;
    char shmname[50];   
    if(argc!=5) {
        printf("Incorrect input command line\n");
        exit(0);
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            i++;
            ordertime = atoi(argv[i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            i++;
            strcpy(shmname, argv[i]);
        } else {
            printf("Unknown flag : %s\n", argv[i]);
            exit(0);
        }
    }

    // Open shared memory
    int shm_fd = shm_open(shmname, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(0);
    }

    // Map shared memory
    SharedMemoryStruct *sharedState = (SharedMemoryStruct *)mmap(NULL, sizeof(SharedMemoryStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (sharedState == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    int processed_visitors = 0;
    pid_t visitor_id;
    while(true){
        
        sem_wait(&sharedState->receptionist_access);
        
        int slot = dequeueOrder(sharedState, &visitor_id);

        
        Order order = generateRandomOrder();
        sharedState->statistics.counter_water += order.water;
        sharedState->statistics.counter_wine += order.wine;
        sharedState->statistics.counter_cheese += order.cheese;
        sharedState->statistics.counter_salad += order.salad;

        printf("Receptionist: Preparing order for visitor %d\n", visitor_id);

        // Simulate preparing time for a random duration between [0.5 * ordertime , ordertime]     
        srand(time(NULL)^ getpid());
        int min_preparing_time = (int)(0.5 * ordertime);
        int random_preparing_time = min_preparing_time + rand() % (ordertime - min_preparing_time + 1);
        sleep(random_preparing_time);
        

        printf("Receptionist: Done order for visitor %d\n", visitor_id);

        sem_post(&sharedState->receprionist_buffer.order_ready[slot]); // Signal the visitor


        if (sharedState->served_visitors == sharedState->statistics.total_visitors) {
            sem_post(&sharedState->receptionist_access); // Ensure no deadlock
            sem_post(&sharedState->table_reset);         // Notify tables if waiting
            sem_post(&sharedState->mutex_access);   // Notify visitors if waiting
            break;
        }

    }

    printf("Receptionist: All visitors have been served. Exiting...\n");
    
    //execute the monitor 
    char *args[] = {"./monitor","-s",shmname, NULL};
    execvp(args[0], args);


    // Clean up
    if (munmap(sharedState, sizeof(SharedMemoryStruct)) == -1) {
        perror("munmap");
    }


    return 0;
}