#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity):max_size(capacity){}

size_t ByteStream::write(const string &data) {

    cout<<"bytestream::write is called"<<endl;
    /*cout<<"remaining capacity: "<<remaining_capacity()<<endl;
    cout<<"data: "+data+" length:"<<data.size()<<endl;
    cout<<"endl";*/

    int len=data.size();
    if(len>remaining_capacity())
    len=remaining_capacity();
    for(int i=0;i<len;i++){
        Buffer.push_back(data[i]);
    }
    write_cnt+=len;
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    DUMMY_CODE(len);
    string strtemp;
    int actual_len=len;
    if(len>Buffer.size())actual_len=Buffer.size();
    return strtemp.assign(Buffer.begin(),Buffer.begin()+actual_len);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    DUMMY_CODE(len); 
    int length=len;
    if(len>Buffer.size()) length=Buffer.size();
        read_cnt+=length;
    while(length){
        Buffer.pop_front();
        length--;

    }
    }

void ByteStream::end_input() {
    cout<<"end input called"<<endl;
    input_end=true;
}

bool ByteStream::input_ended() const { return input_end; }

size_t ByteStream::buffer_size() const { return Buffer.size(); }

bool ByteStream::buffer_empty() const { return Buffer.size()==0; }

bool ByteStream::eof() const { return buffer_empty()&&input_ended() ;}

size_t ByteStream::bytes_written() const { return write_cnt; }

size_t ByteStream::bytes_read() const { return read_cnt; }

size_t ByteStream::remaining_capacity() const { return max_size-Buffer.size(); }
