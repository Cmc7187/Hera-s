package hust.cs.javacourse.search.run;

import hust.cs.javacourse.search.index.*;
import hust.cs.javacourse.search.index.impl.*;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.parse.impl.StopWordTermTupleFilter;
import hust.cs.javacourse.search.parse.impl.LengthTermTupleFilter;
import hust.cs.javacourse.search.parse.impl.TermTupleScanner;
import hust.cs.javacourse.search.util.Config;

import java.io.*;
import java.util.*;

/**
 * 测试索引构建
 */
public class TestBuildIndex {
    /**
     *  索引构建程序入口
     * @param args : 命令行参数
     */
    public static void main(String[] args){
        String filePath=Config.DOC_DIR+"test.txt";
        /**
         * 测试TermTupleStream功能的代码
         */
        /*
        AbstractTermTuple tempTermTupele;
        while((tempTermTupele=input.next())!=null){
            System.out.println(tempTermTupele);
        }*/
        /**
         * 测试indexbuild的代码
         */
        AbstractDocumentBuilder documentBuilder=new DocumentBuilder();
        AbstractIndexBuilder indexBuilder=new IndexBuilder(documentBuilder);
        AbstractIndex index=indexBuilder.buildIndex(Config.PROJECT_HOME_DIR+"/text");
        System.out.println(index.toString());
        /**
         * 测试序列化的代码
         */
        /*
        List<Integer> list=new ArrayList<>();
        list.add(2);
        list.add(3);
        list.add(1);
        AbstractPosting posting1=new Posting(2,3,list);
        AbstractPostingList postingList1=new PostingList();
        postingList1.add(posting1);
        postingList1.add(new Posting(1,3,list));
        postingList1.add(new Posting(4,3,list));
        postingList1.add(new Posting(4,3,list));
        try {
            ObjectOutputStream oos=new ObjectOutputStream(new FileOutputStream("C:\\Users\\zhdqq\\Desktop\\file.txt"));
            ObjectInputStream ois=new ObjectInputStream(new FileInputStream("C:\\Users\\zhdqq\\Desktop\\file.txt"));
            AbstractIndex newIndex=new Index();
            index.writeObject(oos);
            newIndex.readObject(ois);
            System.out.println(newIndex);
        } catch (IOException e) {
            e.printStackTrace();
        }
         */
    }
}
