#pragma once
#include <cstdint>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "stdio.h"
using namespace std;


class DoubleBuffer{
    public:
        DoubleBuffer(int size=0);
        ~DoubleBuffer();
        bool write(int pos, int value);
        int read(int pos);
        int wait_until_full(int called_sequence);

    private:
        int m_size;
        vector<int> *m_firstBuffer;
        vector<int> *m_secondBuffer;
        bool m_full;
        int m_sequence;

        mutex m_lock;
        condition_variable m_cvfull;
};