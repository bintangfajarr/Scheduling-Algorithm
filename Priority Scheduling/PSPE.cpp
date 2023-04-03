/*

[ ALUR ]
1. Proses hardcode masuk.
2. Looping per second.
    - Kalau ada Proses yang selesai diproses hilang dari tabel (masuk ke tabel done).
3. Tiap second ditanya mau masukin input atau enggak.
    - Insert proses penentuan prioritynya ditentukan user.
4. Aging.
5. Selesai (Tabel Kosong)
    - Print keseluruhan proses (Tabel Done).

[ ATURAN ]
1. Maksimal prioritas bernilai 1.
2. Kenaikan prioritas (aging) tiap 3s waiting time.
3. Setiap proses id bersifat unique (dibuat oleh sistem).
4. Input user: [priority, init_burst_time]
5. Proses ready bisa diinterrupt jika dan hanya jika ada proses lain di ready yg lebih prioritas

[ PRESENTASI ]
1. Masukkin Proses ID dengan priority 7 dan burst time 8.

-) Menunjukkan aging.
-) Menunjukkan FCFS ketika prioritynya sama.
-) Menunjukkan Pre-Emptive.

*/

#include <iostream>
#include <queue>
using namespace std;

struct Process {
    int pid; // Process ID
    int priority; // Process Priority
    int priority_awal;
    int init_burst_time; // Initial Process Burst Time
    int sisa_burst_time; // Sisa Process Burst Time
    int waiting_time; // Process Waiting Time
    int arrival_time; // Process Arrival Time
    int turnaround_time; // Process Turnaround Time
};

int next_id = 4;    // untuk menentukan ID proses baru berikutnya

// Comparison function to sort the processes by priority
struct ComparePriority {
    bool operator()(const Process& p1, const Process& p2) {
        if(p1.priority == p2.priority)  // Jika priority sama
        {
            if(p1.arrival_time == p2.arrival_time)  // Jika arrival time juga sama
            {
                // (backup) tentukan dengan ID
                return p1.pid > p2.pid;
            } else {
                // Urutkan (top memiliki arrival time terkecil)
                return p1.arrival_time > p2.arrival_time;
            }
        }
        else
        {
            // Urutkan (top memiliki angka priority terkecil)
            return p1.priority > p2.priority;
        }
    }
};

// Function to implement Priority Scheduling Algorithm
void priorityScheduling(Process processes[], int n) {
    // Create a priority queue to store processes by priority
    priority_queue<Process, vector<Process>, ComparePriority> pq;
    vector<Process> done;           // Vector Proses Done
    Process current_process;        // Run state
    bool run_empty = true;          // Flag apakah run state kosong
    int top_priority;               // Menyimpan nilai priority tertinggi di ready state
    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    deque<int> dataChart;
    
    // Push all the processes into the priority queue
    for (int i = 0; i < n; i++) {
        pq.push(processes[i]);
    }

    int current_time = 0;
    
    bool all_process_terminate = false;
    while(!all_process_terminate)
    {
        // User input
        Process new_process;
        cout << "---\nApakah Anda ingin menginput proses baru (y/n)? ";
        string flag;
        cin >> flag;
        if (flag == "y") 
        {
            // Setup atribut new process
            new_process.pid = next_id; next_id++;
            cout << "Masukkan prioritas proses: ";
            cin >> new_process.priority;
            new_process.priority_awal = new_process.priority;
            cout << "Masukkan inisial burst time proses: ";
            cin >> new_process.init_burst_time;
            new_process.sisa_burst_time = new_process.init_burst_time;
            new_process.waiting_time = 0;
            new_process.arrival_time = current_time;
            // Push new process into priority queue
            pq.push(new_process);
        }

        // Kalau run state empty, ambil proses di ready
        if (run_empty) {
            current_process = pq.top();
            pq.pop();
            run_empty = false;  // Sekarang udah isi

            // Tambah ID baru di gantt chart
            dataChart.push_back(current_process.pid);   // ID
            dataChart.push_back(0);                     // burst time
        }

        // Print current process
        cout << "---\n-) Current Time: " << current_time << endl;
        current_time += 1;
        cout << "-) Current Process: " << "PID: "<< current_process.pid << " | Sisa Burst Time: " << current_process.sisa_burst_time << " | Priority: " << current_process.priority << endl;
        
        // Sisa burst time berkurang
        current_process.sisa_burst_time--;
        // tambah waktu proses di chart
        dataChart.back()++;

        // Prin header tabel ready jika ada proses di ready state
        cout << "-) Ready Process: " << endl;
        if(!pq.empty())
        {
            cout << "+----------------+---------------+---------------+---------------+---------------+" << endl;
            cout << "|ID Process      |Priority       |Arrival Time   |Waiting Time   |Burst Time     |" << endl;
            cout << "+----------------+---------------+---------------+---------------+---------------+" << endl;
        }

        // Save current priority queue
        priority_queue<Process, vector<Process>, ComparePriority> save_pq = pq;

        // Clear current priority queue untuk di update nilainya.
        priority_queue<Process, vector<Process>, ComparePriority>().swap(pq);
        bool is_first = true;
        while(!save_pq.empty())
        {
            // Print tabel ready
            cout << "|" << save_pq.top().pid << "\t\t " << "|" << save_pq.top().priority << "\t\t " << "|" << save_pq.top().arrival_time << "\t\t " << "|" << save_pq.top().waiting_time << "\t\t " << "|" << save_pq.top().init_burst_time << "\t\t " << "|" << endl;

            // Ambil sebuah nilai di save_pq agar bisa diedit
            Process temp = save_pq.top();
            save_pq.pop();

            // Edit value temp
            temp.waiting_time++;
            // Aging (tiap waiting 3 seconds)
            if(temp.waiting_time % 3 == 0 && temp.priority != 1)
            {
                temp.priority--;
            }

            // Balikan kembali ke queue
            pq.push(temp);

            // Ambil top priority di ready state
            if (is_first) {
                top_priority = temp.priority;
                is_first = false;
            } else if (temp.priority < top_priority) {
                top_priority = temp.priority;
            }
        }

        if(!pq.empty())
        {
            cout << "+----------------+---------------+---------------+---------------+---------------+" << endl;
        }

        // Jika current_process beres
        if (current_process.sisa_burst_time == 0) {
            // Set nilai atribut lain
            current_process.turnaround_time = current_time - current_process.arrival_time;
            total_waiting_time += current_process.waiting_time;
            total_turnaround_time += current_process.turnaround_time;
            // Masuk done
            done.push_back(current_process);
            run_empty = true;
            
            // loop berakhir jika pq kosong dan proses terakhir beres
            if (pq.empty()) {
                all_process_terminate = true;
            }
        } else {
            // Masuk pq (jika ada proses di ready yang lebih tinggi prioritasnya)
            if (top_priority < current_process.priority) {
                pq.push(current_process);
                run_empty = true;
            }
        }
    }

    // Menghitung rerata waiting time dan turnaround time
    double avg_waiting_time = (double) total_waiting_time / done.size();
    double avg_turnaround_time = (double) total_turnaround_time / done.size();

    cout << "---\n\n### 'DONE' TABLE ###\n";
    // Output tabel done
    cout << "+----------------+---------------+---------------+---------------+---------------+----------------+" << endl;
    cout << "|ID Process      |Priority       |Arrival Time   |Burst Time     |Waiting Time   |Turnaround Time |" << endl;
    cout << "+----------------+---------------+---------------+---------------+---------------+----------------+" << endl;
    for (auto process : done) {
        cout << "|" << process.pid << "\t\t " << "|" << process.priority_awal << "\t\t " << "|" << process.arrival_time << "\t\t " << "|" << process.init_burst_time << "\t\t " << "|" << process.waiting_time << "\t\t " << "|" << process.turnaround_time << "\t\t  "<< "|" << endl;
    }
    cout << "+----------------+---------------+---------------+---------------+---------------+----------------+" << endl;
    // Print the results
    cout << "Average Waiting Time: " << avg_waiting_time << endl;
    cout << "Average Turnaround Time: " << avg_turnaround_time << endl;
    
    cout << "\n### GANTT CHART ###\n";
    int spasi = 1;
    while (!dataChart.empty()) {
        int pid = dataChart.front();
        dataChart.pop_front();

        // Indentasi
        for (int i=0; i<spasi; i++) {
            cout << " ";
        }

        // Process bar
        cout << "[";
        for (int i=0; i<dataChart.front(); i++) {
            cout << "-";
        }
        cout << "] P" << pid << " (" << dataChart.front() << "s)\n";

        spasi += dataChart.front()+2;
        if (!dataChart.empty()) {
            dataChart.pop_front();
        }
    }
    cout << "\n";
}

int main() {
    // Proses awal
    Process processes[] = {
        {1, 8, 8, 5, 5, 1, 0, 0},
        {2, 1, 1, 3, 3, 0, 0, 0},
    };
    // Hituing banyak process.
    int n = sizeof(processes) / sizeof(processes[0]);
    // Set next ID
    next_id = n+1;

    // Panggil prosedur
    priorityScheduling(processes, n);

    return 0;
}
