package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleFilter;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.util.Config;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class PatternTermTupleFilter extends AbstractTermTupleFilter {
    /**
     * 过滤非英文单词的正则表达式
     */
    private  String Regex = Config.TERM_FILTER_PATTERN;
    private Pattern pattern = null;
    private Matcher match = null;



    public PatternTermTupleFilter(AbstractTermTupleStream input) {
        super(input);
        this.pattern=Pattern.compile(Regex);
    }

    @Override
    public AbstractTermTuple next() {
        AbstractTermTuple termTuple=this.input.next();
        if(termTuple==null) return null;//达到文件尾部
        this.match=this.pattern.matcher(termTuple.term.toString());//传入需要匹配的字符串构建匹配器
        if(this.match.matches()) return termTuple;//匹配成功
        else return this.next();//匹配失败就递归调用
    }
}
