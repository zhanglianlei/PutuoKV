#ifndef __DB_H__
#define __DB_H__

#define MAX_DATA_FILE    2 << 10
#define WRITE_POS_FILE   "write.pos"

/*
 * DataBlock is a key-value data record. It exists in the data file.
 */
struct DataBlock {
    long crc;
    unsigned int timeStamp;
    unsigned int keySize;
    void *key;
    unsigned int valueSize;
    void *value;
};

/*
 * WriteOffset is write.pos file, fix the position of data file
 * quickly when append.
 */
struct  WriteOffset {
    unsigned int fileNo;    //current active file
    unsigned int offset;    //offset of current active file
};

/*
 * DataFile is a log type common file for storing key-value data.
 * That's append only, all the written are just append only, not
 * modify the old data, it likes a log file. When size of the file
 * reached a limited, a new data file is constructed, the older
 * files are read-only.
 */
struct DataFile {
    char fileName[100];    //xxx.w, xxx is number, auto-increment, 
                           //maximum number is active data file,
                           //others are older data files.
    unsigned int fileNo;   //file descriptor
    unsigned int size;     //maximum size is 1GB
    char flag;             //0:Read-only, 1:Writable
};

/*
 * DataFileNode is a node list of the data file. Use the double list
 * to link all the data files.
 */
struct DataFileNode {
    struct DataFile *file;
    struct DataFileNode *prev;
    struct DataFileNode *next;
};


/*
 * DataBase is a log type, based on HashMap, key-value storage.
 */
typedef struct DataBase {
    char dataBaseName[20];            //name of DB.
    unsigned int currentFileNumber;   //current maximum number of active data file.
    struct HashMap *hashIndex;        //index data in memory based on HashMap for reading data quickly.
    unsigned int posFileNo;           //fd of 'write.pos' file.
    struct WriteOffset *writePos;     //write.pos file, fix the position of data file quickly when append.
    struct DataFile *activeDataFile;  //current active data file, append data into this file.
    struct DlistHead *olderDataFiles; //the older data files.
    struct MergedDataFileList *mergedDataFiles;  //data files which have merged.
    pthread_cond_t merge;
    pthread_mutex_t mutex;
}DB;

DB *minikvConnectDB(const char *dbName, struct dbOptions *options);
void destroyDB(DB *db);

#endif //__DB_H__

