#ifndef _LFS_MFLASH_H_
#define _LFS_MFLASH_H_

#include "lfs.h"

struct lfs_mflash_ctx
{
    uint32_t start_addr;
};

extern int lfs_get_default_config(struct lfs_config *lfsc);

#endif