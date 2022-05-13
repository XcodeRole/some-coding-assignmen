#include <iostream>
#include <stdio.h>
#include <vector>
#include <CL/opencl.hpp>
#include <cstring>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cl;
using namespace cv;

#define HI(num)	(((num) & 0x0000FF00) << 8)
#define LO(num)	((num) & 0x000000FF)


// cl_device_id getdevice(int platform_id, int device_id)
// {
// 	cl_platform_id *platforms;
// 	cl_device_id *devices;
// 	cl_uint platform_size, device_size;
// 	int err;
// 	err = clGetPlatformIDs(0, NULL, &platform_size);
// 	platforms = (cl_platform_id*)malloc(sizeof(platform_id)*platform_size);
// 	err = clGetPlatformIDs(1, platforms, NULL);
// 	err = clGetDeviceIDs(platforms[platform_id], CL_DEVICE_TYPE_ALL, 0, NULL, &device_size);
// 	devices = (cl_device_id*)malloc(sizeof(cl_device_id)*device_size);
// 	err = clGetDeviceIDs(platforms[platform_id], CL_DEVICE_TYPE_ALL, sizeof(cl_device_id)*device_size, devices, NULL);
// 	cl_device_id dev(devices[device_id]); // = devices[device_id];
// 	free(platforms); 
//     free(devices);
// 	return dev;
// }

//**********************************************************************
//read PGM data
typedef struct _PGMData {
    int row;
    int col;
    int max_gray;
    int **matrix;
} PGMData;

int **allocate_dynamic_matrix(int row, int col)
{
    int **ret_val;
    int i;
 
    ret_val = (int **)malloc(sizeof(int *) * row);
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }
 
    for (i = 0; i < row; ++i) {
        ret_val[i] = (int *)malloc(sizeof(int) * col);
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }
 
    return ret_val;
}
 
void deallocate_dynamic_matrix(int **matrix, int row)
{
    int i;
    for (i = 0; i < row; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

void SkipComments(FILE *fp)
{
    int ch;
    char line[100];
    while ((ch = fgetc(fp)) != EOF && isspace(ch)) {
        ;
    }
 
    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        SkipComments(fp);
    } else {
        fseek(fp, -1, SEEK_CUR);
    }
}
/*For reading:*/
PGMData* readPGM(const char *file_name, PGMData *data)
{
    FILE *pgmFile;
    char version[3];
    int i, j;
    int lo, hi;
    pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL) {
        perror("cannot open file to read");
        exit(EXIT_FAILURE);
    }
    fgets(version, sizeof(version), pgmFile);
    if (strcmp(version, "P5")) {
        fprintf(stderr, "Wrong file type!\n");
        exit(EXIT_FAILURE);
    }
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->col);
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->row);
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->max_gray);
    fgetc(pgmFile);
 
    data->matrix = allocate_dynamic_matrix(data->row, data->col);
    if (data->max_gray > 255) {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                hi = fgetc(pgmFile);
                lo = fgetc(pgmFile);
                data->matrix[i][j] = (hi << 8) + lo;
            }
        }
    }
    else {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                lo = fgetc(pgmFile);
                data->matrix[i][j] = lo;
            }
        }
    }
 
    fclose(pgmFile);
    return data;
 
}
 
/*For writing*/
 
void writePGM(const char *filename, const PGMData *data)
{
    FILE *pgmFile;
    int i, j;
    int hi, lo;
 
    pgmFile = fopen(filename, "wb");
    if (pgmFile == NULL) {
        perror("cannot open file to write");
        exit(EXIT_FAILURE);
    }
 
    fprintf(pgmFile, "P5 ");
    fprintf(pgmFile, "%d %d ", data->col, data->row);
    fprintf(pgmFile, "%d ", data->max_gray);
 
    if (data->max_gray > 255) {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                hi = HI(data->matrix[i][j]);
                lo = LO(data->matrix[i][j]);
                fputc(hi, pgmFile);
                fputc(lo, pgmFile);
            }
        }
    }
    else {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                lo = LO(data->matrix[i][j]);
                fputc(lo, pgmFile);
            }
        }
    }
 
    fclose(pgmFile);
    deallocate_dynamic_matrix(data->matrix, data->row);
}

//**********************************************************************

int main(){

	//定义平台和设别
    int platform_id=0;
    int device_id=0;

	//获取平台以及设备id
	cl_platform_id *platforms;
	cl_device_id *devices;
	cl_uint platform_size, device_size;
	int err;//用于错误检查
	err = clGetPlatformIDs(0, NULL, &platform_size);
	platforms = (cl_platform_id*)malloc(sizeof(platform_id)*platform_size);
	err = clGetPlatformIDs(1, platforms, NULL);
	err = clGetDeviceIDs(platforms[platform_id], CL_DEVICE_TYPE_ALL, 0, NULL, &device_size);
	devices = (cl_device_id*)malloc(sizeof(cl_device_id)*device_size);
	err = clGetDeviceIDs(platforms[platform_id], CL_DEVICE_TYPE_ALL, sizeof(cl_device_id)*device_size, devices, NULL);
	cl_device_id dev=devices[device_id]; // = devices[device_id];
	// free(platforms); 
    // free(devices);


	//创建context
	cl_context_properties contextProperties[]={CL_CONTEXT_PLATFORM,(cl_context_properties)platforms[platform_id],0};
	cl_context context=clCreateContext(contextProperties,device_size,devices,NULL,NULL,NULL);

    //为指定设备创建工作队列
    cl_command_queue commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);

	//读取灰度图
    string pic_name="test.pgm";
	PGMData picRaw;
	PGMData *picMem=readPGM(pic_name.c_str(),&picRaw);
    
    //分配内存
    cl_mem clCreateBuffer()
	// string fd_name1="test1.pgm";
	// writePGM(fd_name1.c_str(),picMem);
	// Mat img=imread("test1.pgm",0);
	// imshow("das",img);
	// waitKey(0);

    //创建程序对象
    char kernal_fd[]="img_hist_equal.cl";
    ifstream kernelFile(kernal_fd, ios::in);
    ostringstream oss;
    oss<<kernelFile.rdbuf();
    string src_str=oss.str();
    const char *src_str_const=src_str.c_str();
    //src_str_const参数是cl源文件中的代码
    cl_program program=clCreateProgramWithSource(context,1,(const char**)&src_str_const,NULL,NULL);
    cl_int retnum=clBuildProgram(program,device_size,devices,NULL,NULL,NULL);





    cl_kernel kernel1= clCreateKerne(program,"imgToHist",NULL);
    err=clSetKernelArg(kernel1,0,);


}







