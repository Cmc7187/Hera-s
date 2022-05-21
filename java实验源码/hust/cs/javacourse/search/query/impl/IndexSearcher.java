package hust.cs.javacourse.search.query.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractPostingList;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.query.AbstractHit;
import hust.cs.javacourse.search.query.AbstractIndexSearcher;
import hust.cs.javacourse.search.query.Sort;

import java.io.*;
import java.util.*;

public class IndexSearcher extends AbstractIndexSearcher {
    /**
     * 从指定索引文件打开索引，加载到index对象里. 一定要先打开索引，才能执行search方法
     *
     * @param indexFile ：指定索引文件
     */
    @Override
    public void open(String indexFile) {
        try {
            ObjectInputStream ois=new ObjectInputStream(new FileInputStream(indexFile));
            this.index.readObject(ois);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * 根据单个检索词进行搜索
     *
     * @param queryTerm ：检索词
     * @param sorter    ：排序器
     * @return ：命中结果数组
     */
    @Override
    public AbstractHit[] search(AbstractTerm queryTerm, Sort sorter) {
        AbstractHit [] hits=null;
        if(this.index.termToPostingListMapping.containsKey(queryTerm)){//这是查找到的情况下
            AbstractPostingList postingList=this.index.termToPostingListMapping.get(queryTerm);//获得对应的PostingList
            hits=new AbstractHit[postingList.size()];
            for(int i=0;i<postingList.size();i++){
                int docID=postingList.get(i).getDocId();//获得该posting的文档ID
                String docPath=index.getDocName(docID);//通过文档ID获得绝对路径
                Map<AbstractTerm, AbstractPosting> TermToPosting=new TreeMap<>();
                TermToPosting.put(queryTerm,postingList.get(i));
                AbstractHit hit=new Hit(docID,docPath,TermToPosting);//构建Hit对象
                sorter.score(hit);//计算搜索得分
                hits[i]=hit;
            }
            List<AbstractHit> list=Arrays.asList(hits);//先将hits转化为列表，我们进行排序
            sorter.sort(list);//排序
            list.toArray(hits);//再将list转换为数组
            return hits;
        }else return hits;//查找不到包含该词语的文档，返回NULL
    }

    /**
     * 根据二个检索词进行搜索
     *
     * @param queryTerm1 ：第1个检索词
     * @param queryTerm2 ：第2个检索词
     * @param sorter     ：    排序器
     * @param combine    ：   多个检索词的逻辑组合方式
     * @return ：命中结果数组
     */
    @Override
    public AbstractHit[] search(AbstractTerm queryTerm1, AbstractTerm queryTerm2, Sort sorter, LogicalCombination combine) {
        if(queryTerm1.equals(queryTerm2)) return this.search(queryTerm1,sorter);//两个关键字一样的时候就不用管逻辑关系直接调用单个检索词的搜索
        AbstractPostingList postingList1=null;
        AbstractPostingList postingList2=null;
        postingList1=index.termToPostingListMapping.get(queryTerm1);
        postingList2=index.termToPostingListMapping.get(queryTerm2);
        if(combine==LogicalCombination.OR){//两个检索词是或的关系的时候
        if(postingList1!=null||postingList2!=null){//两个当中至少有一个term存在的时候
            if(postingList1!=null&&postingList2!=null){//两个都关键词都检索到的时候
                //这个时候我们直依次调用两个词语的搜索，然后合并起来再排序然后返回hit数组
                AbstractHit[] hits1=this.search(queryTerm1,sorter);
                AbstractHit[] hits2=this.search(queryTerm2,sorter);
                AbstractHit[] AddHits=new Hit[hits1.length+hits2.length];
                System.arraycopy(hits1,0,AddHits,0,hits1.length);
                System.arraycopy(hits2,0,AddHits,hits1.length,hits2.length);
                List<AbstractHit> list=Arrays.asList(AddHits);//先将hits转化为列表，我们进行排序
                sorter.sort(list);//排序
                list.toArray(AddHits);//再将list转换为数组
                return AddHits;
            }else{//只有其中一个检索到的时候
                if(postingList1!=null){//检索词1查找的情况
                    return this.search(queryTerm1,sorter);
                }else{//检索词2查找到的时候
                    return this.search(queryTerm2,sorter);
                }
            }
        }else{//两个term都不存在的时候
            return null;
        }
        }else{//两个检索词是与的关系的时候
            if(postingList1!=null&&postingList2!=null){//只有两个词都查找到的时候才会查找到命中数组
                ArrayList<AbstractHit> list=new ArrayList<>();//由于hit个数未知，我们先用可变长列表来储存，最后再转化为数组返回
                for(int i=0;i<postingList1.size();i++){//构建一个双重循环查找两个单词都出现过的文档
                    for(int j=0;j<postingList2.size();j++){
                        //由于两个list是按照docID排好序的，所以当list2的docID比当前i对应的docId大的时候就绝对找不到了
                        if(postingList1.get(i).getDocId()==postingList2.get(j).getDocId()){
                            //出现公共部分
                            int DocId=postingList1.get(i).getDocId();
                            String DocPath=this.index.getDocName(DocId);
                            Map<AbstractTerm,AbstractPosting> termToPostingMapping=new TreeMap<>();
                            termToPostingMapping.put(queryTerm1,postingList1.get(i));
                            termToPostingMapping.put(queryTerm2,postingList2.get(j));
                            AbstractHit hit=new Hit(DocId,DocPath,termToPostingMapping);//构建新的hit，这里的map有两个对
                            sorter.score(hit);
                            list.add(hit);
                            break;
                        }
                    }
                }
                sorter.sort(list);//进行排序
                AbstractHit[] hits=new Hit[list.size()];
                list.toArray(hits);
                return hits;//将构建好的hits返回
            }else return null;
        }
    }
}
