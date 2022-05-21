#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { return _sender.stream_in().remaining_capacity(); }

size_t TCPConnection::bytes_in_flight() const { return _sender.bytes_in_flight(); }

size_t TCPConnection::unassembled_bytes() const { return _receiver.unassembled_bytes(); }

size_t TCPConnection::time_since_last_segment_received() const { return {}; }

void TCPConnection::segment_received(const TCPSegment &seg) {
     //DUMMY_CODE(seg); 

     if(!active()){
         return;
     }

     if(LISTEN()){
         if(!seg.header().fin||seg.payload().size()) return;//在listen状态下只能对第一次握手内容作出回应
         _receiver.segment_received(seg);
         connect();
         return;
        //此过程过后服务器进入SYN_RECEIVE状态，这个状态没有办法主动的发送数据
     }

    //总的过程就是接收syn和ack，发送ack
     if(SYN_SENT()){//客户端的连接的一种状态
        if(seg.payload().size()) return;//二次握手的报文不应该携带报文
            _receiver.segment_received(seg);
            _sender.ack_received(seg.header().ackno,seg.header().win);

        if(_sender.stream_in().buffer_size()){//有数据发送
            _sender.fill_window();
            add_ack();
        }else{//没有数据，我们要构造一个空的seg来将ack发回去
            _sender.send_empty_segment();//这里要注意的是这个空的ack，如果分组丢失的话没有办法去重传。
            add_ack();
            //这种情况下的分组丢失只能依靠于下一次的数据传输将ack带回去让服务器进入establish状态，
            //如果客户端之后一直没有数据传输，服务器又无法发送数据，只能不断超时重传第二次握手内容，最终导致RST
        }
        return;
        //客户端进入establish状态
     }

     //接下来的情况：客户端在establish,服务器在establish或者syn_receive的状态
     //即使服务器处于syn_receive的状态也是可以接受数据的，并且在接受之后应该都是可以发送数据了。
     //并且应该要注意的是当我们只接收到ack（即不占有seqno的分组）的时候不需要进行确认。
     //本身没有数据需要发送的时候，不通过fillwindow函数发送，而是通过send_empty_segment()发送

    if(seg.length_in_sequence_space()==0&&seg.header().ack){//收到一个空的包，只携带ack报文
        _sender.ack_received(seg.header().ackno,seg.header().win);
        if(_sender.stream_in().buffer_size()){//不用发空包
            _sender.fill_window();
        }
    }    
     }

bool TCPConnection::active() const { return _active; }

size_t TCPConnection::write(const string &data) {
    DUMMY_CODE(data);
    return {};
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) { DUMMY_CODE(ms_since_last_tick); }

void TCPConnection::end_input_stream() {}

void TCPConnection::connect() {//对于双方第一次发送连接请求时候调用
    if(!active()){
        return;
    }
    _sender.fill_window();//第一次fillwindow会发送syn,并将sender的syn置为true
    add_ack();
}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";

            // Your code here: need to send a RST segment to the peer
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}
void TCPConnection::add_ack(){
    TCPSegment seg;
    while(_segments_out.size()){
    seg=_segments_out.front();
    _segments_out.pop();
    if(_receiver.ackno().has_value()){
        seg.header().ack=true;
        seg.header().ackno=_receiver.ackno().value();
        seg.header().win=_receiver.window_size()>UINT16_MAX?UINT16_MAX:_receiver.window_size();
    }
    _segments_out.push(seg);
    }
    while(_sender._outstanding_segments.size()){//已经发送而且没有确认
        seg=_sender._outstanding_segments.front();
        _sender._outstanding_segments.pop();
        if(_receiver.ackno().has_value()){
        seg.header().ack=true;
        seg.header().ackno=_receiver.ackno().value();
        seg.header().win=_receiver.window_size()>UINT16_MAX?UINT16_MAX:_receiver.window_size();
    }
    _sender._outstanding_segments.push(seg);
    }
}