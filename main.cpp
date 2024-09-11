#include <string>
#include <cstring>
#include <stack>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iomanip> 
#include <vector>


using namespace std;

struct task{
int
    arrival_time,
    service_time,
    remaining_time,
    completion_time,
    response_time,
    wait_time,
    listID;
};

void FIFO(vector<task>);
void SJF(vector<task>);
void roundRobin(vector<task>);
void printStats(vector<task>);


int main(int argc, char* argv[])
{
    
    string process = argv[1];   //identifies what type of process either FIFO, RR, or SJF
    process.erase(0, 1);    //erases the dash
    


    for (size_t i = 0; i < process.size(); ++i) //capitalizes string
    {
        process[i] = toupper(process[i]);
    }
    
    vector<task> tasks;


    task temp; //temporary task variable to parse through input
    int inputData;  //the integer scanned in from input file
    bool inputFlag = true;  //a flag that determines whether int scanned in is associated with the arrival time or service time

    while (cin >> inputData)
    {
        if (inputFlag)
        {
            temp.arrival_time = inputData;
            inputFlag = false;
        } else
        {
            temp.service_time = inputData;
            tasks.push_back(temp);
            inputFlag = true;

        }
    }
    cin.clear();
    cout.clear();

    if (process.compare("SJF") == 0)
        process = "SJF(preemptive)";

    if (process == "RR")
    {
        cout << process << " scheduling results (time slice is 1)\n\n";
    } else
    {
         cout << process << " scheduling results\n\n";
    }

  
    cout << "time   cpu   ready queue (tid/rst)\n";
    cout << "----   ---   ---------------------\n";
    

    if (process == "FIFO")
    {
        FIFO(tasks);
    } else if (process == "SJF(preemptive)")
    {
        SJF(tasks);
    } else if (process == "RR")
    {
        roundRobin(tasks);
    } else
    {
        cout << "Invalid type of scheduling process\n";
        return -1;
    }
}

void FIFO(vector<task> tasks)
{
    vector<task> originalTasks = tasks;

    int finishedTasks = 0;
    string cpu = "";
    char cpuLetter = 'A';
    vector<task> readyQueue;
    string printedReadyQueue = "";

    for (size_t i = 0; i < tasks.size(); ++i)
    {
        tasks[i].remaining_time = tasks[i].service_time;
        tasks[i].listID = i;
    }

    for (int time = 0;;++time)  //loop with incremental variable keeping track of 'time'
    {
        if (finishedTasks >= (int)tasks.size())
            break; 

        for (size_t i = 0; i < tasks.size(); ++i)
        {
            if (tasks[i].arrival_time == time)
                readyQueue.push_back(tasks[i]);
        }

        if (readyQueue.size() >= 1)
        {
            cpu = "";
            cpuLetter = readyQueue[0].listID + 'A';
            cpu = cpuLetter + to_string(readyQueue[0].remaining_time);

            readyQueue[0].remaining_time -= 1;

           

            printedReadyQueue.clear();
            for (size_t i = 1; i < readyQueue.size(); ++i)  //this loop prints out what processes are queued up 
            {
                cpuLetter = readyQueue[i].listID + 'A';
                printedReadyQueue += cpuLetter;
                printedReadyQueue += to_string(readyQueue[i].remaining_time);
                if (i != readyQueue.size() - 1)
                    printedReadyQueue += ", ";
            }

            if (readyQueue.size() <= 1)
                printedReadyQueue = "--";

            if (readyQueue[0].remaining_time == 0)
            {
                tasks[readyQueue[0].listID].completion_time = time + 1;
                tasks[readyQueue[0].listID].response_time = tasks[readyQueue[0].listID].completion_time - tasks[readyQueue[0].listID].arrival_time;
                tasks[readyQueue[0].listID].wait_time = tasks[readyQueue[0].listID].response_time - tasks[readyQueue[0].listID].service_time;
                readyQueue.erase(readyQueue.begin());
                finishedTasks++;
            } 
            
        } else
        {
            cpu = "";
            printedReadyQueue = "--";
        }
    
    cout << right << setw(3) << time << "    " << left << setw(3) << cpu << "   " << left << printedReadyQueue << "\n";

    }
    printStats(tasks);

}

void SJF(vector<task> tasks)
{
    string cpu = "";
    char cpuLetter = 'A';
    int currentTask = 0;
    vector<task> readyQueue;
    string printedReadyQueue = "";

    for (size_t i = 0; i < tasks.size(); ++i)
    {
        tasks[i].remaining_time = tasks[i].service_time;
        tasks[i].listID = i;
    }


    for (int time = 0;; ++time)
    {

        for (size_t x = currentTask; x < tasks.size(); ++x)
        {
            if (tasks[x].arrival_time == time)
            {
                readyQueue.push_back(tasks[x]);
            }
        }

        if (readyQueue.size() > 1){
            task temp;
            bool swapped;
            for (size_t i = 0; i < readyQueue.size() - 1; i++) {
                swapped = false;
                for (size_t j = 0; j < readyQueue.size() - i - 1; j++) {
                    if (readyQueue[j].remaining_time > readyQueue[j + 1].remaining_time) {
                        temp = readyQueue[j];
                        readyQueue[j] = readyQueue[j+1];
                        readyQueue[j+1] = temp;
                        swapped = true;
                    }
                }

            if (swapped == false)
                break;
            }
        }
        
        if (readyQueue.size() != 0)
        {
            if (readyQueue[0].remaining_time == 0)
            {
                tasks[readyQueue[0].listID].completion_time = time;
                tasks[readyQueue[0].listID].response_time = tasks[readyQueue[0].listID].completion_time - tasks[readyQueue[0].listID].arrival_time;
                tasks[readyQueue[0].listID].wait_time = tasks[readyQueue[0].listID].response_time - tasks[readyQueue[0].listID].service_time;
                currentTask++;
                readyQueue.erase(readyQueue.begin());

                if (readyQueue.size() == 0)
                {
                    if (currentTask == (int)tasks.size())
                        break;
                    
                    cout << right << setw(3) << time << "          --\n";
                    continue;
                }
                    
            }
            
            cpu = "";
            cpuLetter = readyQueue[0].listID + 'A';
            cpu = cpuLetter + to_string(readyQueue[0].remaining_time);

            readyQueue[0].remaining_time -= 1;

            
            printedReadyQueue.clear();
            for (size_t i = 1; i < readyQueue.size(); ++i)  //this loop prints out what processes are queued up 
            {
                cpuLetter = readyQueue[i].listID + 'A';
                printedReadyQueue += cpuLetter;
                printedReadyQueue += to_string(readyQueue[i].remaining_time);
                if (i != readyQueue.size() - 1)
                    printedReadyQueue += ", ";
            }

            if (readyQueue.size() <= 1)
            {printedReadyQueue = "--";}
        }
        else
        {
            cpu = "";
            printedReadyQueue = "--";
        }

        cout << right << setw(3) << time << "    " << left << setw(3) << cpu << "   " << left << printedReadyQueue << "\n";

    }

    printStats(tasks);

}

void roundRobin(vector<task> tasks)
{


    
    string cpu = "";
    char cpuLetter = 'A';
    int finishedTasks = 0;

    vector<task> readyQueue;
    string printedReadyQueue = "";
 
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        tasks[i].remaining_time = tasks[i].service_time;
        tasks[i].listID = i;
    }

    task temp;
    for (int time = 0;; ++time)
    {
     
        if (finishedTasks >= (int)tasks.size())
       {

           break; 
       }
      
            
        for (size_t i = 0; i < tasks.size(); ++i)
        {
            if (tasks[tasks.size() - i - 1].arrival_time == time)
                readyQueue.push_back(tasks[tasks.size() - i - 1]);
        }

      if (readyQueue.size() >= 1)
      {

        temp = readyQueue[0];
        readyQueue.erase(readyQueue.begin());
        
        readyQueue.push_back(temp);
        temp = readyQueue[0];


       
        if (temp.remaining_time > 0)
        {cpu = "";
        cpuLetter = temp.listID + 'A';
        cpu = cpuLetter + to_string(temp.remaining_time);

        if (temp.remaining_time == 1)
        {
            tasks[temp.listID].completion_time = time + 1;
            finishedTasks++;
        }
            


        printedReadyQueue = "";
      
      
        for (size_t i = 1; i < readyQueue.size(); ++i)  //this loop prints out what processes are queued up 
        {
           if (i != 1 && readyQueue[i].remaining_time != 0)
            {printedReadyQueue += ", ";}
           
            if (readyQueue[i].remaining_time > 0)
            {
            cpuLetter = readyQueue[i].listID + 'A';
            printedReadyQueue += cpuLetter;
            printedReadyQueue += to_string(readyQueue[i].remaining_time);
            }
        }

       if (printedReadyQueue.compare("") == 0)
            printedReadyQueue = "--";


        bool isClear = true;
        for (size_t i = 0; i < readyQueue.size(); ++i)
        {
            if (readyQueue[i].remaining_time != 0)
                isClear = false;
        }
    
        if (readyQueue.size() <= 1 || isClear)
            {printedReadyQueue = "--";}

        } else
        {
            cpu = "";
            printedReadyQueue = "--";
         }

    
        readyQueue[0].remaining_time -= 1;


        if (readyQueue[readyQueue.size() -1].remaining_time < 1)
        {
            
            readyQueue.pop_back();
    
        }
      } else
      {
        cpu = "";
        printedReadyQueue = "--";
      }

        cout << right << setw(3) << time << "    " << left << setw(3) << cpu << "   " << left << printedReadyQueue << "\n";

    }

    for (size_t i = 0; i < tasks.size(); ++i)
    {
        tasks[i].response_time = tasks[i].completion_time - tasks[i].arrival_time;
        tasks[i].wait_time = tasks[i].response_time - tasks[i].service_time;
           
    }


    printStats(tasks);
}

void printStats(vector<task> tasks)
{
    //prints out the stats
    cout << "\n     arrival service completion response wait\n";
    cout << "tid   time    time      time      time   time\n";
    cout << "---  ------- ------- ---------- -------- ----\n";
    char cpuLetter = 'A';
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        cout << right;
        cout << " " << cpuLetter << "   " << setw(4) << tasks[i].arrival_time << "    "<< setw(4) << tasks[i].service_time;
        cout << "    "<< setw(6) << tasks[i].completion_time << "     " << setw(5) << tasks[i].response_time << "    " << setw(3) << tasks[i].wait_time << "\n";
        cpuLetter += 1;
    }

    cout << "\nservice wait\n";
    cout << " time   time\n";
    cout << "------- ----\n";
    
    task temp;
    bool swapped;
    for (size_t i = 0; i < tasks.size() - 1; i++) {
        swapped = false;
        for (size_t j = 0; j < tasks.size() - i - 1; j++) {
            if (tasks[j].service_time > tasks[j + 1].service_time) {
                temp = tasks[j];
                tasks[j] = tasks[j+1];
                tasks[j+1] = temp;
                swapped = true;
            }
        }

        if (swapped == false)
            break;
    }

    for (size_t i = 0; i < tasks.size(); ++i)
    {
        cout << right;
        cout << setw(4) << tasks[i].service_time << "    " << setw(3) << tasks[i].wait_time << "\n";
    }
}