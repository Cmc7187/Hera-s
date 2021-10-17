#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
//只接受不超过最大值的字节流
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);
    cout<<endl;
    cout<<"first_unreassmbled:"<<this->first_unreassembled<<"\ndata:"+data<<"\nindex:"<<index<<"\neof:"<<eof<<endl;
    string tp_data=data,tp_data2;
    size_t tp_index=index;
    maxindex=first_unreassembled-_output.buffer_empty()+_capacity;
    if(index>=maxindex){
            if(eof) end_flag=true;
            if(eof&&empty()) _output.end_input();
            return;
    } 
    if(data.size()==0||(index+data.size()<=first_unreassembled)){//判断是否有空子串或是冗余数据到达
        if(eof) end_flag=true;
            if(eof&&empty()) _output.end_input();
            return;
    }
    if(index<first_unreassembled){
        //开头需要裁减
        size_t offset=first_unreassembled-index;
        tp_index=first_unreassembled;
        tp_data.assign(data.begin()+offset,data.end());
    }
    if(tp_index+tp_data.size()>=maxindex){//在开头修改完之后的基础看末尾是否需要修改
        //末尾需要进行裁减，
        tp_data2.assign(tp_data.begin(),tp_data.begin()+maxindex-index);
    }else tp_data2=tp_data;
    Seg seg(tp_data2,tp_index);
    //cout<<seg.length<<endl;
    auto it_forworad=cached_substring.lower_bound(seg);
    //我们先向后合并
    while(it_forworad!=cached_substring.end()&&merger(seg,*it_forworad)==true){
        remove(it_forworad);
        it_forworad=cached_substring.lower_bound(seg);
    }
    //现在向前合并
    auto it_back=cached_substring.lower_bound(seg);
    if(it_back!=cached_substring.begin()){//寻找到的第一个index大于seg的不是第一个元素或是没有比seg的index大的元素的情况
        it_back--;//此时无论是大于seg.index还是为end()都不能向前合并
        while(merger(seg,*it_back)!=false){
            remove(it_back);
            it_back=cached_substring.lower_bound(seg);
            if(it_back==cached_substring.begin()) break;
            it_back--;
        }
    }
    //合并完成我们现在插入
    insert(seg);
    cout<<"before write,unreassembled_bytes:"<<unreassemblerd_bytes_num<<endl;
    wtriteToStream();
    cout<<"after write,unreassemblerd_bytes_num="<<unreassemblerd_bytes_num<<endl;

    if(eof) end_flag=true;
    if(end_flag&&(unreassemblerd_bytes_num==0)) {
        cout<<"endiput"<<endl;
        _output.end_input();
        }
    cout<<"eof"<<end_flag<<"\noutput_endinputflag:"<<_output.eof()<<endl;
}

bool StreamReassembler::merger(Seg &seg1,const Seg &seg2){
    Seg x,y;
    if(seg1.index<seg2.index){
        x=seg1;
        y=seg2;
    }else{
        x=seg2;
        y=seg1;
    }
    //上一步保证了x是index小的
    if(x.index+x.length<y.index) return false;//合并失败
    if(x.index+x.length>=y.index+y.length){
        //x包含了y的情况
        seg1=x;
        return true;
    }else{//y包含或是有重叠部分的时候
        seg1.index=x.index;
        seg1.data=x.data+y.data.substr(x.index+x.length-y.index);
        seg1.length=seg1.data.size();
        return true;
    }
}
size_t StreamReassembler::unassembled_bytes() const { return unreassemblerd_bytes_num; }

bool StreamReassembler::empty() const { return unreassemblerd_bytes_num==0;}

size_t StreamReassembler::wtriteToStream(){
    cout<<"write_to_stream called"<<endl;
    size_t cnt=0;
    auto it=cached_substring.begin();
    if(it==cached_substring.end()) return 0;
    while(it!=cached_substring.end()&&it->index==first_unreassembled){
        _output.write(it->data);
        cnt+=it->length;
        this->first_unreassembled+=it->length;
        remove(it);
        it=cached_substring.begin();
    }
    cout<<"read:"<<cnt<<"       firunre...:"<<first_unreassembled<<endl;
    return cnt;
}