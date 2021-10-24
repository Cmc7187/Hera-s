#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity)
    , _time_out(retx_timeout) {}

uint64_t TCPSender::bytes_in_flight() const { return _bytes_in_flight; }

void TCPSender::fill_window() {

    cout<<"----------------------fill window start-----------------------"<<endl;
    cout<<"_abs_ack="<<_abs_ack<<"  _next_seq="<<_next_seqno<<"  windowsize="<<_windowsize
                <<"   buffersize="<<_stream.buffer_size()<<"    stream_eof="<< _stream.eof()<< endl;


    if(!_syn){//hasn't send syn
        _syn=true;
        TCPSegment seg;
        seg.header().syn=true;
        seg.header().seqno=wrap(0,_isn);
        send_data(seg);
        cout<<"----------------------fill window end-----------------------"<<endl;
        return;
        
    }
    int datalen;
    int remaining_size;
    if(_windowsize==0)_windowsize=1;
        remaining_size=_windowsize-_bytes_in_flight;
        if(remaining_size<=0){
            cout<<"remainingsize=0,return"<<endl;
            cout<<"----------------------fill window end-----------------------"<<endl;
            return;
        }
    if(_stream.buffer_size()){//there is data to be sent
        datalen=_stream.buffer_size()>remaining_size?remaining_size:_stream.buffer_size();
        if(datalen>TCPConfig::MAX_PAYLOAD_SIZE) datalen=TCPConfig::MAX_PAYLOAD_SIZE;
        string str=_stream.read(datalen);
        TCPSegment seg;
        seg.header().seqno=wrap(_next_seqno,_isn);
        seg.payload()=Buffer(std::move(str));
        if(_stream.eof()&&(remaining_size-datalen>0)&&!_fin){
            seg.header().fin=true;
            _fin=true;
        }
        send_data(seg);
        cout<<"----------------------fill window end-----------------------"<<endl;
        return;
    }else{//determine send fin or not,there is no data left
        if(_stream.eof()&&(remaining_size>0)&&!_fin){
            _fin=true;//make sure fin flag will be sent only onece
            TCPSegment seg;
            seg.header().fin=true;
            seg.header().seqno=wrap(_next_seqno,_isn);
            send_data(seg);
            cout<<"----------------------fill window end-----------------------"<<endl;
            return;
        }
    }

    cout<<"seg wasn't send"<<endl;

    cout<<"----------------------fill window end-----------------------"<<endl;

}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
//! \returns `false` if the ackno appears invalid (acknowledges something the TCPSender hasn't sent yet)
bool TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    //DUMMY_CODE(ackno, window_size);
    cout<<"----------------------------receive ack start--------------------------------"<<endl;
    uint64_t temp_abs_ack=unwrap(ackno,_isn,_abs_ack);
    cout<<"_abs_ack_received="<<temp_abs_ack<<"  pre_abs_ack="<<_abs_ack<<"   _next_seq="<<_next_seqno
                <<"  windowsize="<<window_size<<endl;
    if(temp_abs_ack>_next_seqno) {
        cout<<"ack was invalid,return false"<<endl;
        cout<<"----------------------------receive ack end--------------------------------"<<endl;
        return false;//acknowledge something hasn't sent
    }
    if(temp_abs_ack<=_abs_ack) {
        cout<<"ack didn't upgrate ack"<<endl;
        _windowsize=window_size;
        cout<<"----------------------------receive ack end--------------------------------"<<endl;
        return true;//didn't update left but is valid,do nothing
    }
    else{//ack>_abs_ack,successful ack new data

        cout<<"ack is upgrated"<<endl;
 
        _bytes_in_flight-=temp_abs_ack-_abs_ack;
        _windowsize=window_size;
        _abs_ack=temp_abs_ack;
        _timer=0;
        _time_out=_initial_retransmission_timeout;
        _retransmit_times=0;

        while(!_outstanding_segments.empty()){
            TCPSegment seg=_outstanding_segments.front();
           if( unwrap(seg.header().seqno,_isn,_abs_ack)+seg.length_in_sequence_space()>_abs_ack){
               //last seg wasn't fully acked,can't pop
               break;
           }
           else{
            _outstanding_segments.pop();
           }
        }
        if(_outstanding_segments.empty()) _time_started=false;
        fill_window();
        cout<<"----------------------------receive ack end--------------------------------"<<endl;
        return true;
    }
    
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
     //DUMMY_CODE(ms_since_last_tick); 
     if(_time_started){
        _timer+=ms_since_last_tick;
        if(_timer>=_time_out){
            //timeout,retransmit
            TCPSegment seg=_outstanding_segments.front();
            cout<<"timeout,retransmit segment"<<seg.header().to_string()<<"payload:"<<seg.payload().copy()<<endl;
            _segments_out.push(seg);
            _retransmit_times++;
            _timer=0;
            _time_out=_time_out*2;
        }
     }
     }

unsigned int TCPSender::consecutive_retransmissions() const { return _retransmit_times; }

void TCPSender::send_empty_segment() {
    TCPSegment seg;
    seg.header().seqno=wrap(_next_seqno,_isn);
    _segments_out.push(seg);
}

void TCPSender::send_data(const TCPSegment &seg){
    cout<<"--------------------------send_data start-----------------------------"<<endl;
    _next_seqno+=seg.length_in_sequence_space();
    _bytes_in_flight+=seg.length_in_sequence_space();
    if(!_time_started){
        _time_started=true;
        _timer=0;
    }
    _segments_out.push(seg);
    _outstanding_segments.push(seg);
    cout<<"segment was sent:"<<seg.header().to_string()<<"payload:"<<seg.payload().copy()<<endl;
    cout<<"--------------------------send_data end-----------------------------"<<endl;
}
