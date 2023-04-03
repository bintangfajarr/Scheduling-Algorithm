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

[ PRESENTASI ]
# Masukkin Proses ID di detik ke-2 (saat current time 1) dengan priority 7 dan burst time 8.

-) Menunjukkan Proses aging.
-) Menunjukkan Non-Pre-Emptive.
-) Menunjukkan FSCF ketika priority-nya sama

*/

#include <iostream>
#include <queue>
using namespace std;

struct Process {
    int pid; // Process ID
    int priority; // Process Priority
    int priority_awal; // Priority Awal Proses
    int init_burst_time; // Initial Process Burst Time
    int sisa_burst_time; // Sisa Burst Time Proses
    int waiting_time; // Process Waiting Time
    int arrival_time; // Process Arrival Time
    int turnaround_time; // Process Turnaround Time
};

// Process ID inputan baru (oleh sistem agar unique)
int next_id = 3;

// Comparison function to sort the processes by priority in Priority Queue
struct ComparePriority {
    bool operator()(const Process& p1, const Process& p2) {
        // Prioritas sama
        if(p1.priority == p2.priority)
        {
            return p1.pid > p2.pid;
        }
        // Prioritas beda
        else
        {
            return p1.priority > p2.priority;
        }
    }
};

// Function to implement Priority Scheduling Algorithm
void priorityScheduling(Process processes[], int n) {
    // Create a priority queue to store processes by priority
    priority_queue<Process, vector<Process>, ComparePriority> pq;
    // Vector Terminated Process
    vector<Process> done;

    // Push all the processes into the priority queue
    for (int i = 0; i < n; i++) {
        pq.push(processes[i]);
    }

    // First Process
    Process current_process = pq.top();
    pq.pop();

    // Calculate total waiting time and turnaround time
    int total_turnaround_time = 0;
    int total_waiting_time = current_process.init_burst_time;
    
    int current_time = 1;

    // Calculate turnaround time process
    current_process.turnaround_time = current_process.waiting_time + current_process.init_burst_time;
    // Push into table done
    done.push_back(current_process);

    int flag_all_process_terminate = 0;
    while (flag_all_process_terminate == 0) {
        // Input user
        Process new_process;
        // Cek input user "y/n"
        string flag;
        cout << "---\nApakah Anda ingin menginput proses baru (y/n)? ";
        cin >> flag;
        if (flag == "y") 
        {
            // Setup atribut new process.
            new_process.pid = next_id; next_id++;
            cout << "Masukkan prioritas proses: ";
            cin >> new_process.priority;
            new_process.priority_awal = new_process.priority;
            cout << "Masukkan inisial burst time proses: ";
            cin >> new_process.init_burst_time;
            new_process.sisa_burst_time = new_process.init_burst_time;
            new_process.waiting_time = 0;
            new_process.arrival_time = current_time;
            // Push new process into priority queue.
            pq.push(new_process);
        }

        // Kalau proses selesai dilakukan (terminate)
        if(current_time == total_waiting_time+1)
        {
            // Keluar dari while (Terminate proses terakhir)
            if(pq.empty())
            {
                flag_all_process_terminate = 1;
                break;
            }
            // Take top process on priority queue
            current_process = pq.top();
            pq.pop();

            // Calculate total waiting time
            total_waiting_time += current_process.init_burst_time;
            // Calculate turnaround time process
            current_process.turnaround_time = current_process.waiting_time + current_process.init_burst_time - 1;
            // Push into table done
            done.push_back(current_process);
        }
        // Save current priority queue for updating value process.
        priority_queue<Process, vector<Process>, ComparePriority> save_pq = pq;
        // Copy current priority queue buat diprint di console.
        priority_queue<Process, vector<Process>, ComparePriority> copy_pq = pq;
        
        // Output
        cout << "---\n-) Current Time: " << current_time << endl;
        current_time += 1;
        current_process.sisa_burst_time--;
        cout << "-) Current Process: " << "PID: "<< current_process.pid << " | Sisa Burst Time: " << current_process.sisa_burst_time << " | Priority: " << current_process.priority << endl;
        cout << "-) Ready Process: " << endl;

        // Output tabel ready
        if(!pq.empty())
        {
            cout << "+----------------+---------------+---------------+---------------+---------------+" << endl;
            cout << "|ID Process      |Priority       |Arrival Time   |Waiting Time   |Burst Time     |" << endl;
            cout << "+----------------+---------------+---------------+---------------+---------------+" << endl;
        }
        
        while (!copy_pq.empty()) {
            // Print nilai teratas pada priority queue.
            cout << "|" << copy_pq.top().pid << "\t\t " << "|" << copy_pq.top().priority << "\t\t " << "|" << copy_pq.top().arrival_time << "\t\t " << "|" << copy_pq.top().waiting_time << "\t\t " << "|" << copy_pq.top().init_burst_time << "\t\t " << "|" << endl;
            // Popping top value in cpq.
            copy_pq.pop();
        }

        if(!pq.empty())
        {
            cout << "+----------------+---------------+---------------+---------------+---------------+" << endl;
        }

        // Clear current priority queue untuk di update nilainya.
        priority_queue<Process, vector<Process>, ComparePriority>().swap(pq);

        // Update Value Priority Queue
        while(!save_pq.empty())
        {
            Process temp = save_pq.top();
            save_pq.pop();
            temp.waiting_time++;
            // Aging
            if(temp.waiting_time % 3 == 0 && temp.priority != 1)
            {
                temp.priority--;
            }
            pq.push(temp);
        }
    }

    total_waiting_time = 0;
    // Output tabel done.
    cout << "---\n\n### 'DONE' TABLE ### " << endl;
    cout << "+----------------+---------------+---------------+---------------+---------------+----------------+" << endl;
    cout << "|ID Process      |Priority       |Arrival Time   |Burst Time     |Waiting Time   |Turnaround Time |" << endl;
    cout << "+----------------+---------------+---------------+---------------+---------------+----------------+" << endl;
    for(int k = 0; k < done.size(); k++)
    {
        if(k != 0)
        {
            cout << "|" << done.at(k).pid << "\t\t " << "|" << done.at(k).priority_awal << "\t\t " << "|" << done.at(k).arrival_time << "\t\t " << "|" << done.at(k).init_burst_time << "\t\t " << "|" << done.at(k).waiting_time-1 << "\t\t " << "|" << done.at(k).turnaround_time << "\t\t  "<< "|" << endl;
            total_waiting_time += done.at(k).waiting_time-1;
        }
        else
        {
            cout << "|" << done.at(k).pid << "\t\t " << "|" << done.at(k).priority_awal << "\t\t " << "|" << done.at(k).arrival_time << "\t\t " << "|" << done.at(k).init_burst_time << "\t\t " << "|" << done.at(k).waiting_time << "\t\t " << "|" << done.at(k).turnaround_time << "\t\t  "<< "|" << endl;
            total_waiting_time += done.at(k).waiting_time;
        }
        total_turnaround_time += done.at(k).turnaround_time;
    }
    cout << "+----------------+---------------+---------------+---------------+---------------+----------------+" << endl;

    // Calculate average waiting time and turnaround time
    double avg_waiting_time = (double)total_waiting_time / n;
    double avg_turnaround_time = (double)total_turnaround_time / n;

    // Print the results
    cout << "Average Waiting Time: " << avg_waiting_time << endl;
    cout << "Average Turnaround Time: " << avg_turnaround_time << endl;
    
    // Print gantchart
    cout << "\n### GANTCHART ###" << endl;
    int space_process = 0;
    for(int k = 0; k < done.size(); k++)
    {
        // Indentasi
        for(int l = 0; l < space_process; l++)
        {
            cout << " ";
        }

        // Print balok proses
        cout << "[";
        for(int l = 0; l < done.at(k).init_burst_time; l++)
        {
            cout << "-";
        }
        cout << "] P" << done.at(k).pid << " (" << done.at(k).init_burst_time << "s)" << endl ;
        
        // Tambah total spasi
        space_process += done.at(k).init_burst_time + 2;
    }
    cout << "\n";


}

int main() {
    // Hardcode input data
    Process processes[] = {
        {1, 8, 8, 5, 5, 1, 0, 0},
        {2, 1, 1, 3, 3, 0, 0, 0},
    };
    // Hituing banyak process
    int n = sizeof(processes) / sizeof(processes[0]);

    // Panggil prosedur
    priorityScheduling(processes, n);

    return 0;
}
