#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

struct Process {
    size_t id;
    size_t arrival_time;
    size_t burst_time;
    size_t remaining_time;
    size_t completion_time = 0;
    size_t turn_around_time = 0;
    size_t waiting_time = 0;
    size_t response_time = -1;
};

// Round Robin Scheduling Algorithm
void round_robin(std::vector<Process>& processes, size_t time_quantum) {
    std::queue<int> q;  // Queue to store process indices
    size_t current_time = 0;
    size_t completed = 0;
    size_t n = processes.size();

    // Sort processes by arrival time
    std::sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.arrival_time < p2.arrival_time;
    });

    // Enqueue the first process
    q.push(0);
    std::vector<bool> in_queue(n, false);
    in_queue[0] = true;

    while (completed < n) {
        if (!q.empty()) {
            int i = q.front();
            q.pop();

            // First time process gets CPU, set response time
            if (processes[i].response_time == -1) {
                processes[i].response_time = current_time - processes[i].arrival_time;
            }

            // Execute process for the minimum of time quantum or remaining time
            int exec_time = std::min(time_quantum, processes[i].remaining_time);
            processes[i].remaining_time -= exec_time;
            current_time += exec_time;

            // Enqueue newly arrived processes
            for (size_t j = 0; j < n; j++) {
                if (!in_queue[j] && processes[j].arrival_time <= current_time && processes[j].remaining_time > 0) {
                    q.push(j);
                    in_queue[j] = true;
                }
            }

            // Re-enqueue the process if it is not yet completed
            if (processes[i].remaining_time > 0) {
                q.push(i);
            } else {
                // Process completed
                processes[i].completion_time = current_time;
                processes[i].turn_around_time = processes[i].completion_time - processes[i].arrival_time;
                processes[i].waiting_time = processes[i].turn_around_time - processes[i].burst_time;
                completed++;
                in_queue[i] = false; // Mark as completed
            }
        } else {
            // If queue is empty, move to the next available process
            for (size_t j = 0; j < n; j++) {
                if (processes[j].remaining_time > 0) {
                    q.push(j);
                    in_queue[j] = true;
                    break;
                }
            }
        }
    }
}

// Function to display process details
void print_processes(const std::vector<Process>& processes) {
    std::cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n";
    for (size_t i = 0; i < processes.size(); i++) {
        std::cout << processes[i].id << "\t"
                  << processes[i].arrival_time << "\t"
                  << processes[i].burst_time << "\t"
                  << processes[i].completion_time << "\t"
                  << processes[i].turn_around_time << "\t"
                  << processes[i].waiting_time << "\t"
                  << processes[i].response_time << "\n";
    }
}

int main() {
    size_t n, time_quantum;
    std::cout << "Enter number of processes: ";
    std::cin >> n;

    std::vector<Process> processes(n);
    for (size_t i = 0; i < n; i++) {
        processes[i].id = i + 1;
        std::cout << "Enter arrival time for Process " << processes[i].id << ": ";
        std::cin >> processes[i].arrival_time;
        std::cout << "Enter burst time for Process " << processes[i].id << ": ";
        std::cin >> processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
    }

    std::cout << "Enter the time quantum: ";
    std::cin >> time_quantum;

    round_robin(processes, time_quantum);
    print_processes(processes);

    return 0;
}

