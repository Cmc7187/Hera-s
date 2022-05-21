package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.util.StopWords;

import java.util.Arrays;
import java.util.List;

public class StopWordTermTupleFilter extends AbstractTermTupleFilter {
    /**
     * 停用词转化为列表可以调用JDK来查询是否存在某个单词
     */
    protected static List<String> stop_WordsList= Arrays.asList(StopWords.STOP_WORDS);



    /**
     * 被装饰者作为参数构造对象
     * @param input:实际是一个构造好的TermTupleScanner对象
     */
    public StopWordTermTupleFilter(AbstractTermTupleStream input) {
        super(input);
    }

    /**
     * 获得下一个三元组
     *
     * @return: 下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next() {
        AbstractTermTuple nowTermTuple=input.next();//根据多态调用的是TermTupleScanner生成好的三元组TermTuple
        if(nowTermTuple==null) return null;//已经到达文件尾
        if(stop_WordsList.contains(nowTermTuple.term.toString())) return this.next();//如果包含就递归调用该next函数获得下一个三元组
        else return nowTermTuple;
    }
}
