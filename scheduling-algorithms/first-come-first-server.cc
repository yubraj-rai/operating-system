#include<iostream>
#include<vector>
#include<queue>

struct Process{
    size_t arrival_time ;
    size_t burst_time ;
    size_t completion_time ;
    size_t turn_around_time ;
    size_t waiting_time ;
    size_t response_time ;
} ;

void calculate_times(std::queue<Process>& pqueue) {
    size_t current_time = 0 ;
    while(!pqueue.empty()) {
        Process& p = pqueue.front() ;

        //if process arrives after the current time, wait
        if(current_time < p.arrival_time){
            current_time = p.arrival_time ;
        }

        //completion time(CT) = current_time + burst time 
        p.completion_time = current_time + p.burst_time ;

        //Turn around time(TAT) = completion time - arrival time
        p.turn_around_time = p.completion_time - p.arrival_time ;

        //waiting time(WT) = turn around time - burst time
        p.waiting_time = p.turn_around_time - p.burst_time ;

        //Response time(RT) = waiting time( since fcfs doesn't preempt)
        p.response_time = p.waiting_time ;

        //update the current time to completion time
        current_time = p.completion_time ;

        pqueue.pop() ;
    }
}

int main(void){

    size_t n ;
    std::cout<<"Enter number of process"<<std::endl;
    std::cin>>n ;

    std::queue<Process> ProcessQueue ;
    for(auto i = 0 ; i < n; i++){
        Process p ;
        std::cout<<"Enter arrival time: P "<<i+1<<std::endl;
        std::cin>>p.arrival_time ;

        std::cout<<"Enter burst time: p"<<i+1<<std::endl;
        std::cin>>p.burst_time ;

        ProcessQueue.push(p) ;
    }

    calculate_times(ProcessQueue) ;


    return 0 ;
}
