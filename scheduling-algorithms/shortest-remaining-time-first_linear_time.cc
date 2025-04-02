#include <iostream>
#include <vector>
#include <climits>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time = 0;
    int turnaround_time = 0;
    int waiting_time = 0;
    int response_time = -1; // -1 means it hasn't started yet
};

void calculate_srtf(std::vector<Process>& processes) {
    int n = processes.size();
    int current_time = 0, completed = 0;
    int last_executed = -1; // Store the last executed process to prevent redundant checks

    // Find the total burst time (upper bound of our loop)
    int total_burst_time = 0;
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time; // Initialize remaining time
        total_burst_time += processes[i].burst_time;
    }

    // Keep executing until all processes finish
    while (completed < n) {
        int shortest_job = -1;
        int min_remaining_time = INT_MAX;

        // Find the process with the shortest remaining time at the current time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_remaining_time) {
                    min_remaining_time = processes[i].remaining_time;
                    shortest_job = i;
                }
            }
        }

        // No process found, advance time
        if (shortest_job == -1) {
            current_time++;
            continue;
        }

        // If the process is executed for the first time, set response time
        if (processes[shortest_job].response_time == -1) {
            processes[shortest_job].response_time = current_time - processes[shortest_job].arrival_time;
        }

        // Execute the process for 1 unit of time
        processes[shortest_job].remaining_time--;
        current_time++;

        // If the process finishes execution
        if (processes[shortest_job].remaining_time == 0) {
            completed++;
            processes[shortest_job].completion_time = current_time;
            processes[shortest_job].turnaround_time = processes[shortest_job].completion_time - processes[shortest_job].arrival_time;
            processes[shortest_job].waiting_time = processes[shortest_job].turnaround_time - processes[shortest_job].burst_time;
        }
    }
}

// Function to display process details
void print_processes(const std::vector<Process>& processes) {
    std::cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\tRT\n";
    for (const Process& p : processes) {
        std::cout << p.id << "\t" << p.arrival_time << "\t" << p.burst_time << "\t"
                  << p.completion_time << "\t" << p.turnaround_time << "\t"
                  << p.waiting_time << "\t" << p.response_time << "\n";
    }
}

int main() {
    int n;
    std::cout << "Enter number of processes: ";
    std::cin >> n;

    std::vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        std::cout << "Enter arrival time for Process " << processes[i].id << ": ";
        std::cin >> processes[i].arrival_time;
        std::cout << "Enter burst time for Process " << processes[i].id << ": ";
        std::cin >> processes[i].burst_time;
    }

    calculate_srtf(processes);
    print_processes(processes);

    return 0;
}

