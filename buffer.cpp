//
// Created by williamfreitas on 15/05/22.
//
#include "buffer.h"

using namespace std;

Buffer::Buffer(int size)
        : m_size(size)
        , m_full(false)
        , m_sequence(0)
{
    m_buffer = new vector<int>;
    m_buffer->resize(size);
}

bool Buffer::write(int pos, int value)
{
    unique_lock<mutex> l(m_lock); //
    if(pos < m_size) {
        (*m_buffer)[pos]=value;
        if (pos == m_size-1) { // Verifica se o buffer está cheio
            ++m_sequence; // Incrementa a sequência
            m_full = true;
            m_cvfull.notify_all();
        }
        else { // Caso o buffer não esteja cheio
            m_full = false;
        }
        // Posição válida dentro do buffer
        return true;
    }
    else {
        // Posição não válida dentro do buffer
        return false;
    }
}

int Buffer::wait_until_full(int called_sequence){
    unique_lock<mutex> l(m_lock); //
    while(!(m_full && m_sequence != called_sequence)) {
        m_cvfull.wait(l); // espera o buffer ficar cheio
    }
    return m_sequence;
}