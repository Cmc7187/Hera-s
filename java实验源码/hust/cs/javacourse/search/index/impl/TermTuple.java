package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.index.AbstractTermTuple;

public class TermTuple extends AbstractTermTuple {
    /**
     * 两个参数的构造函数
     * @param term ：包含的单词
     * @param position：当前的位置
     */
    public TermTuple(AbstractTerm term,int position) {
        this.curPos=position;
        this.term=term;
    }

    public TermTuple() {
    }

    /**
     * 判断二个三元组内容是否相同
     *
     * @param obj ：要比较的另外一个三元组
     * @return 如果内容相等（三个属性内容都相等）返回true，否则返回false
     */
    @Override
    public boolean equals(Object obj) {
        if(!(obj instanceof TermTuple)) return false;//不是相同的类的时候直接返回false
        TermTuple compared_obj=(TermTuple) obj;
        if(this.curPos==compared_obj.curPos&&this.term.equals(compared_obj.term)) return true;//频率绝对是1
         else return false;
    }

    /**
     * 获得三元组的字符串表示
     *
     * @return ： 三元组的字符串表示
     */
    @Override
    public String toString() {
        return "Term:"+this.term.toString()+",frq:"+this.freq+",position:"+this.curPos;
    }
}
