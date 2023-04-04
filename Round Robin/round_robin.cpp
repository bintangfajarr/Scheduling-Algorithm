#include <bits/stdc++.h>

using namespace std;

// Struct untuk menyimpan informasi proses
struct Process
{
    int id;              // ID proses
    int arrival_time;    // Waktu kedatangan
    int burst_time;      // Waktu burst
    int completion_time; // Waktu selesai
    int waiting_time;    // Waktu waiting
    int remaining_time;
    bool isDone;
};

void printBar(int burst_time, int id)
{
    cout << "[";
    for (int j = 0; j < burst_time; j++)
    {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "-";
    }
    cout << "]" << endl;
}

void printSpasi(int burst_time)
{
    for (int j = 0; j < burst_time; j++)
    {
        // this_thread::sleep_for(chrono::seconds(1));
        cout << " ";
    }
}

bool isDone(Process it)
{
    return it.remaining_time <= 0;
}

int main()
{
    int size;
    vector<Process> processes;
    int quantum_time;

    cout << "Masukkan jumlah proses : ";
    cin >> size;
    cout << "Masukkan arrival time dan burst time setiap proses" << endl;
    for (int i = 1; i <= size; i++)
    {
        Process temp;
        temp.id = i;
        temp.waiting_time = 0;
        temp.completion_time = 0;
        temp.isDone = false;

        cout << "Proses " << i << " : ";
        cin >> temp.arrival_time >> temp.burst_time;

        temp.remaining_time = temp.burst_time;

        processes.push_back(temp);
    }
    cout << "Masukkan quantum time : ";
    cin >> quantum_time;

    // Urutkan proses berdasarkan waktu kedatangan
    sort(processes.begin(), processes.end(), [](Process a, Process b)
         { return a.arrival_time < b.arrival_time; });

    // Inisialisasi waktu saat ini, Gantt Chart, dan total waiting time
    int current_time = 0;
    int total_waiting_time = 0;

    if (processes[0].arrival_time > current_time)
    {
        current_time = processes[0].arrival_time;
    }

    int spasi = 0;

    bool flag = true;

    cout << endl
         << "Gantt Chart: " << endl;
    while (flag)
    {
        flag = false;
        for (int i = 0; i < processes.size(); i++)
        {
            if (!processes[i].isDone && processes[i].arrival_time <= current_time)
            {
                if (processes[i].remaining_time - quantum_time > 0)
                {
                    cout << "Proses " << processes[i].id << " (" << quantum_time << "s) | ";

                    printSpasi(spasi);
                    spasi += quantum_time + 2;

                    printBar(quantum_time, processes[i].id);

                    current_time += quantum_time;
                    processes[i].remaining_time -= quantum_time;

                    flag = true;
                }
                else
                {
                    cout << "Proses " << processes[i].id << " (" << processes[i].remaining_time << "s) | ";

                    printSpasi(spasi);
                    spasi += processes[i].remaining_time + 2;

                    printBar(processes[i].remaining_time, processes[i].id);

                    processes[i].waiting_time = current_time - (processes[i].burst_time - processes[i].remaining_time) - processes[i].arrival_time;
                    total_waiting_time += processes[i].waiting_time;

                    current_time += processes[i].remaining_time;
                    processes[i].remaining_time = 0;

                    processes[i].completion_time = current_time;

                    processes[i].isDone = true;
                }
            }
        }
    }

    // Cetak informasi setiap proses
    cout << endl
         << "Informasi setiap proses:" << endl;
    cout << "ID\tWaktu Kedatangan\tWaktu Burst\tWaktu Selesai\tWaktu Waiting" << endl;
    for (int i = 0; i < processes.size(); i++)
    {
        cout << processes[i].id << "\t" << processes[i].arrival_time << "\t\t\t" << processes[i].burst_time << "\t\t" << processes[i].completion_time << "\t\t" << processes[i].waiting_time << endl;
    }

    // Cetak average waiting time
    float avg_waiting_time = (float)total_waiting_time / processes.size();
    cout << endl
         << "Average Waiting Time: " << avg_waiting_time << endl;

    return 0;
}
