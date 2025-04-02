#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

struct Process {
    size_t id;
    size_t arrival_time;
    size_t burst_time;
    size_t remaining_time;
    size_t completion_time = 0;
    size_t turn_around_time = 0;
    size_t waiting_time = 0;
    size_t response_time = -1;  // -1 indicates response time is not yet assigned
};

// Custom comparator for the priority queue (SRTF Scheduling)
struct Compare {
    bool operator()(const Process* p1, const Process* p2) {
        if (p1->remaining_time == p2->remaining_time)
            return p1->arrival_time > p2->arrival_time;  // If same remaining time, pick earlier arrival
        return p1->remaining_time > p2->remaining_time; // Shorter remaining time gets priority
    }
};

// Function to calculate completion, turnaround, waiting, and response times
void calculate_times(std::vector<Process>& processes) {
    size_t n = processes.size();
    size_t completed = 0, current_time = 0;
    size_t index = 0;

    // Sort processes by arrival time
    std::sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.arrival_time < p2.arrival_time;
    });

    // Min-heap (priority queue) for the process with the shortest remaining time
    std::priority_queue<Process*, std::vector<Process*>, Compare> pq;

    while (completed < n) {
        // Push all processes that have arrived into the priority queue
        while (index < n && processes[index].arrival_time <= current_time) {
            pq.push(&processes[index]);
            index++;
        }

        if (!pq.empty()) {
            Process* p = pq.top();
            pq.pop();

            // Assign response time if it's the first time the process is executed
            if (p->response_time == -1) {
                p->response_time = current_time - p->arrival_time;
            }

            p->remaining_time--;  // Execute for 1 unit
            current_time++;

            if (p->remaining_time == 0) {  // Process completes
                completed++;
                p->completion_time = current_time;
                p->turn_around_time = p->completion_time - p->arrival_time;
                p->waiting_time = p->turn_around_time - p->burst_time;
            } else {
                pq.push(p);  // Push back if it's not completed
            }
        } else {
            // If no process is available, jump to the next process arrival time
            if (index < n) {
                current_time = processes[index].arrival_time;
            }
        }
    }
}

// Function to display process details
void print_processes(const std::vector<Process>& processes) {
    std::cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n";
    for (const auto& p : processes) {
        std::cout << p.id << "\t"
                  << p.arrival_time << "\t"
                  << p.burst_time << "\t"
                  << p.completion_time << "\t"
                  << p.turn_around_time << "\t"
                  << p.waiting_time << "\t"
                  << p.response_time << "\n";
    }
}

int main() {
    size_t n;
    std::cout << "Enter number of processes: ";
    std::cin >> n;

    std::vector<Process> processes(n);
    for (size_t i = 0; i < n; i++) {
        processes[i].id = i + 1;

        std::cout << "Enter arrival time for Process " << processes[i].id << ": ";
        std::cin >> processes[i].arrival_time;

        std::cout << "Enter burst time for Process " << processes[i].id << ": ";
        std::cin >> processes[i].burst_time;

        processes[i].remaining_time = processes[i].burst_time;  // Initialize remaining time
    }

    calculate_times(processes);
    print_processes(processes);

    return 0;
}

