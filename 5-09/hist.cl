#define HIST_DEP 256
__kernel
void histogram(__global int *input_img,int num,__global int *hist_array){
    __local hist_local[HIST_DEP];
    int lid=get_local_id(0);
    int gid=get_global_id(0);
    int lsize=get_local_size(0);
    int gsize=get_global_size(0);
    for(int i=lid;i<HIST_DEP;i+=gsize){
        hist_local[i]=0;
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    for(int i=gid;i<num;i+=gsize){
        atomic_add(&hist_local[i],1);
    }  

    barrier(CLK_LOCAL_MEM_FENCE);

    for(int i=lid;i<HIST_DEP;i+=lsize){
        atomic_add(&hist_array[i],&hist_local[i]);
    }

}