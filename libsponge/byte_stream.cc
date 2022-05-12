#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity):_capacity(capacity),remain_capa(capacity)
{ 
    streamBUf.resize(capacity);
}


size_t ByteStream::write(const string &data) {
    size_t remain_cap=remaining_capacity();
    size_t data_len=data.length();
    if(data_len>=remain_cap)
    {
        remain_capa=0;
        for(size_t i=0;i<remain_cap;++i)
        {
            streamBUf[(write_cur+i)%_capacity]=data[i];
        }
        written+=remain_cap;
        write_cur=read_cur;
        stream_full=true;
        return remain_cap;
    }
    else{
        remain_capa-=data_len;
        for(size_t i=0;i<data_len;++i)
        {
            streamBUf[(write_cur+i)%_capacity]=data[i];
        }
        written+=data_len;
        write_cur=(write_cur+data_len)%_capacity;
        return data_len;
    }
    
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string res(len,'0');
    size_t unread=buffer_size();
    if(unread>=len)
    {
        for(size_t i=0;i<len;i++)
        {
            res[i]=streamBUf[(read_cur+i)%_capacity];
        }
    }
    else
    {
        for(size_t i=0;i<unread;i++)
        {
            res[i]=streamBUf[(read_cur+i)%_capacity];
        }
        
    }
    
    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    
    size_t unread_len=buffer_size();
    if(unread_len<=len)
    {
        readed+=unread_len;
        remain_capa=_capacity;
        read_cur=write_cur;
        stream_full=false;
    }
    else{
        remain_capa+=len;
        readed+=len;
        read_cur=(read_cur+len)%_capacity;
    }
    
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
        
    size_t unread_len=buffer_size();
    string res(len,'0');
    if(unread_len>=len)
    {
        remain_capa+=len;
        for(size_t i=0;i<len;i++)
        {
            res[i]=streamBUf[(read_cur+i)%_capacity];
        }
        read_cur=(read_cur+len)%_capacity;
        readed+=len;
    }
    else
    {
        remain_capa=_capacity;
        for(size_t i=0;i<unread_len;i++)
        {
            res[i]=streamBUf[(read_cur+i)%_capacity];
        }
        readed+=unread_len;
        read_cur=write_cur;
        stream_full=false;
    }
    
    return res;
}

void ByteStream::end_input() {
    input_end_flag=true;
}

bool ByteStream::input_ended() const { 
    return input_end_flag;
}

size_t ByteStream::buffer_size() const { 
    return _capacity-remain_capa;
 }

bool ByteStream::buffer_empty() const { 
    return buffer_size()==0;
}

bool ByteStream::eof() const {
    if(eof_signal)
        return true;
    if(buffer_empty())
    {
        if(input_end_flag)return true;
        return false;
    }
    
    return false;
}

size_t ByteStream::bytes_written() const { 
    return written;
}

size_t ByteStream::bytes_read() const { 
    return readed;
}

size_t ByteStream::remaining_capacity() const { 
    return remain_capa;
}
