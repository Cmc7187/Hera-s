package hust.cs.javacourse.search.query.impl;

import hust.cs.javacourse.search.index.AbstractPosting;
import hust.cs.javacourse.search.index.AbstractTerm;
import hust.cs.javacourse.search.query.AbstractHit;
import hust.cs.javacourse.search.query.Sort;

import java.util.Collections;
import java.util.List;
import java.util.Map;

public class SimpleSorter implements Sort {
    @Override
    public void sort(List<AbstractHit> hits) {
        Collections.sort(hits);//hit实现比较接口，直接掉API进行比较
    }

    @Override
    public double score(AbstractHit hit) {
        double Score=0;
        for(Map.Entry<AbstractTerm, AbstractPosting> entry:hit.getTermPostingMapping().entrySet()){
            AbstractPosting posting=entry.getValue();//获得term和对应的posting
            Score+=posting.getFreq();
        }
        hit.setScore(-Score);//直接使用频率作为得分
        return Score;//返回实际得分
    }
}
