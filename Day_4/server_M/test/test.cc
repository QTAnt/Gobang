#include <iostream>
#include <vector>
#include <string>
#include <pthread.h>
#include <time.h>

using namespace std;

int main()
{
    pthread_mutex_t lock;
    pthread_cond_t cond;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init (&cond, NULL);
    
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 5;

    int ret = pthread_cond_timedwait(&cond, &lock, &ts);
    cout << ret << endl;
}
