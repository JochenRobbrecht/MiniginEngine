#pragma once
#include <iostream>
#include <fstream>
#include <thread>

//drag the json file to chrome://tracing to visualize

class Session {
private:
    std::ofstream m_OutputStream;
    bool m_first;
    Session() : m_first(true) {}

public:

    void BeginSession(const std::string& filepath = "measurements.json") {
        m_OutputStream.open(filepath);
        m_OutputStream << "{\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void EndSession() {
        m_OutputStream << "]}";
        m_OutputStream.flush();
        m_OutputStream.close();
        m_first = true;
    }

    void SaveMeasurement(const std::string& Name, int64_t Start, int64_t End, const std::thread::id& ThreadID) {
        if (m_first)
            m_first = false;
        else
            m_OutputStream << ",";

        m_OutputStream << "{" << "\"cat\":\"function\",\"dur\":" << (End - Start) << ",\"name\":\"" << Name << "\",";
        m_OutputStream << "\"ph\":\"X\",\"pid\":0," << "\"tid\":" << ThreadID << "," << "\"ts\":" << Start << "}";

        m_OutputStream.flush();
    }

    static Session& Get() {
        static Session instance;
        return instance;
    }
};

class Timer {
public:
    Timer(const std::string& name) : m_Name(name) {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        int64_t begin = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        int64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        Session::Get().SaveMeasurement(m_Name, begin, end, std::this_thread::get_id());
    }
private:
    std::string m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;

};

#define STR(s) #s
#define XSTR(s) STR(s)

#define MEASURE 1
#if MEASURE
#define TIME() Timer Time{__FUNCTION__ "-" XSTR(__LINE__)}
#else
#define TIME()
#endif
void doStuff1() {
    TIME();
    for (int i{}; i < 100; ++i) std::cout << i << std::endl;
}

void doStuff2() {
    TIME();
    for (int i{}; i < 200; ++i) std::cout << i << std::endl;
}

void doStuff2(int p) {
    TIME();
    for (int i{}; i < 200; ++i) std::cout << i * p << std::endl;
}

void BigFunc() {
    TIME();
    std::cout << "Think Big!\n";
    doStuff1();
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    doStuff2();
}

int main()
{
    Session::Get().BeginSession();
    doStuff1();
    std::thread myThread(BigFunc);
    doStuff2();
    BigFunc();
    doStuff2(3);
    Session::Get().EndSession();
    myThread.join();
}
