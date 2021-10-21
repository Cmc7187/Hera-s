#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <cstdint>
#include <string>
#include<set>
#include <iostream>
using namespace std;
//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:
  struct Seg{
    uint64_t index;
    string data;
    uint64_t length;
    Seg(){};
    Seg(const string &_data, uint64_t _index):index(_index),data(_data){
      length=_data.size();
    }
    bool operator<(const Seg &seg)const{
      return index<seg.index;
    }
    void print(){
      cout<<"index:"<<index<<"  data:"+this->data<<"  length:"<<length<<endl;
    }
  };
    // Your code here -- add private members as necessary.

    ByteStream _output;  //!< The reassembled in-order byte stream
    size_t _capacity;    //!< The maximum number of bytes
    set<Seg> cached_substring; // 缓存的子串
    uint64_t first_unreassembled=0;
    size_t unreassemblerd_bytes_num=0;//缓存但还未排序的字节数
    size_t maxindex;
    bool end_flag=false;
  public:
    //! \brief Construct a `StreamReassembler` that will store up to `capacity` bytes.
    //! \note This capacity limits both the bytes that have been reassembled,
    //! and those that have not yet been reassembled.
    StreamReassembler(const size_t capacity);

    //! \brief Receives a substring and writes any newly contiguous bytes into the stream.
    //!
    //! If accepting all the data would overflow the `capacity` of this
    //! `StreamReassembler`, then only the part of the data that fits will be
    //! accepted. If the substring is only partially accepted, then the `eof`
    //! will be disregarded.
    //!
    //! \param data the string being added
    //! \param index the index of the first byte in `data`
    //! \param eof whether or not this segment ends with the end of the stream
    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    //用来合并有重叠部分的子串
    // seg1是待插入子串，seg2是原来在set的子串
    //! \returns 返回是否合并成功
    bool merger(Seg &seg1,const Seg &seg2);

    //用来删除set中的元素
    void remove(set<Seg>::iterator it){
      unreassemblerd_bytes_num-=it->length;
      cached_substring.erase(it);
    }

    void insert(const Seg &seg){
      cached_substring.insert(seg);
      unreassemblerd_bytes_num+=seg.length;
    }
    
    //! \return 写入的字节数目
    size_t wtriteToStream();

    //! \name Access the reassembled byte stream
    //!@{
    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }
    //!@}

    //! The number of bytes in the substrings stored but not yet reassembled
    //!
    //! \note If the byte at a particular index has been submitted twice, it
    //! should only be counted once for the purpose of this function.
    size_t unassembled_bytes() const;

    //! \brief Is the internal state empty (other than the output stream)?
    //! \returns `true` if no substrings are waiting to be assembled
    bool empty() const;

    uint64_t get_firstureassembled()const{return first_unreassembled;}
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
