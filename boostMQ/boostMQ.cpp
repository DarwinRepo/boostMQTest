// boostMQ.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <boost/timer/timer.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;
using namespace boost::timer;

cpu_timer myTimer;
int testTimes = 1;

void client()
{
    message_queue mq(open_only, "name");
    boost::ulong_long_type recvd_size;
    unsigned int priority;
    char buffer[20];
    while(1)
    {                                                           
        for ( int i = 0; i < testTimes; i++)
        {
            mq.receive(buffer, 20, recvd_size, priority);
        }
        myTimer.stop();
        auto cost_time = myTimer.elapsed();
        std::cout << "Cost time total = " << cost_time.wall / 1000000.0 << " ms.\n";
        std::cout << "Cost time per = " << cost_time.wall / 1000.0 / testTimes << " us.\n";
        Sleep(500);
    }
}

void server()  
{
    message_queue::remove("name");
    message_queue mq(open_or_create,"name",1000,20,permissions());
    char buffer[6] = "Hello";
    
    //start client
    std::thread clientThread{ client };

    while (1)
    {
        myTimer.start();
        for (int i = 0; i < testTimes; i++)
        {
            mq.send(buffer,6,0);
        }
        Sleep(3000);
    }
    clientThread.join();
}



int main()
{
    std::cout << "Hello BOOST MQ!\n";
    std::thread myTask{ server };
    myTask.join();
    std::cout << "Bye BOOST MQ!\n";
}


