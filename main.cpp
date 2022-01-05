#include <iostream>
#include <pthread.h>
#include <string>
#include <stdlib.h>
#include <fcntl.h>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
//thread condition variable initialization
static pthread_mutex_t bsem;
static pthread_cond_t waitTurn = PTHREAD_COND_INITIALIZER;
static int turn = 0;
//struct to store info from input
struct node{

    long long value;
    int position;
    
};
//void function that is caled when threads are created
void *seven_segment_display(void *arg_ptr){
    //Issue lock
    pthread_mutex_lock(&bsem);
    //casting to access data from struct
    struct node *argument = (struct node *)arg_ptr;
    int pos = argument->position;
    long long number = argument->value;

    while(turn != pos){
        pthread_cond_wait(&waitTurn, &bsem);
    }
    //array that keeps track of the codes for their respective digit
    string codes[10] = {
        "1 1 1 1 1 1 0",
        "0 1 1 0 0 0 0",
        "1 1 0 1 1 0 1",
        "1 1 1 1 0 0 1",
        "0 1 1 0 0 1 1",
        "1 0 1 1 0 1 1",
        "1 0 1 1 1 1 1",
        "1 1 1 0 0 0 0",
        "1 1 1 1 1 1 1",
        "1 1 1 1 0 1 1"
    };

    vector<pair <int,string> > output;
    int digit = 0;
    int size = 1;
    //Obtaining the size of the number
    while(number >= 10)
    {
        size++;
        number /= 10;
    }
    number = argument->value;
    //getting the end of the number one by one and storing in vector along with its code
    for(int i = 0; i < size; i++){
        digit = number % 10;
        output.push_back(make_pair(digit,codes[digit]));
        number /= 10;
    }
   //Printing from the output vector backwards to obtain correct order
    for(int i = output.size()-1; i >= 0;i--){
        cout << output[i].first << " = " << output[i].second << endl;
    }
    cout << endl;

    turn++;
	pthread_cond_broadcast(&waitTurn);
	pthread_mutex_unlock(&bsem);
	return NULL;
}

int main(int argc, char *argv[]){

    long long line;
    
    vector<long long> inputs;

    //reading inputs into vector
    while(cin >> line){
        inputs.push_back(line);
    }

    //obtaining number of values (n)
    int n = inputs.size();

    //creating struct array for threads
    struct node arg[n];

    //assigning the correct values to the struct array
    for(int i = 0; i < n; i++){
        
        arg[i].position = i;
        arg[i].value = inputs[i];
    }

    //Initializing access to mutex condition variable
    pthread_mutex_init(&bsem, NULL);
    //Creates thread array
    pthread_t *tid = new pthread_t[n];

    //create threads
    for(int i =0; i < n; i++)
	{
		pthread_create(&tid[i], NULL, seven_segment_display, &arg[i]);
	}
    //Wait for threads to join
    for(int i = 0; i < n; i++)
	{
		pthread_join(tid[i], NULL);
	}
    //freeing memory
	delete [] tid;
	return 0;






}