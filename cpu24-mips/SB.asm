#SB 测试    revise date:2018/3/14 tiger
#依次输出   0x00000000 0x00000001 0x00000002 0x00000003 0x00000004 0x00000005 0x00000006 0x00000007 0x00000008 0x00000009 0x0000000a 0x0000000b 0x0000000c 0x0000000d 0x0000000e 0x0000000f 0x00000010 0x00000011 0x00000012 0x00000013 0x00000014 0x00000015 0x00000016 0x00000017 0x00000018 0x00000019 0x0000001a 0x0000001b 0x0000001c 0x0000001d 0x0000001e 0x0000001f 0x03020100 0x07060504 0x0b0a0908 0x0f0e0d0c 0x13121110 0x17161514 0x1b1a1918 0x1f1e1d1c
.text

addi $t1,$zero,0     #init_addr 
addi $t3,$zero,32     #counter

#sb写入 01,02,03,04
addi $s1,$zero, 0x00  #
addi $s2,$zero, 0x01  #

sb_store:
sb $s1,($t1)
add $a0,$0,$s1          
addi $v0,$zero,34        # system call for print
syscall                  # print

add $s1,$s1,$s2          #data +1
addi $t1,$t1,1           # addr ++  
addi $t3,$t3,-1          #counter
bne $t3,$zero,sb_store

addi $t3,$zero,8
addi $t1,$zero,0    # addr   
sb_branch:
lw $s1,($t1)       #读出数据 
add $a0,$0,$s1          
addi $v0,$zero,34        # system call for print
syscall                  # print
addi $t1,$t1,4    
addi $t3,$t3, -1    
bne $t3,$zero,sb_branch

addi   $v0,$zero,10      # system call for exit
syscall                  # we are out of here.  


#bgez 测试    大于等于零跳转   递减运算 ，从正数开始向零运算revise date:2018/3/12 tiger  
#依次输出0x0000000f 0x0000000e 0x0000000d 0x0000000c 0x0000000b 0x0000000a 0x00000009 0x00000008 0x00000007 0x00000006 0x00000005 0x00000004 0x00000003 0x000000020 x000000010 x00000000
.text
addi $s1,$zero,15  #初始值
bgez_branch:
add $a0,$0,$s1          
addi $v0,$zero,34         
syscall                  # 输出当前值
addi $s1,$s1,-1
bgez $s1,bgez_branch   #测试指令

addi   $v0,$zero,10         #停机指令
syscall                  # 系统调用


#divu mflo测试   11110000 /2/2/2  ....  revise date:2018/3/14 tiger
#依次输出0x11110000 0x08888000 0x04444000 0x02222000 0x01111000 0x00888800 0x00444400 0x00222200 0x00111100 0x00088880 0x00044440 0x00022220 0x00011110 0x00008888 0x00004444 0x00002222 0x00001111 0x00000888 0x00000444 0x00000222 0x00000111 0x00000088 0x00000044 0x00000022 0x00000011 0x00000008 0x00000004 0x00000002 0x00000001
.text

addi $t0,$zero,2     # /2
addi $s1,$zero,0x1111     
sll $s1,$s1,16
add $a0,$0,$s1           
addi $v0,$zero,34         
syscall                  
addi $t3,$zero,28         #循环次数

divu_branch:
 divu $s1,$t0             #测试指令
 mflo $s1                 #测试指令
 add $a0,$0,$s1          
 addi $v0,$zero,34         
 syscall                  #输出当前值
 addi $t3,$t3, -1    
 bne $t3,$zero,divu_branch   #循环


addi   $v0,$zero,10         
syscall                  # 暂停或退出


