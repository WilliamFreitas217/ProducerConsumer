#pragma once
#include <cstdint>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "stdio.h"
using namespace std;


class Buffer{
    public:
        Buffer(int size=0);
        bool write(int pos, int value);
        int wait_until_full(int called_sequence);

    private:
        int m_size;
        vector<int> *m_buffer;
        bool m_full;
        int m_sequence;

        mutex m_lock;
        condition_variable m_cvfull;
};