package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.*;

import java.io.*;
import java.util.Map;
import java.util.Set;

public class Index extends AbstractIndex {
    /**
     * 返回索引的字符串表示
     *
     * @return 索引的字符串表示
     *
     */
    @Override
    public String toString() {
        StringBuffer stringBuffer=new StringBuffer();
        stringBuffer.append("DocID------->DocPath：\n");
        for(Map.Entry<Integer,String> entry:this.docIdToDocPathMapping.entrySet()){
            stringBuffer.append("DocId: "+entry.getKey()+"   -->    DocPath:"+entry.getValue()+'\n');
        }
        stringBuffer.append("Term---->PostingList：\n");
        for(Map.Entry<AbstractTerm,AbstractPostingList>entry:this.termToPostingListMapping.entrySet()){
            stringBuffer.append("Term: "+entry.getKey()+"    -->   PostingList:"+entry.getValue()+'\n');
        }
        return stringBuffer.toString();
    }

    /**
     * 添加文档到索引，更新索引内部的HashMap
     *
     * @param document ：文档的AbstractDocument子类型表示
     */
    @Override
    public void addDocument(AbstractDocument document) {
        this.docIdToDocPathMapping.put(document.getDocId(),document.getDocPath());//更新文档ID和路径的键值对
        //接下来更新term->postingList
        AbstractTermTuple termTuple;
        for(int i=0;i<document.getTupleSize();i++){//依次遍历每一个三元组
             termTuple=document.getTuple(i);
             if(this.termToPostingListMapping.containsKey(termTuple.term)){//判断该term是否出现在任何一篇文档中
                 AbstractPostingList postingList=this.termToPostingListMapping.get(termTuple.term);//获得该term对应的postingList
                 int index=postingList.indexOf(document.getDocId());
                 if(index==-1){//判断该term是否第一次出现在该文档中
                     //我们需要构建一个新的posting，然后加入postingList
                     AbstractPosting posting=new Posting();
                     posting.setFreq(1);
                     posting.setDocId(document.getDocId());
                     posting.getPositions().add(termTuple.curPos);
                     postingList.add(posting);
                 }else{
                     //我们不需要构建新的posting，只需要更新出现的位置和频率就可以
                     AbstractPosting posting=postingList.get(index);//获得已经创建好的posting,然后去修改对应的值
                     int newFreq=posting.getFreq()+1;//出现频率加一
                     posting.setFreq(newFreq);
                     posting.getPositions().add(termTuple.curPos);//加入新的位置
                 }
             }else{//该term还没有和任何的文档进行联系,我们要创建一个新的postingList
                AbstractPostingList postingList=new PostingList();
                AbstractPosting posting=new Posting();
                 posting.setFreq(1);
                 posting.setDocId(document.getDocId());
                 posting.getPositions().add(termTuple.curPos);
                 postingList.add(posting);
                 this.termToPostingListMapping.put(termTuple.term,postingList);//构建,map
             }
        }
    }

    /**
     * <pre>
     * 从索引文件里加载已经构建好的索引.内部调用FileSerializable接口方法readObject即可
     * @param file ：索引文件
     * </pre>
     */
    @Override
    public void load(File file) {
        try {
            ObjectInputStream ois=new ObjectInputStream(new FileInputStream(file.getPath()));
            this.readObject(ois);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    /**
     * <pre>
     * 将在内存里构建好的索引写入到文件. 内部调用FileSerializable接口方法writeObject即可
     * @param file ：写入的目标索引文件
     * </pre>
     */
    @Override
    public void save(File file) {
        try {
            ObjectOutputStream oos=new ObjectOutputStream(new FileOutputStream(file.getPath()));
            this.writeObject(oos);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 返回指定单词的PostingList
     *
     * @param term : 指定的单词
     * @return ：指定单词的PostingList;如果索引字典没有该单词，则返回null
     */
    @Override
    public AbstractPostingList search(AbstractTerm term) {
        if(this.termToPostingListMapping.containsKey(term)) return this.termToPostingListMapping.get(term);
        else return null;//不包含该key
    }

    /**
     * 返回索引的字典.字典为索引里所有单词的并集
     *
     * @return ：索引中Term列表
     */
    @Override
    public Set<AbstractTerm> getDictionary() {
        return this.termToPostingListMapping.keySet();
    }

    /**
     * <pre>
     * 对索引进行优化，包括：
     *      对索引里每个单词的PostingList按docId从小到大排序
     *      同时对每个Posting里的positions从小到大排序
     * 在内存中把索引构建完后执行该方法
     * </pre>
     */
    @Override
    public void optimize() {
        Set<Map.Entry<AbstractTerm,AbstractPostingList>>entrySet=this.termToPostingListMapping.entrySet();
        for(Map.Entry<AbstractTerm,AbstractPostingList> set:entrySet){//增强for循环来将每一个posting排序
            AbstractPostingList postingList=set.getValue();
            postingList.sort();
            for(int i=0;i<postingList.size();i++) postingList.get(i).sort();
        }
    }

    /**
     * 根据docId获得对应文档的完全路径名
     *
     * @param docId ：文档id
     * @return : 对应文档的完全路径名
     */
    @Override
    public String getDocName(int docId) {
        if(this.docIdToDocPathMapping.containsKey(docId)) return this.docIdToDocPathMapping.get(docId);
        else return null;
    }

    /**
     * 写到二进制文件
     *
     * @param out :输出流对象
     */
    @Override
    public void writeObject(ObjectOutputStream out) {
        try {
            out.writeObject(this.docIdToDocPathMapping);
            out.writeObject(this.termToPostingListMapping);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 从二进制文件读
     *
     * @param in ：输入流对象
     */
    @Override
    public void readObject(ObjectInputStream in) {
        try {
            this.docIdToDocPathMapping=(Map<Integer, String>) in.readObject();
            this.termToPostingListMapping=(Map<AbstractTerm, AbstractPostingList>) in.readObject();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }
}
