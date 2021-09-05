#ifndef __DB_H__
#define __DB_H__

#define MAX_DATA_FILE    2 << 10
#define WRITE_POS_FILE   "write.pos"

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

