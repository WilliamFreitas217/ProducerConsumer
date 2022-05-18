#pragma once
#include <cstdint>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "stdio.h"
using namespace std;


class DoubleBuffer{
    public:
        DoubleBuffer(uint16_t size=0);
        ~DoubleBuffer();
        bool write(uint16_t pos, int16_t value);
        int16_t read(uint16_t pos);
        int16_t wait_until_full(int16_t called_sequence);

    private:
        uint16_t m_size;
        vector<int16_t> *m_firstBuffer;
        vector<int16_t> *m_secondBuffer;
        bool m_full;
        int16_t m_sequence;

        mutex m_lock;
        condition_variable m_cvfull;
};