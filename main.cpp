
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <queue>

using namespace std;

// Process Control Block (PCB)
struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int remainingTime;
};

struct Execution {
    int pid;
    int StartTime;
    int EndTime;
};

int main() {

    int n;

    cout << "Enter number of processes: ";
    cin >> n;

    if (n <= 0) {
        cout << "Invalid number of processes." << endl;
        return 1;
    }

    vector<Process> processes(n);

    // Step 1: Accept process information
    for (int i = 0; i < n; i++) {

        processes[i].pid = i + 1;

        cout << "\nEnter details for Process "
             << processes[i].pid << endl;

        cout << "Arrival Time: ";
        cin >> processes[i].arrivalTime;

        cout << "Burst Time: ";
        cin >> processes[i].burstTime;

        if (processes[i].arrivalTime < 0 ||
            processes[i].burstTime <= 0) {

            cout << "Invalid process information!" << endl;
            return 1;
        }

        // Initialize remaining time
        processes[i].remainingTime = processes[i].burstTime;
    }

    // Step 2: Accept Time Quantum
    int timeQuantum;

    cout << "\nEnter Time Quantum: ";
    cin >> timeQuantum;

    if (timeQuantum <= 0) {
        cout << "Invalid Time Quantum!" << endl;
        return 1;
    }

    // Step 3: Sort processes by Arrival Time
    stable_sort(
        processes.begin(),
        processes.end(),
        [](const Process& a, const Process& b) {
            return a.arrivalTime < b.arrivalTime;
        }
    );

    // Step 4: Initialize Round Robin scheduling
    queue<int> readyQueue;
    vector<Execution> executionHistory;

    int nextProcess = 0;
    int currentTime = 0;
    int completedCount = 0;

    // Add processes that have arrived at time 0
    while (nextProcess < n &&
           processes[nextProcess].arrivalTime <= currentTime) {

        readyQueue.push(nextProcess);
        nextProcess++;
    }

    // Step 5: Round Robin Scheduling
    cout << "\n========== Execution Order ==========\n";

    while (completedCount < n) {

        // If ready queue is empty, move to next arrival
        if (readyQueue.empty()) {
            currentTime = processes[nextProcess].arrivalTime;
        }

        // Add all newly arrived processes
        while (nextProcess < n &&
               processes[nextProcess].arrivalTime <= currentTime) {

            readyQueue.push(nextProcess);
            nextProcess++;
        }

        // Select process from front of queue
        int index = readyQueue.front();
        readyQueue.pop();

        // Calculate execution time
        int executionTime = min(
            timeQuantum,
            processes[index].remainingTime
        );

        // Execute process
        int startTime = currentTime;
        currentTime += executionTime;

        // Update remaining time
        processes[index].remainingTime -= executionTime;

        cout << "P" << processes[index].pid
             << " : " << startTime
             << " - " << currentTime
             << " | Remaining Time: "
             << processes[index].remainingTime << endl;

        // Add newly arrived processes to ready queue
        while (nextProcess < n &&
               processes[nextProcess].arrivalTime <= currentTime) {

            readyQueue.push(nextProcess);
            nextProcess++;
        }

        // Check whether process is completed
        if (processes[index].remainingTime > 0) {

            // Process is unfinished: send it to queue's back
            readyQueue.push(index);

        } else {

            // Process is completed
            processes[index].completionTime = currentTime;
            completedCount++;
        }
    }

    // Step 6: Calculate Turnaround Time
    for (int i = 0; i < n; i++) {

        processes[i].turnaroundTime =
            processes[i].completionTime -
            processes[i].arrivalTime;
    }

    // Step 7: Calculate Waiting Time
    for (int i = 0; i < n; i++) {

        processes[i].waitingTime =
            processes[i].turnaroundTime -
            processes[i].burstTime;
    }

    // Step 8: Calculate averages
    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;

    for (int i = 0; i < n; i++) {

        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    double averageWaitingTime = totalWaitingTime / n;
    double averageTurnaroundTime = totalTurnaroundTime / n;

    // Step 9: Display results
    cout << fixed << setprecision(2);

    cout << "\n========== Round Robin Scheduling Results ==========\n\n";

    cout << left
         << setw(10) << "PID"
         << setw(10) << "AT"
         << setw(10) << "BT"
         << setw(10) << "CT"
         << setw(10) << "TAT"
         << setw(10) << "WT" << endl;

    cout << "------------------------------------------------------------\n";

    for (int i = 0; i < n; i++) {

        cout << left
             << setw(10) << processes[i].pid
             << setw(10) << processes[i].arrivalTime
             << setw(10) << processes[i].burstTime
             << setw(10) << processes[i].completionTime
             << setw(10) << processes[i].turnaroundTime
             << setw(10) << processes[i].waitingTime
             << endl;
    }

    cout << "\nAverage Waiting Time: "
         << averageWaitingTime << endl;

    cout << "Average Turnaround Time: "
         << averageTurnaroundTime << endl;

    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();

    return 0;
}
