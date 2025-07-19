#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int finishTime;
};

void roundRobinScheduler(vector<Process>& processes, int timeQuantum) {
    queue<int> readyQueue;
    int currentTime = 0;
    int completedProcesses = 0;
    int n = processes.size();
    vector<int> startTimes(n, -1);
    vector<int> endTimes(n, -1);

    while (completedProcesses < n) {
        // Add processes to the ready queue that have arrived by the current time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && startTimes[i] == -1) {
                readyQueue.push(i);
                startTimes[i] = currentTime;
            }
        }

        if (readyQueue.empty()) {
            currentTime++;
            continue;
        }

        int idx = readyQueue.front();
        readyQueue.pop();

        int executionTime = min(timeQuantum, processes[idx].remainingTime);
        processes[idx].remainingTime -= executionTime;
        currentTime += executionTime;

        if (processes[idx].remainingTime == 0) {
            completedProcesses++;
            processes[idx].finishTime = currentTime;
            endTimes[idx] = currentTime;
        } else {
            readyQueue.push(idx);
        }
    }

    // Display Gantt Chart
    cout << "\nGantt Chart:\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << processes[i].id << " (" << startTimes[i] << "-" << endTimes[i] << ") ";
    }
    cout << "\n";

    // Calculate and display turnaround time and waiting time
    cout << "\nProcess\tTurnaround Time\tWaiting Time\n";
    for (int i = 0; i < n; i++) {
        int turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
        int waitingTime = turnaroundTime - processes[i].burstTime;
        cout << "P" << processes[i].id << "\t" << turnaroundTime << "\t\t" << waitingTime << "\n";
    }
}

void simulateStarvation(vector<Process>& processes, int baseQuantum) {
    cout << "\nSimulating Starvation Scenario:\n";
    for (int i = 0; i < processes.size(); i++) {
        int dynamicQuantum = baseQuantum + (i * 2); // Increase quantum for later processes
        cout << "Running P" << processes[i].id << " with dynamic quantum: " << dynamicQuantum << "\n";
        processes[i].remainingTime = processes[i].burstTime;
    }
    roundRobinScheduler(processes, baseQuantum);
}

int main() {
    vector<Process> processes = {
        {1, 0, 10, 10, 0},
        {2, 1, 5, 5, 0},
        {3, 2, 8, 8, 0},
        {4, 3, 2, 2, 0}
    };

    int timeQuantum = 3;

    // Task 1: Basic Round Robin Scheduling
    cout << "Task 1: Basic Round Robin Scheduling\n";
    roundRobinScheduler(processes, timeQuantum);

    // Task 2: Dynamic Time Quantum
    cout << "\nTask 2: Dynamic Time Quantum\n";
    for (int i = 0; i < processes.size(); i++) {
        processes[i].remainingTime = processes[i].burstTime;
    }
    int dynamicQuantum = timeQuantum + 2; // Example of dynamic quantum
    roundRobinScheduler(processes, dynamicQuantum);

    // Task 3: Simulate Starvation Scenarios
    cout << "\nTask 3: Simulate Starvation Scenarios\n";
    simulateStarvation(processes, timeQuantum);

    return 0;
}