//
// Created by williamfreitas on 15/05/22.
//
#include "doublebuffer.h"

using namespace std;

DoubleBuffer::DoubleBuffer(int size)
        : m_size(size)
        , m_full(false)
        , m_sequence(0)
{
    m_firstBuffer = new vector<int>;
    m_secondBuffer = new vector<int>;
    m_firstBuffer->resize(size);
    m_secondBuffer->resize(size);
}

bool DoubleBuffer::write(int pos, int value)
{
    unique_lock<mutex> l(m_lock);
    if(pos < m_size) {
        (*m_firstBuffer)[pos]=value;
        if (pos == m_size-1) {
            if(++m_sequence < 0) m_sequence = 0;
            swap(m_firstBuffer, m_secondBuffer);
            m_full = true;
            m_cvfull.notify_all();
        }
        else {
            m_full = false;
        }
        return true;
    }
    else {
        return false;
    }
}

int  DoubleBuffer::wait_until_full(int called_sequence){
    unique_lock<mutex> l(m_lock);
    while(!(m_full && m_sequence != called_sequence)) {
        m_cvfull.wait(l);
    }
    return m_sequence;
}