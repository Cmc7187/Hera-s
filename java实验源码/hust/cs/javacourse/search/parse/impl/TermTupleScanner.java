package hust.cs.javacourse.search.parse.impl;

import hust.cs.javacourse.search.index.AbstractTermTuple;
import hust.cs.javacourse.search.index.impl.Term;
import hust.cs.javacourse.search.index.impl.TermTuple;
import hust.cs.javacourse.search.parse.AbstractTermTupleScanner;
import hust.cs.javacourse.search.util.Config;
import hust.cs.javacourse.search.util.FileUtil;
import hust.cs.javacourse.search.util.StringSplitter;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.List;
import java.util.Locale;

public class TermTupleScanner extends AbstractTermTupleScanner {
    /**
     * docFilePath:保存文档的路径
     */
    protected String docFilePath;

    /**
     * 用来储存获得的文本内容
     */
    protected  String contents;

    /**
     * 将内容分词之后的单词列表
     */
    protected List<String> words;

    /**
     * position用来计算单词的位置
     */
    protected int position=0;

    /**
     * 构造函数，
     * @param docFilePath 需要解析的文件路径
     */
    public TermTupleScanner(String docFilePath) {
        this.docFilePath = docFilePath;
        this.contents=FileUtil.read(this.docFilePath);//调用文件工具类将文件中数据读进来
        StringSplitter splitter = new StringSplitter();
        splitter.setSplitRegex(Config.STRING_SPLITTER_REGEX);//设置分词的正则表达式
        this.words=splitter.splitByRegex(this.contents);//获得分词的列表
    }

    /**
     * 构造函数
     *
     * @param input ：指定输入流对象，应该关联到一个文本文件
     */
    public TermTupleScanner(BufferedReader input) {
        super(input);
        StringBuffer stringBuffer=new StringBuffer();
        String s=null;
        try{ while( (s = input.readLine()) != null){
                stringBuffer.append(s).append("\n"); //reader.readLine())返回的字符串会去掉换行符，因此这里要加上
            }
        }catch (IOException e){
            e.printStackTrace();
        }
        this.contents = stringBuffer.toString().trim(); //去掉最后一个多的换行符
        StringSplitter splitter = new StringSplitter();
        splitter.setSplitRegex(Config.STRING_SPLITTER_REGEX);//设置分词的正则表达式
        this.words=splitter.splitByRegex(this.contents);//获得分词的列表
    }

    /**
     *
     * 获得下一个三元组
     *
     * @return: 下一个三元组；如果到了流的末尾，返回null
     */
    @Override
    public AbstractTermTuple next() {
        if(position>=this.words.size()) return null;//到达文件尾部
        else{
        AbstractTermTuple newTermTuple=new TermTuple(new Term((this.words.get(this.position)).toLowerCase()),this.position);//位置和下标的值差1
           this.position++;//位置向后推一
           return newTermTuple;//将创建好的对象返回
        }
    }

}
