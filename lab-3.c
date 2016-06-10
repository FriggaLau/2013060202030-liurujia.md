#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <set>
#include <cmath>
#include <cassert>

using namespace std;

typedef unsigned long long u64;

int page_recsz = 4, pagesz = 4, lev = 1;
int pageoffset = 12;//页内偏移地址位数
u64 addr;
u64 offsetmask; //页内偏移掩码
u64 tmp[64]; //临时生成的映射关系
u64 ids[64]; //各级页号

inline int getPageOffset(int x){
    for(int i=0; i<32; i++){
        if((x>>i)==0)return i-1;
    }
    return 0;
}

void printtable(){//将各级页表的映射关系进行打印
    printf("Page Table：\n");
    for(int i=0; i<lev-1; i++){
        printf("PageTable %d,Item 0x%lX is：0x%lX \n",
                    i+1, ids[i], tmp[i]);
    }
    printf("pageTable %d, Item 0x%lX is PhysicBlockNum ：0x%lX\n\n", lev, ids[lev-1], tmp[lev-1]);
}

int main(){
    //freopen("1.txt", "r", stdin);
    srand(time(NULL));

    while(true){
        cout << "please input the pageItemSize, pageSize, pageTableLevel" << endl;
        cin>>page_recsz>>pagesz>>lev;
        if(page_recsz<8){
            cout<<"PageRecord大小至少为8B"<<endl;
            continue;
        }
        pageoffset = 10 + getPageOffset(pagesz);
        offsetmask = (1<<pageoffset)-1;
        int com;
        u64 paddr, inneroffset;
        cout<<"Input LogicAddr\t2.Input Ctrl+C to exit"<<endl;
        cin>>com;
        if(com==1){
            scanf("%lX", &addr);
            inneroffset = offsetmask&addr;
            paddr = mapping();
            paddr = (paddr<<pageoffset) | inneroffset;//将paddr（页号）与pageoffset（页内偏移拼接）进行拼接，得到物理地址
            printf("PhysicalAddr：0x%lX  PhysicalBlockNo：0x%lX  Offset：0x%lX\n\n", paddr,
                        ((~offsetmask)&paddr)>>pageoffset, inneroffset);
            printtable();
        }else break;
    }
    OUT:

    return 0;
}


u64 mapping(){//映射到物理地址
    int levsize = getPageOffset(pagesz*1024/page_recsz);  //除了第1级外每级的位数
    u64 mask = (1<<levsize)-1;
    ids[0] = addr>>(levsize*(lev-1)+pageoffset);
    for(int i=1; i<lev; i++){//获取各级的页号
        ids[i] = (mask<<(pageoffset + (lev-1-i)*levsize)) & addr;
        ids[i] = ids[i]>>(pageoffset + (lev-1-i)*levsize);
    }

    set<u64> repeater;
    for(int i=0; i<lev-1; i++){//随机生成页表的内容
        int x = rand();
        while(repeater.find(x)!=repeater.end()){
            x = rand();
        }//去重
        repeater.insert(x);
        tmp[i] = x;
    }
    //finally physical address=logical address
    //physical number in last page + pageoffset = logical address = physical address
    tmp[lev-1] = ((~offsetmask)&addr)>>pageoffset;
    printtable();
    return tmp[lev-1];
}

