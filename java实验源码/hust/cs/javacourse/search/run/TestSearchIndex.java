package hust.cs.javacourse.search.run;

import hust.cs.javacourse.search.index.AbstractDocumentBuilder;
import hust.cs.javacourse.search.index.AbstractIndex;
import hust.cs.javacourse.search.index.AbstractIndexBuilder;
import hust.cs.javacourse.search.index.impl.DocumentBuilder;
import hust.cs.javacourse.search.index.impl.IndexBuilder;
import hust.cs.javacourse.search.index.impl.Term;
import hust.cs.javacourse.search.parse.AbstractTermTupleStream;
import hust.cs.javacourse.search.query.AbstractHit;
import hust.cs.javacourse.search.query.AbstractIndexSearcher;
import hust.cs.javacourse.search.query.Sort;
import hust.cs.javacourse.search.query.impl.IndexSearcher;
import hust.cs.javacourse.search.query.impl.SimpleSorter;
import hust.cs.javacourse.search.util.Config;

import javax.swing.plaf.nimbus.AbstractRegionPainter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;

/**
 * 测试搜索
 */
public class TestSearchIndex {
    public static void main(String [] argv){
        SimpleSorter simpleSorter=new SimpleSorter();
        AbstractDocumentBuilder documentBuilder=new DocumentBuilder();
        AbstractIndexBuilder indexBuilder=new IndexBuilder(documentBuilder);
        AbstractIndex index=indexBuilder.buildIndex(Config.PROJECT_HOME_DIR+"/text");
        AbstractIndexSearcher indexSearcher=new IndexSearcher();
        try {
            ObjectOutputStream oos=new ObjectOutputStream(new FileOutputStream("C:\\Users\\zhdqq\\Desktop\\file.txt"));
            index.writeObject(oos);
        } catch (IOException e) {
            e.printStackTrace();
        }
        indexSearcher.open("C:\\Users\\zhdqq\\Desktop\\file.txt");
        AbstractHit[] hits=indexSearcher.search(new Term("test"),new Term("aaa"),simpleSorter, AbstractIndexSearcher.LogicalCombination.AND);
        for(AbstractHit hit:hits)
            System.out.println(hit.toString());
    }
    /**
     *  搜索程序入口
     * @param args ：命令行参数
     */

}
