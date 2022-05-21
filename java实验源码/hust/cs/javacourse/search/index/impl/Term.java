package hust.cs.javacourse.search.index.impl;

import hust.cs.javacourse.search.index.AbstractTerm;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public  class Term extends AbstractTerm {
    /**
     * Term的默认构造函数
     */
    public Term(){

    }

    /**
     * 构造函数
     * @param contents：term的内容
     */
    public Term(String contents){
        super(contents);//调用了父类的构造函数
    }

    /**
     * 比较两个term的内容是否相同
     * @param obj ：要比较的另外一个Term
     * @return 内容相同返回true
     */
    @Override
    public boolean equals(Object obj) {
        if(this.content.equals(((Term)obj).content)) return true;
        else return false;
    }

    /**
     *
     * @return 返回一个该term的内容
     */
    @Override
    public String toString() {
        return this.content;
    }

    /**
     * 返回
     * @return 返回content的内容
     */
    @Override
    public String getContent() {
        return this.content;
    }

    @Override
    public void setContent(String content) {
        this.content=content;
    }

    @Override
    public int compareTo(AbstractTerm o) {
        return this.content.compareTo(o.getContent());//直接调用contens的比较函数
    }

    @Override
    public void writeObject(ObjectOutputStream out) {
        try {
            out.writeObject(this.content);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void readObject(ObjectInputStream in) {
        try {
            this.content=(String) in.readObject();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }
}
