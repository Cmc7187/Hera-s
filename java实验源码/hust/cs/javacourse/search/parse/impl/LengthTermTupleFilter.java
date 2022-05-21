package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.util.Config;

/**
 * 包装者，过滤掉长度不符合要求的单词
 */
public class LengthTermTupleFilter extends AbstractTermTupleFilter {
    public LengthTermTupleFilter(AbstractTermTupleStream input) {
        super(input);//这里的input实际是停用词过滤器的对象
    }



    /**
     * 获得下一个三元组
     *
     * @return: 下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next() {
        AbstractTermTuple nowTermTuple=input.next();
        if(nowTermTuple==null) return null;//上一步已经符合过滤掉或者是到达末尾
        if(nowTermTuple.term.toString().length()< Config.TERM_FILTER_MINLENGTH
                ||nowTermTuple.term.toString().length()>Config.TERM_FILTER_MAXLENGTH)return this.next();//长度不符合
        else return nowTermTuple;
    }
}
