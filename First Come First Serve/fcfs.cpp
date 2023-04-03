#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

// Struct untuk menyimpan informasi proses
struct Process {
    int id;             // ID proses
    int arrival_time;   // Waktu kedatangan
    int burst_time;     // Waktu burst
    int completion_time;    // Waktu selesai
    int waiting_time;   // Waktu waiting
};

void printBar (int burst_time, int i) {
    cout << "[";
    for (int j = 0; j < burst_time; j++) {
        this_thread::sleep_for(chrono::seconds(1)); 
        cout << "-";
    }
    cout << "] Proses " << i + 1 << " (" << burst_time << "s)" << endl;
}

void printSpasi (int burst_time) {
    cout << " ";
    for (int j = 0; j < burst_time; j++) {
        // this_thread::sleep_for(chrono::seconds(1)); 
        cout << " ";
    }
    cout << " ";
}

int main() {
    // Inisialisasi vektor proses dengan data yang telah dihardcode
    vector<Process> processes = {
        {1, 0, 6, 0, 0},
        {2, 1, 4, 0, 0},
        {3, 2, 8, 0, 0},
        {4, 3, 1, 0, 0},
        {5, 4, 3, 0, 0}
    };

    // Urutkan proses berdasarkan waktu kedatangan
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrival_time < b.arrival_time;
    });

    // Inisialisasi waktu saat ini, Gantt Chart, dan total waiting time
    int current_time = 0;
    vector<int> gantt_chart;
    int total_waiting_time = 0;

    int spasi = 0;

    // Lakukan FCFS
    cout << "Gantt Chart: " << endl;
    for (int i = 0; i < processes.size(); i++) {
        // Tambahkan proses ke Gantt Chart
        gantt_chart.push_back(processes[i].id);

        // Hitung waktu selesai dan waiting time
        current_time += processes[i].burst_time;
        processes[i].completion_time = current_time;
        processes[i].waiting_time = current_time - processes[i].arrival_time - processes[i].burst_time;
        
        // Tambahkan waiting time ke total_waiting_time
        total_waiting_time += processes[i].waiting_time;

        // Gantt Chart
        if (i == 0) {
            printSpasi(spasi);
        } else {
            spasi += processes[i - 1].burst_time + 2;
            printSpasi(spasi);
        }
        printBar(processes[i].burst_time, i);
    }

    // Cetak informasi setiap proses
    cout << endl << "Informasi setiap proses:" << endl;
    cout << "ID\tWaktu Kedatangan\tWaktu Burst\tWaktu Selesai\tWaktu Waiting" << endl;
    for (int i = 0; i < processes.size(); i++) {
        cout << processes[i].id << "\t" << processes[i].arrival_time << "\t\t\t" << processes[i].burst_time << "\t\t" << processes[i].completion_time << "\t\t" << processes[i].waiting_time << endl;
    }

    // Cetak average waiting time
    float avg_waiting_time = (float)total_waiting_time / processes.size();
    cout << endl << "Average Waiting Time: " << avg_waiting_time << endl;

    return 0;
}
