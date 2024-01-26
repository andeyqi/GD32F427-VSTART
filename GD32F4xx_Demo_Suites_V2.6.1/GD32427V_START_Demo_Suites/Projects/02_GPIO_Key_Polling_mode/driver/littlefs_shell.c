#include "lfs_mflash.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "littleshell.h"

#define SHELL_Printf printf
#define PRINTF    printf

/*******************************************************************************
 * Variables
 ******************************************************************************/

lfs_t lfs;
struct lfs_config cfg;
int lfs_mounted;


static unsigned int format(char argc, char **argv)
{
    int res;

    if (lfs_mounted)
    {
        SHELL_Printf("LFS is mounted, please unmount it first.\r\n");
        return 1;
    }

    if (argc != 2 || strcmp(argv[1], "yes"))
    {
        SHELL_Printf("Are you sure? Please issue command \"format yes\" to proceed.\r\n");
        return 1;
    }

    res = lfs_format(&lfs, &cfg);
    if (res)
    {
        PRINTF("\rError formatting LFS: %d\r\n", res);
    }

    return 1;
}

LTSH_FUNCTION_EXPORT(format,"lfs format api");

static unsigned int  mount(char argc, char **argv)
{
    int res;

    if (lfs_mounted)
    {
        SHELL_Printf("LFS already mounted\r\n");
        return 1;
    }

    res = lfs_mount(&lfs, &cfg);
    if (res)
    {
        PRINTF("\rError mounting LFS\r\n");
    }
    else
    {
        lfs_mounted = 1;
    }

    return 1;
}
LTSH_FUNCTION_EXPORT(mount,"lfs mount api");

static unsigned int unmount(char argc, char **argv)
{
    int res;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 1;
    }

    res = lfs_unmount(&lfs);
    if (res)
    {
        PRINTF("\rError unmounting LFS: %i\r\n", res);
    }

    lfs_mounted = 0;
    return 1;
}
LTSH_FUNCTION_EXPORT(unmount,"lfs unmount api");

static unsigned int cd(char argc, char **argv)
{
    SHELL_Printf(
        "There is no concept of current directory in this example.\r\nPlease always specify the full path.\r\n");
    return 1;
}
LTSH_FUNCTION_EXPORT(cd,"lfs cd api");


static unsigned int lls(char argc,  char **argv)
{
    int res;
    char *path;
    lfs_dir_t dir;
    struct lfs_info info;
    int files;
    int dirs;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 1;
    }

    if (argc > 2)
    {
        SHELL_Printf("Invalid number of parameters\r\n");
        return 1;
    }

    if (argc < 2)
    {
        path = "/";
    }
    else
    {
        path = argv[1];
    }

    /* open the directory */
    res = lfs_dir_open(&lfs, &dir, path);
    if (res)
    {
        PRINTF("\rError opening directory: %i\r\n", res);
        return 1;
    }

    PRINTF(" Directory of %s\r\n", path);
    files = 0;
    dirs  = 0;

    /* iterate until end of directory */
    while ((res = lfs_dir_read(&lfs, &dir, &info)) != 0)
    {
        if (res < 0)
        {
            /* break the loop in case of an error */
            PRINTF("\rError reading directory: %i\r\n", res);
            break;
        }

        if (info.type == LFS_TYPE_REG)
        {
            SHELL_Printf("%8d %s\r\n", info.size, info.name);
            files++;
        }
        else if (info.type == LFS_TYPE_DIR)
        {
            SHELL_Printf("%     DIR %s\r\n", info.name);
            dirs++;
        }
        else
        {
            SHELL_Printf("%???\r\n");
        }
    }

    res = lfs_dir_close(&lfs, &dir);
    if (res)
    {
        PRINTF("\rError closing directory: %i\r\n", res);
        return 1;
    }

    PRINTF(" %d File(s), %d Dir(s)\r\n", files, dirs);

    return 1;
}
LTSH_FUNCTION_EXPORT(lls,"lfs ls api");


static unsigned int rm(int32_t argc, char **argv)
{
    int res;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 1;
    }

    res = lfs_remove(&lfs, argv[1]);

    if (res)
    {
        PRINTF("\rError while removing: %i\r\n", res);
    }

    return 1;
}
LTSH_FUNCTION_EXPORT(rm,"lfs rm api");


static unsigned int lmkdir(char argc, char **argv)
{
    int res;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 1;
    }

    res = lfs_mkdir(&lfs, argv[1]);

    if (res)
    {
        PRINTF("\rError creating directory: %i\r\n", res);
    }

    return 1;
}
LTSH_FUNCTION_EXPORT(lmkdir,"lfs mkdir api");

static unsigned int write(char argc, char **argv)
{
    int res;
    lfs_file_t file;

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 1;
    }

    res = lfs_file_open(&lfs, &file, argv[1], LFS_O_WRONLY | LFS_O_APPEND | LFS_O_CREAT);
    if (res)
    {
        PRINTF("\rError opening file: %i\r\n", res);
        return 1;
    }

    res = lfs_file_write(&lfs, &file, argv[2], strlen(argv[2]));
    if (res > 0)
        res = lfs_file_write(&lfs, &file, "\r\n", 2);

    if (res < 0)
    {
        PRINTF("\rError writing file: %i\r\n", res);
    }

    res = lfs_file_close(&lfs, &file);
    if (res)
    {
        PRINTF("\rError closing file: %i\r\n", res);
    }

    return 1;
}
LTSH_FUNCTION_EXPORT(write,"lfs write api");


static unsigned int cat(char argc, char **argv)
{
    int res;
    lfs_file_t file;
    uint8_t buf[16+1];

    if (!lfs_mounted)
    {
        SHELL_Printf("LFS not mounted\r\n");
        return 1;
    }

    res = lfs_file_open(&lfs, &file, argv[1], LFS_O_RDONLY);
    if (res)
    {
        PRINTF("\rError opening file: %i\r\n", res);
        return 1;
    }

    do
    {
        res = lfs_file_read(&lfs, &file, buf, sizeof(buf));
        if (res < 0)
        {
            PRINTF("\rError reading file: %i\r\n", res);
            break;
        }
        if(res > 0)
        {
            buf[res] = '\0';
            PRINTF("%s",(char *)buf);
        }
    } while (res);

    res = lfs_file_close(&lfs, &file);
    if (res)
    {
        PRINTF("\rError closing file: %i\r\n", res);
    }

    return 1;
}
LTSH_FUNCTION_EXPORT(cat,"lfs cat api");

static unsigned int lfsinit(char argc, char **argv)
{
    lfs_get_default_config(&cfg);
    return 1;
}
LTSH_FUNCTION_EXPORT(lfsinit,"lfs init api");


static unsigned int df(char argc, char **argv)
{
    printf("used block %d\r\n",lfs_fs_size(&lfs));
    return 1;
}
LTSH_FUNCTION_EXPORT(df,"lfs init api");

 void getcwd(char * buff, int len)
 {
     if(lfs_mounted)
     {  
        buff[0] = '/';
        buff[1] = ' ';
        buff[2] = '\0';
     }
 }
 
 
 void fs_init(void)
 {
    int res;
    lfs_get_default_config(&cfg);
    
    res = lfs_mount(&lfs, &cfg);
    if (res)
    {
        PRINTF("\rError mounting LFS\r\n");
        
        res = lfs_format(&lfs, &cfg);
        if (res)
        {
            PRINTF("\rError formatting LFS: %d\r\n", res);
        }
        else
        {
            res = lfs_mount(&lfs, &cfg);
            if(res)
              PRINTF("\rError mounting LFS\r\n");
        }
    }
    else
    {
        lfs_mounted = 1;
    }
 };