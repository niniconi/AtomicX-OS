void kernel(){
    int * vrom = (int *)0xffff8000000a0000;
    int i,j;
    for (i=0,j=0;i<1440;i++,j=0){
        while(j++<4)
            *(vrom + j*1440 + i)=0x000000ff;
    }
    for (i=0,j=4;i<1440;i++,j=4){
        while(j++<8)
            *(vrom + j*1440 + i)=0x0000ff00;
    }
    while(1);
}
