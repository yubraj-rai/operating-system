#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>

struct Process {
    size_t id;
    size_t arrival_time;
    size_t burst_time;
    size_t completion_time = 0;
    size_t turn_around_time = 0;
    size_t waiting_time = 0;
    size_t response_time = 0;
};

// Function to calculate completion, turnaround, waiting, and response times
void calculate_times(std::vector<Process>& vProcess) {
    size_t current_time = 0;
    size_t completed = 0;

    // Sort processes by arrival time (to process them in order)
    std::sort(vProcess.begin(), vProcess.end(), [](const Process& p1, const Process& p2) {
            return p1.arrival_time < p2.arrival_time;
            });

    // Min-heap (priority queue) to select the shortest job first
    std::priority_queue<Process, std::vector<Process>, std::function<bool(const Process&, const Process&)>> pq(
            [](const Process& p1, const Process& p2) {
            return (p1.burst_time == p2.burst_time) ? (p1.arrival_time > p2.arrival_time) : (p1.burst_time > p2.burst_time);
            });

    size_t index = 0;
    while (completed < vProcess.size()) {
        // Push all processes that have arrived into the priority queue
        while (index < vProcess.size() && vProcess[index].arrival_time <= current_time) {
            pq.push(vProcess[index]);
            index++;
        }

        if (!pq.empty()) {
            Process p = pq.top();
            pq.pop();

            size_t process_index = p.id - 1;

            // Set completion time, turnaround time, waiting time, and response time
            vProcess[process_index].completion_time = current_time + p.burst_time;
            vProcess[process_index].turn_around_time = vProcess[process_index].completion_time - p.arrival_time;
            vProcess[process_index].waiting_time = vProcess[process_index].turn_around_time - p.burst_time;
            vProcess[process_index].response_time = vProcess[process_index].waiting_time;

            current_time += p.burst_time;
            completed++;
        } else {
            current_time++; // Move time forward if no process is available
        }
    }
}

// Function to display the process details
void print_processes(const std::vector<Process>& vProcess) {
    std::cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\tRT\n";
    for (const auto& p : vProcess) {
        std::cout << p.id << "\t" << p.arrival_time << "\t" << p.burst_time << "\t"
            << p.completion_time << "\t" << p.turn_around_time << "\t"
            << p.waiting_time << "\t" << p.response_time << "\n";
    }
}

int main() {
    size_t n;
    std::cout << "Enter number of processes: ";
    std::cin >> n;

    std::vector<Process> vProcess(n);
    for (size_t i = 0; i < n; i++) {
        vProcess[i].id = i + 1;

        std::cout << "Enter arrival time for Process " << vProcess[i].id << ": ";
        std::cin >> vProcess[i].arrival_time;

        std::cout << "Enter burst time for Process " << vProcess[i].id << ": ";
        std::cin >> vProcess[i].burst_time;
    }

    calculate_times(vProcess);
    print_processes(vProcess);

    return 0;
}

