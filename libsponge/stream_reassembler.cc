#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;


StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity),unassembled{},last_reassembler{0}{}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if(eof)
    {
        if(index+data.length()>=1&&index+data.length()>=index&&index+data.length()>=data.length())
        {
            eof_char=index+data.length()-1;
        }
        else if(index+data.length()<index&&index+data.length()<data.length())
        {
            eof_char=SIZE_MAX;
        }
        else 
        {
            eof_char=0;
        }
        
    }
    if(data=="")
    {
        
    }
    else if(index==0)
    {
        if(last_reassembler==index&&_output.bytes_written()==0)
        {
            _output.write(data);
            last_reassembler=_output.bytes_written()-1;
            for(auto it=unassembled.begin();it!=unassembled.end();it++)
                push_substring(it->second,it->first,0);
        }
        else
        {
            if(index+data.length()-1>last_reassembler)
            {
                _output.write(data.substr(last_reassembler+1));

                last_reassembler=_output.bytes_written()-1;
                
                for(auto it=unassembled.begin();it!=unassembled.end();it++)
                    push_substring(it->second,it->first,0);
            }
            
        }
        
    }
    else if(last_reassembler+1==index&&_output.bytes_written()>0)
    {

        _output.write(data);
        last_reassembler=_output.bytes_written()-1;
        if(unassembled.count(index))
            unassembled[index]="";
        for(auto it=unassembled.begin();it!=unassembled.end();it++)
            push_substring(it->second,it->first,0);
    }
    else if(last_reassembler>=index)
    {
        if(index+data.length()-1>last_reassembler)
        {
            //0123
            // 1234

            _output.write(data.substr(last_reassembler-index+1));

            last_reassembler=_output.bytes_written()-1;
            if(unassembled.count(index))
                unassembled[index]="";
            for(auto it=unassembled.begin();it!=unassembled.end();it++)
                push_substring(it->second,it->first,0);
        }else
        {
            if(unassembled.count(index))
                unassembled[index]="";
        }
        

    }   
    else
    {
        unassembled[index]=data;
    }

    if(last_reassembler==eof_char)
    {
        _output.eof_signal=true;
    }
               
    
}

size_t StreamReassembler::unassembled_bytes() const {
    if(unassembled.size()==0)
        return 0; 
    if(unassembled.size()==1)
        return unassembled.begin()->second.length();
    size_t first=SIZE_MAX;size_t last=0;
    for(auto it=unassembled.begin();it!=unassembled.end();it++)
    {
        if(it->first<first)first=it->first;
        if(it->first+it->second.length()-1>last)last=it->first+it->second.length()-1;
    }
    std::vector<size_t>count(last-first+1);
    for(auto it=unassembled.begin();it!=unassembled.end();it++)
    {
        for(size_t i=it->first;i<it->first+it->second.length();++i)
            count[i-first]=1;
    }
    size_t unassembled_bits=0;
    for(size_t i=0;i<last-first+1;++i)
    {
        if(count[i]==1)
            unassembled_bits+=count[i];
    }

    return unassembled_bits;

}

bool StreamReassembler::empty() const {
    return unassembled.size()==0;
}

