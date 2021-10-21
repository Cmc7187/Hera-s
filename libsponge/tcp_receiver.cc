#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg) {
    
    cout<<"tcp receive seg called"<<endl;
    cout<<"报文如下："<<endl;
    cout<<"header:\n"<<seg.header().to_string()<<endl;
    cout<<"data:"<<seg.payload().copy()<<endl;
    cout<<endl;


    DUMMY_CODE(seg);
    if(seg.header().syn){//报文是开启报文的情况
        //连接开启
        if(_syn) return false;//两次开启
        _syn=true;
        _isn=seg.header().seqno;
        _reassembler.push_substring(seg.payload().copy(),  unwrap(seg.header().seqno,_isn,_checkpoint)-1,
                 seg.header().fin   );//fin和syn同时传来的情况？？
        if(seg.header().fin) _fin=true;
        return true;
    }
    if(_syn){//接下来的情况都是在连接状态开启的条件下
        if(_fin&&seg.header().fin) return false;
        if(seg.header().fin&&!_fin)_fin=true;
        _checkpoint=_reassembler.get_firstureassembled();
        uint64_t absseqno=unwrap(seg.header().seqno,_isn,_checkpoint);
        uint64_t index=absseqno-1;
        uint64_t length=seg.length_in_sequence_space();
        string data=seg.payload().copy();
        size_t buffersize=_reassembler.stream_out().buffer_size();//我们在push以后数据会变，先保存。

        /*cout<<"报文如下："<<endl;
        cout<<"header:\n"<<seg.header().to_string()<<endl;
        cout<<"data:"<<seg.payload().copy()<<endl;*/
        cout<<"-----------------------------------------------------------------"<<endl;
        cout<<"absseq:"<<absseqno<<"    index:"<<index<<endl;
        cout<<"firstunreassembled："<<_reassembler.get_firstureassembled()<<"   firstunread:"
                <<_reassembler.get_firstureassembled()-_reassembler.stream_out().buffer_size()<<endl;
        cout<<endl;

        if(seg.header().fin){//传递报文并且提出关闭请求的时候
            _reassembler.push_substring(data,index,true);
            return true;//不管怎么说fin标志是送到了
        }else{
            _reassembler.push_substring(data,index,false);

            if(index<_checkpoint){
                if(index+length>_checkpoint) return true;
                else return false;
            }else{
                if(index>=_checkpoint-buffersize+_capacity)
                return false;
                else return true;
            }
        }

        //还没有请求连接就传数据
  }else {
      _reassembler.push_substring(seg.payload().copy(),  unwrap(seg.header().seqno,_isn,_checkpoint)-1,
                 seg.header().fin   );//防止第一个就传来结束标志
      return false;
  }
}

optional<WrappingInt32> TCPReceiver::ackno() const { 

    cout<<"ackno is called"<<endl;


    if(_syn){
        if(_fin)  return wrap(_reassembler.get_firstureassembled()+2,_isn);//fin标志也算在内
        return wrap(_reassembler.get_firstureassembled()+1,_isn);
    }else return std::nullopt;
 }

size_t TCPReceiver::window_size() const { 
    cout<<"windowsize() is called"<<endl;

    return _capacity-_reassembler.stream_out().buffer_size();
     }
