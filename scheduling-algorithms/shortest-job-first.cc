#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

struct Process {
    size_t id;
    size_t arrival_time;
    size_t burst_time;
    size_t completion_time = 0;
    size_t turn_around_time = 0;
    size_t waiting_time = 0;
    size_t response_time = -1;  // -1 indicates response time is not yet assigned
};

// Custom comparator for priority queue (SJF Scheduling)
struct Compare {
    bool operator()(const Process* p1, const Process* p2) {
        return (p1->burst_time == p2->burst_time) ? (p1->arrival_time > p2->arrival_time) : (p1->burst_time > p2->burst_time);
    }
};

// Function to calculate completion, turnaround, waiting, and response times
void calculate_times(std::vector<Process>& processes) {
    size_t n = processes.size();
    size_t completed = 0, current_time = 0, index = 0;

    // Sort processes by arrival time
    std::sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.arrival_time < p2.arrival_time;
    });

    std::priority_queue<Process*, std::vector<Process*>, Compare> pq;

    while (completed < n) {
        while (index < n && processes[index].arrival_time <= current_time) {
            pq.push(&processes[index]);
            index++;
        }

        if (!pq.empty()) {
            Process* p = pq.top();
            pq.pop();

            p->response_time = (p->response_time == -1) ? current_time - p->arrival_time : p->response_time;
            p->completion_time = current_time + p->burst_time;
            p->turn_around_time = p->completion_time - p->arrival_time;
            p->waiting_time = p->turn_around_time - p->burst_time;

            current_time += p->burst_time;
            completed++;
        } else {
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
    }

    calculate_times(processes);
    print_processes(processes);

    return 0;
}

