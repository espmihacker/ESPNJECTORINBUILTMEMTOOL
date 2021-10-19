#ifndef MEMORY_H
#define MEMORY_H

#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include "MemTool/Loger.h"
#include <string>

typedef int8_t BYTE;
typedef double DOUBLE;
typedef uint32_t DWORDk;
typedef uint64_t QWORD;
typedef int16_t WORD;
//typedef char GameName;

struct little_map
{
 std::uintptr_t address;
 std::int64_t value;
};

enum TYPES{

    TYPE_QWORD,
    TYPE_DWORD,
    TYPE_WORD,
    TYPE_BYTE,
    TYPE_FLOAT,
    TYPE_DOUBLE,

};
std::string GameNameS="com.pubg.imobile";
char GameName[100]="com.pubg.imobile";
char UE4[] = "libUE4.so";
char Tersafe[] = "libtersafe.so";
long int lib_base=0;
int handleme=0;


void setName(int code);
long int get_module_base(int pid, const char *module_name);
bool checkPid();
bool checkRoot();
void Write(long int address, const char *value, TYPES type);
void WriteQword(long int address, QWORD value);
void WriteDword(long int address, DWORDk value);
void WriteWord(long int address, WORD value);
void WriteByte(long int address, BYTE value);
void WriteFloat(long int address, float value);
void WriteDouble(long int address, DOUBLE value);
long int getRealOffset(long int address);
int getPid(const char *pkg_name);
int isApkRunning(const char *pkg_name);

int isApkRunning(const char *pkg_name){
    if (getPid(pkg_name) != 0 && getPid(pkg_name) > 0){
        return 1;
    }
    return 0;
}


int getPid(const char *pkg_name){
    int pid = 0;
    int id;
    DIR* dir;
    FILE* fp;
    char filename[32];
    char cmdline[256];

    struct dirent* entry;
    if (pkg_name == NULL) {
        return -1;
    }
    dir = opendir("/proc");
    if (dir == NULL) {
        return -1;
    }
    while ((entry = readdir(dir)) != NULL) {
        id = atoi(entry->d_name);
        if (id != 0) {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp) {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(pkg_name, cmdline) == 0) {
                    pid = id;
                    break;
                }
            }
        }
    }
    closedir(dir);
    return pid;
}

long int getRealOffset(long int address)
{
    if (lib_base == 0)
        LOGE("Error: Can't get memory base address");
    return lib_base + address;
}

void WriteQword(long int address, QWORD value) {
    pwrite64(handleme, (void*)&value, sizeof(value), address);
}
void WriteDword(long int address, DWORDk value) {
    pwrite64(handleme, (void*)&value, sizeof(value), address);
}
void WriteWord(long int address, WORD value) {
    pwrite64(handleme, (void*)&value, sizeof(value), address);
}
void WriteByte(long int address, BYTE value) {
    pwrite64(handleme, (void*)&value, sizeof(value), address);
}
void WriteFloat(long int address, float value) {
    pwrite64(handleme, (void*)&value, sizeof(value), address);
}
void WriteDouble(long int address, DOUBLE value) {
    pwrite64(handleme, (void*)&value, sizeof(value), address);
}
void Clear() {
    close(handleme);
}

void Write(long int address, const char *value, TYPES type){
    switch (type) {
        case TYPE_QWORD:
            WriteQword(address, atoi(value));
            break;
        case TYPE_DWORD:
            WriteDword(address, atoi(value));
            break;
        case TYPE_WORD:
            WriteWord(address, atoi(value));
            break;
        case TYPE_BYTE:
            WriteByte(address, atoi(value));
            break;
        case TYPE_FLOAT:
            WriteFloat(address, atof(value));
            break;
        case TYPE_DOUBLE:
            WriteDouble(address, atof(value));
            break;
    }
}

bool checkRoot(){

    char getRoot[100];
    if (getuid() == 0)
    {
        sprintf(getRoot, "MODE_ROOT");
        return true;
    }
    else
    {
        sprintf(getRoot, "MODE_NO_ROOT");
        return false;
    }

}

bool checkPid(){

    int ipid = getPid(GameName);

    char lj[64];
    sprintf(lj, "/proc/%d/mem", ipid);
    handleme = open(lj, O_RDWR);
    if (handleme == -1)
    {
        puts("Gagal mendapatkan memory!\n");
        puts("Restart Game, Execute in Lobby/Game!\n");
        return false;
        //exit(1);

    }else{
        return true;
    }


}

long int get_module_base(int pid, const char *module_name){
    FILE *fp;
    unsigned int addr = 0;
    char filename[32], buffer[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "rt");
    if (fp != nullptr) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strstr(buffer, module_name)) {
                addr = (long int)strtoull(buffer, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}
/*long int get_module_base(int pid, const char *module_name)
{

    FILE *fp;
    long addr = 0;
    char *pch;
    char filename[32];
    char line[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, module_name))
            {
                pch = strtok(line, "-");
                addr = strtoul(pch, NULL, 16);
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}*/
void setName(int code){

    //sprintf(GameName,"%s","com.pubg.imobile")

    switch ((int)code) {

        case 1:
            GameNameS="com.tencent.ig";
            sprintf(GameName,"%s","com.tencent.ig"); break;
        case 2:
            GameNameS="com.pubg.krmobile";
            sprintf(GameName,"%s","com.pubg.krmobile"); break;
        case 3:
            GameNameS="com.vng.pubgmobile";
            sprintf(GameName,"%s","com.vng.pubgmobile"); break;
        case 4:
            GameNameS="com.rekoo.pubgm";
            sprintf(GameName,"%s","com.rekoo.pubgm"); break;
        case 5:
            GameNameS="com.pubg.imobile";
            sprintf(GameName,"%s","com.pubg.imobile"); break;


    }

}



#endif //MEMORY_H
